/*
    Copyright (C) 2015  Intel Corporation

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <cstdio>
#include <cstdlib>
#if defined XFSTK_OS_WIN
#include <windows.h>
#endif
#include "../../common/xfstktypes.h"
#include "merrifielddownloader.h"

using namespace std;
extern CPSTR Merrifield_error_code_array [MAX_ERROR_CODE_MERRIFIIELD];  //JG - This is really used but it ijust a data array that we can just read.

MerrifieldDownloader::MerrifieldDownloader()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    this->b_provisionfailed = false;
    this->b_provisionhasstarted = false;
    this->CurrentDownloaderDevice = NULL;
    this->CurrentDownloaderOptions = NULL;
    this->DeviceSpecificOptions = NULL;
    this->b_DnX_OS = 0;
    this->abort = false;
}
bool MerrifieldDownloader::SetOptions(IOptions *options)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    bool RetVal = false;
    if(options != NULL) {
        this->CurrentDownloaderOptions = options;
        this->DeviceSpecificOptions = (MerrifieldOptions *) options;
        if(this->DeviceSpecificOptions->IsVerbose()) {
            this->SetDebugLevel(this->DeviceSpecificOptions->GetDebugLevel());
        }
        RetVal = true;
    }
    return RetVal;
}

bool MerrifieldDownloader::SetDevice(IGenericDevice *device)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    bool RetVal = false;
    this->m_dldr_state.InitStepsDone();
    if(device != NULL && this->CurrentDownloaderOptions !=NULL) {
        this->CurrentDownloaderDevice = (IDevice *) device;
        this->CurrentDownloaderDevice->SetUtilityInstance((void*) (&(this->libutils)));
        RetVal = true;
    }
    return RetVal;
}

bool MerrifieldDownloader::UpdateTarget()
{

    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    bool RetVal = false;
    bool OsDownloadRequested = false;
    if(this->CurrentDownloaderOptions == NULL || this->CurrentDownloaderDevice == NULL)
    {
        return RetVal;
    }

    if(this->DeviceSpecificOptions->IsPerformEmmcDumpEnabled())
    {
        this->Init();
        this->do_emmc_update(this->DeviceSpecificOptions);
        this->libutils.u_log(LOG_STATUS, "EMMC-DUMP: Download completed.");
        return true;
    }

    this->libutils.u_log(LOG_DOWNLOADER, "FWDnxPath -- %s", this->DeviceSpecificOptions->GetFWDnxPath());
    this->libutils.u_log(LOG_DOWNLOADER, "FWImagePath -- %s", this->DeviceSpecificOptions->GetFWImagePath());
    this->libutils.u_log(LOG_DOWNLOADER, "OSDnxPath -- %s", this->DeviceSpecificOptions->GetOSDnxPath());
    this->libutils.u_log(LOG_DOWNLOADER, "OSImagePath -- %s", this->DeviceSpecificOptions->GetOSImagePath());
    this->libutils.u_log(LOG_DOWNLOADER, "MiscDnxPath -- %s", this->DeviceSpecificOptions->GetMiscDnxPath());
    this->libutils.u_log(LOG_DOWNLOADER, "MiscBinPath -- %s", this->DeviceSpecificOptions->GetMiscBinPath());
    this->libutils.u_log(LOG_DOWNLOADER, "Gpflags -- %x", this->DeviceSpecificOptions->GetGPFlags());
    this->libutils.u_log(LOG_DOWNLOADER, "DebugLevel -- %x", this->DeviceSpecificOptions->GetDebugLevel());
    this->libutils.u_log(LOG_DOWNLOADER, "UsbDelay ms -- %d", this->DeviceSpecificOptions->GetUsbdelayms());
    this->libutils.u_log(LOG_DOWNLOADER, "TransferType -- %s", this->DeviceSpecificOptions->GetTransferType());
    this->libutils.u_log(LOG_DOWNLOADER, "WipeIFWI -- %s", (this->DeviceSpecificOptions->IsWipeIfwiEnabled())?"True":"False");
    this->libutils.u_log(LOG_DOWNLOADER, "Idrq -- %s", (this->DeviceSpecificOptions->IsIdrqEnabled())?"True":"False");
    this->libutils.u_log(LOG_DOWNLOADER, "Verbose -- %s", (this->DeviceSpecificOptions->IsVerbose())?"True":"False");

    this->Init();

    if(!this->b_usbinitok)
        return false;

    unsigned long inputflags = this->DeviceSpecificOptions->GetGPFlags();
    if(inputflags & 0x00000001)
    {
        this->b_continue_to_OS = true;
        OsDownloadRequested = true;
    }
    else
    {
        this->b_continue_to_OS = false;
    }

    this->do_update(this->DeviceSpecificOptions);
    this->b_provisionhasstarted = true;
    last_error er;
    GetLastError(&er);
    if((!OsDownloadRequested) && (er.error_code == 0)) {
        if ( this->m_dldr_state.IsGPPReset())
        {
            this->Init();
            if(!this->b_usbinitok)
                return false;
#if defined XFSTK_OS_WIN
            Sleep(5000);
#else
            sleep(5);
#endif
            this->do_update(this->DeviceSpecificOptions);
        }
        RetVal = this->m_dldr_state.GetFwStatus();
        if(this->m_dldr_state.IsProvisionFailed()) {
            this->libutils.u_log(LOG_STATUS, "FAIL");
            this->libutils.u_log(LOG_STATUS, "Errors encounter during FW download.");
            this->b_provisionfailed = true;
        }
        else {
            this->libutils.u_log(LOG_STATUS, "PASS");
            if(this->DeviceSpecificOptions->IsIdrqEnabled())
                this->libutils.u_log(LOG_STATUS, "IDRQ completed.");
            else if ((this->m_dldr_state.GetIFWIStatus())&&(this->DeviceSpecificOptions->IsWipeIfwiEnabled()))
                this->libutils.u_log(LOG_STATUS, "Wipe IFWI Process Completed.");
            else if ((!this->m_dldr_state.GetIFWIStatus())&&(this->DeviceSpecificOptions->IsWipeIfwiEnabled()))
                this->libutils.u_log(LOG_STATUS, "eMMC is virgin, IFWI not wiped.");
            else
                this->libutils.u_log(LOG_STATUS, "Firmware only download completed.");
            this->b_provisionfailed = false;
        }
    }
    else {
        RetVal = this->m_dldr_state.GetOsStatus();//isosdone();
        last_error er;
        GetLastError(&er);
        if(this->b_provisionhasstarted) {
            if((er.error_code != 0) && this->m_dldr_state.IsFwState() && !this->m_dldr_state.GetFwStatus()) {
                this->libutils.u_log(LOG_STATUS, "FAIL");
                this->libutils.u_log(LOG_STATUS, "Errors encounter during FW download. Aborting ...");
                this->b_provisionfailed = true;
                return false;
            } else if((er.error_code != 0) && this->m_dldr_state.IsOsState() && !RetVal) {
                this->libutils.u_log(LOG_STATUS, "FAIL");
                this->libutils.u_log(LOG_STATUS, "Errors encounter during OS download. Aborting ...");
                this->b_provisionfailed = true;
                return false;
            } else if(er.error_code == 0){
                if(this->DeviceSpecificOptions->IsIdrqEnabled())
                {
                    this->libutils.u_log(LOG_STATUS, "PASS");
                    this->libutils.u_log(LOG_STATUS, "IDRQ completed.");
                }
                else if(this->m_dldr_state.GetOsStatus() && this->m_dldr_state.IsOsOnly()) {
                    this->libutils.u_log(LOG_STATUS, "PASS");
                    this->libutils.u_log(LOG_STATUS, "OS only download completed.");
                } else if(this->m_dldr_state.GetOsStatus()) {
                    this->libutils.u_log(LOG_STATUS, "PASS");
                    this->libutils.u_log(LOG_STATUS, "Firmware and OS download completed.");
                } else if(this->m_dldr_state.IsOsOnly()) {
                    this->libutils.u_log(LOG_STATUS, "PASS");
                    this->libutils.u_log(LOG_STATUS, "Firmware download skipped. Continuing to OS...");
                } else if(this->m_dldr_state.IsFwState()){
                    this->libutils.u_log(LOG_STATUS, "PASS");
                    this->libutils.u_log(LOG_STATUS, "Firmware download completed. Continuing to OS...");
                }
                this->b_provisionfailed = false;
            } else if(er.error_code != 0){
                this->libutils.u_log(LOG_STATUS, "FAIL");
                this->libutils.u_log(LOG_STATUS, "Errors encounter during download. Aborting...");
                this->b_provisionfailed = true;
            }
        }
    }
    return RetVal;
}

bool MerrifieldDownloader::GetStatus()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    if(this->CurrentDownloaderOptions == NULL ||
       this->CurrentDownloaderDevice == NULL) {
        return 0x0;
    }
    return !(this->b_provisionfailed);
}

bool MerrifieldDownloader::GetLastError(last_error* er)
{
    return m_dldr_state.GetLastError(er);
}

bool MerrifieldDownloader::SetDebugLevel(unsigned long DebugLevel)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    if(DebugLevel > 0) {
        this->libutils.isDebug = DebugLevel;
    }
    return true;
}

bool MerrifieldDownloader::SetStatusCallback(void(*StatusPfn)(char* Status, void* ClientData), void* ClientData)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    this->libutils.u_setstatuspfn(StatusPfn,ClientData);
    return true;
}
int MerrifieldDownloader::GetResponse(unsigned char *buffer, int maxsize)
{
    return this->m_dldr_state.getResponseBuffer(buffer,maxsize);
}

bool MerrifieldDownloader::Cleanup()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    return true;
}
void MerrifieldDownloader::Init()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    b_usbinitok = false;
    bool result = true;
    result = this->CurrentDownloaderDevice->Open();
    this->b_usbinitok = result;
}
void MerrifieldDownloader::ReInit()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    this->CurrentDownloaderDevice->Init();
    this->CurrentDownloaderDevice->Open();
#if defined XFSTK_OS_WIN
    Sleep(1000);
#else
    sleep(1);
#endif
    this->CurrentDownloaderDevice->Init();
    this->CurrentDownloaderDevice->Open();
}

void MerrifieldDownloader::do_abort()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    this->CurrentDownloaderDevice->Abort();
}

void MerrifieldDownloader::cleanup()
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
}

void MerrifieldDownloader::do_emmc_update(MerrifieldOptions* options)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    m_dldr_state.Init(this->CurrentDownloaderDevice, &this->libutils);
    m_dldr_state.SetOptions(options);
    m_dldr_state.DoEmmcUpdate(
                            (PSTR)options->GetFWDnxPath(),
                            options->GetEmmcFile(),
                            options->GetEmmcTokenOffset(),
                            options->GetEmmcExpirationDur(),
                            options->GetEmmcPartition(),
                            options->GetEmmcBlockSize(),
                            options->GetEmmcBlockCount(),
                            options->GetEmmcOffset(),
                            options->IsEmmcUmipDumpEnabled(),
                            options->GetUsbdelayms()
                          );
    this->do_abort();
}

void MerrifieldDownloader::do_update(MerrifieldOptions* options)
{
    this->libutils.u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
    m_dldr_state.SetIDRQstatus(options->IsIdrqEnabled());
    m_dldr_state.SetCsdbStatus(options->GetCSDBStatus(),options->directCSDBStatus());
    m_dldr_state.Init(this->CurrentDownloaderDevice, &this->libutils);
    m_dldr_state.DoUpdate((PSTR)options->GetFWDnxPath(),
                          (PSTR)options->GetFWImagePath(),
                          (PSTR)options->GetOSDnxPath(),
                          (PSTR)options->GetOSImagePath(),
                          (PSTR)options->GetMiscDnxPath(),
                          options->GetGPFlags(),
                          options->GetUsbdelayms(),
                          options->IsWipeIfwiEnabled(),
                          (PSTR)options->GetMiscBinPath()
                          );
    this->do_abort();
}
