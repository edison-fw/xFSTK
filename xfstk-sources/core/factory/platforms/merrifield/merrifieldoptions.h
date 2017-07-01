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
#ifndef MERRIFIELDOPTIONS_H
#define MERRIFIELDOPTIONS_H

#include <string>
#include "../../interfaces/ioptions.h"
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;


#define CSDB_SIG_SIZE  4

class MerrifieldOptions : public IOptions
{
private:
    string SoftfusesPath; //not supported on this platform, used for parseing generic platforms (superset strategy)
    string fwDnxPath;
    string fwImagePath;
    string osDnxPath;
    string osImagePath;
    string miscDnxPath;
    string miscBinPath;
    string csdbStatus;
    unsigned int gpFlags;
    unsigned int debugLevel;
    unsigned int usbdelayms;
    unsigned int targetIndex;
    int readWriteTimeout;
    string transferType;
    bool idrqEnabled;
    bool isVerbose;
    bool wipeifwi;
    bool downloadFW;
    bool downloadOS;
    bool isActionRequired;
    bool initcsdb;
    bool finalcsdb;
    void UpdateFlags();
    bool allPathsAreValid();
    void ParseLegacy(int, char*[]);
    bool validateCSDBState();
    //emmc
    string file;
    string uFwDnx;
    int partition;
    long blockSize;
    long blockCount;
    long offset;
    string tokenOffset;
    string expirationDuration;
    bool umipdump;
    bool m_isRegisterToken;
    bool performEmmcDump;

public:
    MerrifieldOptions();
    void SetDefaults();
    void Parse(int, char*[]);
    void Clear();
    void PrintAllOptions();
    DeviceTransportType GetTransportType();
    const char* GetFWDnxPath();
    const char* GetFWImagePath();
    const char* GetOSDnxPath();
    const char* GetOSImagePath();
    const char* GetMiscDnxPath();
    const char* GetMiscBinPath();
    string GetCSDBStatus();
    unsigned char directCSDBStatus();
    unsigned int GetTarget();
    unsigned int GetGPFlags();
    unsigned int GetDebugLevel();
    unsigned int GetUsbdelayms();
    int GetReadWriteTimeout() {return readWriteTimeout;}
    const char* GetTransferType();
    const char* GetQuery();
    bool IsIdrqEnabled();
    bool IsFWDownload();
    bool IsOSDownload();
    bool IsQuery();
    bool IsActionRequired();
    bool IsVerbose();
    bool IsWipeIfwiEnabled();
    int serialComPort;
    //emmc
    string GetEmmcFile(){return file;}
    string GetEmmcUnsignedFwDNX(){return uFwDnx;}
    string GetEmmcTokenOffset(){return tokenOffset;}
    string GetEmmcExpirationDur() const { return expirationDuration; }
    int GetEmmcPartition(){return partition;}
    long GetEmmcBlockSize(){return blockSize;}
    long GetEmmcBlockCount(){return blockCount;}
    long GetEmmcOffset(){return offset;}
    bool IsEmmcUmipDumpEnabled(){return umipdump;}
    bool IsPerformEmmcDumpEnabled(){return performEmmcDump;}
    bool IsRegisterToken() const { return m_isRegisterToken; }
};
#endif // MerrifieldOPTIONS_H
