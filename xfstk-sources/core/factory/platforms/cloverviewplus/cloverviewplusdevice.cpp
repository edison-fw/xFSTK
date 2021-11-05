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
#include "cloverviewplusdevice.h"
#include "../../common/xfstkcommon.h"
#include "../../common/xfstktypes.h"

extern IntelSoCGlobalDeviceState IntelSoCGlobalDeviceCurrentState;

CloverviewPlusDevice::CloverviewPlusDevice()
{
#ifdef XFSTK_OS_WIN
    deviceFactory = NULL;
#endif
    this->PhysicalTransportDevice = NULL;
}
CloverviewPlusDevice::~CloverviewPlusDevice()
{
    if(this->PhysicalTransportDevice != NULL)
        delete this->PhysicalTransportDevice;

#ifdef XFSTK_OS_WIN
    if(deviceFactory) {
        delete deviceFactory;
        deviceFactory = NULL;
    }
#endif
}
int CloverviewPlusDevice::GetNumberAvailableDevices()
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->GetNumberAvailableDevices();
    return RetVal;
}

int CloverviewPlusDevice::GetNumberAvailableDevices(DeviceInfos* socdevices)
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->GetNumberAvailableDevices(socdevices);
    return RetVal;
}

bool CloverviewPlusDevice::SetUtilityInstance(void *UtilityInstance)
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->SetUtilityInstance(UtilityInstance);
    return RetVal;
}

void* CloverviewPlusDevice::GetDeviceHandle(int DeviceNumber)
{
    void *RetVal = NULL;
    RetVal = this->PhysicalTransportDevice->GetDeviceHandle(DeviceNumber);
    return RetVal;
}
bool CloverviewPlusDevice::SetDeviceHandle(void *DeviceHandle)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->SetDeviceHandle(DeviceHandle);
    return RetVal;
}
bool CloverviewPlusDevice::SetTransport(DeviceTransportType Transport)
{
    bool RetVal = false;
    switch(Transport) {
    case XFSTK_USB20:
#ifdef XFSTK_OS_WIN
        if(deviceFactory) {
            delete deviceFactory;
            deviceFactory = NULL;
        }
        deviceFactory = new WDDeviceFactory();
        if(deviceFactory) {
            this->PhysicalTransportDevice = (IDevice *) deviceFactory->CreateDevice(CLOVERVIEWPLUS_DEVICE_ID);
            if(this->PhysicalTransportDevice) {
                RetVal = true;
            }
        }
        break;
#else
        this->PhysicalTransportDevice = (IDevice *) new CloverviewPlusUSB20Device();
        if(this->PhysicalTransportDevice) {
            RetVal = true;
        }
        break;
#endif
    case XFSTK_SERIAL:
        this->PhysicalTransportDevice = (IDevice *) new CloverviewPlusSerialDevice();
        if(this->PhysicalTransportDevice) {
            RetVal = true;
        }
        break;
    case XFSTK_SOCKET:
        RetVal = false;
        break;
    case XFSTK_ITP:
        RetVal = false;
        break;
    case XFSTK_USB30:
        RetVal = false;
        break;
    case XFSTK_NOT_SPECIFIED:
        RetVal = false;
        break;
    default:
        RetVal = false;
        break;
    }
    return RetVal;
}

bool CloverviewPlusDevice::Open()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Open();
    return RetVal;
}

bool CloverviewPlusDevice::Init()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Init();
    return RetVal;
}

bool CloverviewPlusDevice::Abort()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Abort();
    return RetVal;
}

bool CloverviewPlusDevice::Write(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Write(pBuffer, bufferSize);
    return RetVal;
}

bool CloverviewPlusDevice::Read(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Read(pBuffer, bufferSize);
    return RetVal;
}

bool CloverviewPlusDevice::GetAck(void *pAck, unsigned int *ackSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->GetAck(pAck, ackSize);
    return RetVal;
}

int CloverviewPlusDevice::Detect()
{   int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->Detect();
    return RetVal;
}

bool CloverviewPlusDevice::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{   bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->IsCorrectProtocol(szBuff, bytes_rxed);
    return RetVal;
}

bool CloverviewPlusDevice::IsSupportedDevice(unsigned short vendorid, unsigned short productid)
{   bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->IsSupportedDevice(vendorid, productid);
    return RetVal;
}

void CloverviewPlusDevice::GetUsbsn(char * usbsn)
{
    this->PhysicalTransportDevice->GetUsbsn(usbsn);
}

#ifdef XFSTK_OS_WIN

#else

//////////////////////////////////////
CloverviewPlusUSB20Device::CloverviewPlusUSB20Device()
{
    this->bus = NULL;
    this->dev = NULL;
    this->dev_port = NULL;
    this->dev_extern_mt = NULL;
    this->busses = NULL;
    this->dev_handle = NULL;
    this->target = 0;
    this->numinits = 0;
    this->reqnuminits = 1;
    this->libutils = NULL;
    this->id_product = CLOVERVIEWPLUS_PRODUCT_ID;
    this->id_vendor = INTEL_VENDOR_ID;
    USBTIMEOUT = 5000;
    if(!IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized) {
        usb_init();
        IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized = true;
        if(IntelSoCGlobalDeviceCurrentState.IsUsb20DebugEnabled) {
            usb_set_debug(255);
        }
    }
}
CloverviewPlusUSB20Device::~CloverviewPlusUSB20Device()
{
    if(dev_handle) {
        usb_close(dev_handle);
        dev_handle = NULL;
    }
}

int CloverviewPlusUSB20Device::GetNumberAvailableDevices()
{
    struct usb_device *dev = NULL;
    struct usb_bus *bus = NULL;
    int availnumdevs = 0;

    usb_find_busses();
    usb_find_devices();
    usb_get_busses();

    for (bus = usb_busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next) {
            if(this->IsSupportedDevice(dev->descriptor.idVendor, dev->descriptor.idProduct)) {
                availnumdevs++;
            }
        }
    }

    return availnumdevs;
}

int CloverviewPlusUSB20Device::GetNumberAvailableDevices(DeviceInfos* socdevices)
{
    struct usb_device *dev = NULL;
    struct usb_bus *bus = NULL;
    usb_dev_handle *handle;
    int availnumdevs = 0;
    int i;

    usb_find_busses();
    usb_find_devices();
    usb_get_busses();

    i = socdevices->number_devices;

    for (bus = usb_busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next) {
            if(this->IsSupportedDevice(dev->descriptor.idVendor, dev->descriptor.idProduct)) {
                availnumdevs++;
                if (i >= 32)
                    continue;
                handle = usb_open(dev);
                if (!handle)
                    continue;
                memset(&socdevices->soc_device[i], 0, sizeof(DeviceInfo));
                usb_get_string_simple(handle, dev->descriptor.iSerialNumber, socdevices->soc_device[i].usbsn, 128);
                usb_close(handle);
                i++;
            }
        }
    }

    socdevices->number_devices = i;

    return availnumdevs;
}

void* CloverviewPlusUSB20Device::GetDeviceHandle(int DeviceNumber)
{
    struct usb_device *dev = NULL;
    struct usb_bus *bus = NULL;
    int availnumdevs = 0;

    usb_find_busses();
    usb_find_devices();
    usb_get_busses();

    for (bus = usb_busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next) {
            if(this->IsSupportedDevice(dev->descriptor.idVendor, dev->descriptor.idProduct)) {
                if(availnumdevs == DeviceNumber) {
                    return (void *) dev;
                }
                availnumdevs++;
            }
        }
    }
    return NULL;
}

bool CloverviewPlusUSB20Device::SetDeviceHandle(void *DeviceHandle)
{
    bool RetVal = false;
    int ret = 0;

    if(DeviceHandle != NULL) {
        this->CurrentDeviceHandle = DeviceHandle;

        dev_handle = NULL;
        dev_handle = usb_open((struct usb_device *)DeviceHandle);

        if(dev_handle == NULL) {
            return false;
        }
        memset(this->usbsn, 0, 18);
        ret = usb_get_string_simple(dev_handle, ((struct usb_device *)DeviceHandle)->descriptor.iSerialNumber, this->usbsn,17);
        usb_close(dev_handle);
        dev_handle = NULL;
        RetVal = true;
    }
    return RetVal;
}
bool CloverviewPlusUSB20Device::VerifyDeviceHandle(struct usb_device* Handle)
{
    struct usb_device *dev = NULL;
    struct usb_bus *bus = NULL;
    char myusbsn[128];

    usb_find_busses();
    usb_find_devices();
    usb_get_busses();

    for (bus = usb_busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next) {
            if(this->IsSupportedDevice(dev->descriptor.idVendor, dev->descriptor.idProduct)) {
                if(Handle == dev) {
                    dev_handle = usb_open(dev);

                    if(dev_handle == NULL) {
                        return false;
                    }
                    memset(myusbsn, 0, 18);
                    usb_get_string_simple(dev_handle, dev_port->descriptor.iSerialNumber, myusbsn,17);
                    if(memcmp(myusbsn, this->usbsn, 16)) {
                        this->libutils->u_log( LOG_USB, "USBSN: %s is not matching up with %s ", this->usbsn, myusbsn);
                        usb_close(dev_handle);
                        dev_handle = NULL;
                        return false;
                    } else {
                        usb_close(dev_handle);
                        dev_handle = NULL;
                        this->libutils->u_log( LOG_USB, "USBSN: find usbdev %x ", dev);
                        return true;
                    }
                }

            }
        }
    }
    return false;
}
void CloverviewPlusUSB20Device::FindDevice(void)
{

    int targetsfound = 0;
    bool targetsearchcomplete = false;
    bool found = false;

    if(this->CurrentDeviceHandle != NULL) {
       this->dev_port = (struct usb_device *)(this->CurrentDeviceHandle);//this->dev_extern_mt;
       this->dev = (struct usb_device *) this->CurrentDeviceHandle;//this->dev_extern_mt;
       targetsearchcomplete = true;
       this->libutils->u_log(LOG_USB, "USB Device found - Externally Specified for MT");
       this->libutils->u_log(LOG_USB, "USB Device Handle - %x", this->dev_port);
       found = true;
    }
    else {
        usb_find_busses();
        usb_find_devices();
        busses = usb_get_busses();
        for (bus = usb_busses; bus; bus = bus->next)
        {
            for (dev = bus->devices; dev; dev = dev->next)
            {
                if(this->IsSupportedDevice(dev->descriptor.idVendor, dev->descriptor.idProduct))  {
 #if defined XFSTK_MULTIDEVICE
                    if(targetsfound != this->target) {
                        targetsfound++;
                        continue;
                    }
                    this->dev_port = dev;
                    targetsearchcomplete = true;
#else
                    this->dev_port = bus->devices;
#endif
                    this->libutils->u_log(LOG_USB, "USB Device found");
                    found = true;
                }
                if(targetsearchcomplete) {
                    break;
                }
            }
            if(targetsearchcomplete) {
                break;
            }
        }
    }

    if (!found)
    {
#if defined XFSTK_OS_WIN
        this->libutils->u_log( LOG_USB, "Device Vendor id:%#x, productId:%#x  NOT FOUND!!",
              this->id_vendor, this->id_product);
#else
        this->libutils->u_log( LOG_USB, "Device Vendor id:%#x, productId:%#x  NOT FOUND!!",
              this->id_vendor, this->id_product);
        this->libutils->u_abort("Device Vendor id:%#x, productId:%#x  NOT FOUND!!");
#endif
    }
    else {
        dev_handle = NULL;
#if 0
        if(!this->VerifyDeviceHandle(this->dev_port)) {
            this->libutils->u_abort("Invalidate device handle, aborting...");
            return;
        } else {
            this->libutils->u_log( LOG_USB,"Device handle is valid");
        }
#endif
        dev_handle = usb_open(dev_port);
        this->libutils->u_log( LOG_USB,"USB dev_handle -- %x", dev_handle);
        if(dev_handle == NULL) {
            return;
        }
        memset(this->usbsn, 0, 18);
        usb_get_string_simple(dev_handle, dev_port->descriptor.iSerialNumber, this->usbsn,17);
        this->libutils->u_log( LOG_USB, "USBSN: %s", this->usbsn);
        this->libutils->SetUsbsn(this->usbsn);
        usb_close(dev_handle);
        dev_handle = NULL;

        numinits++;
        if(numinits == reqnuminits) {
            dev_handle = usb_open(dev_port);

            if(dev_handle == NULL) {
                this->libutils->u_log( LOG_USB, "Invalid USB dev_handle return %x", dev_handle);
                return;
            }

            if(!dev_port->config) {
                this->libutils->u_log( LOG_USB, "USB invalid dev_port->config: %x", dev_port->config);
                usb_close(dev_handle);
                dev_handle = NULL;
                return;
            }

            int failed_claim = 0;
#if defined XFSTK_OS_WIN
            if(!(usb_set_configuration(dev_handle, dev_port->config->bConfigurationValue)))
            {
#endif
                usb_interface_descriptor* puid = dev_port->config->interface->altsetting;
                for(uint32 idx = 0; idx < puid->bNumEndpoints; idx++)
                {
                    if(puid->endpoint[idx].bmAttributes != USB_ENDPOINT_TYPE_BULK) {
                        continue;
                    }
                    uint32 uiEndPoint = puid->endpoint[idx].bEndpointAddress;
                    if((uiEndPoint & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_IN) {
                        this->libutils->u_log(LOG_USB, "Bulk IN endpoint in  : 0x%02X", uiEndPoint);
                        in_pipe = uiEndPoint;
                    }
                    if((uiEndPoint & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_OUT) {
                        this->libutils->u_log(LOG_USB,"Bulk OUT endpoint in  : 0x%02X", uiEndPoint);
                        out_pipe = uiEndPoint;
                    }
                }
                if(!(usb_claim_interface(dev_handle, dev_port->config->interface->altsetting->bInterfaceNumber))) {
                    failed_claim++;

                    if(failed_claim > 10) {
                        usb_close(dev_handle);
                        dev_handle = NULL;
                        this->libutils->u_abort("LOST DEVICE");
                    }
                }
#if defined XFSTK_OS_WIN
           }
           else {
                usb_close(dev_handle);
                dev_handle = NULL;
                this->libutils->u_abort("LOST DEVICE");
           }
#endif
        }
    }
        return;
}

bool CloverviewPlusUSB20Device::Open()
{
    bool RetVal = true;
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    this->Init();

    FindDevice();

    if(dev_handle == NULL)
        RetVal = false;
    return RetVal;
}

bool CloverviewPlusUSB20Device::Init()
{
    bool RetVal = true;
    this->dev_handle = NULL;
    this->in_pipe    = 0;
    this->out_pipe   = 0;
    this->numinits = 0;
    this->reqnuminits = 1;
    return RetVal;
}
bool CloverviewPlusUSB20Device::SetUtilityInstance(void *UtilityInstance)
{
    if(UtilityInstance) {
        this->libutils = (CloverviewPlusUtils *) UtilityInstance;
        return true;
    }
    return false;
}

bool CloverviewPlusUSB20Device::Abort()
{
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    if(dev_handle){
        usb_close(dev_handle);
        dev_handle = NULL;
    }
    return true;
}

bool CloverviewPlusUSB20Device::Write(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = true;
    this->libutils->u_log(LOG_USB, "%s --->%s", __PRETTY_FUNCTION__, this->libutils->makestring((PUCHAR)pBuffer, bufferSize).c_str());
    int bytes_written = 0;
    ULONG delay_time = 2000;    //USBTIMEOUT;
    if(dev_handle == NULL) {
        return true;
    }
    for(ULONG i = 0; i < delay_time; i++){
        bytes_written = usb_bulk_write(dev_handle, out_pipe, reinterpret_cast<char*>(pBuffer),
                                      bufferSize, USBTIMEOUT);
        if(bytes_written < 0) {
            bytes_written = 0;
            return false;
        }
        else {
            break;
        }
        if(i == 5) {
            this->libutils->u_log(LOG_USB, "USB I/O ERROR RETRYING R/W\n");
            return false;
        }
        srand((unsigned)time(0));
        int random_integer;
        int lowest=1, highest=10;
        int range=(highest-lowest)+1;
        random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
#if defined XFSTK_OS_WIN
        Sleep(random_integer);
#else
        usleep(random_integer*10);
#endif
    }
    return RetVal;
}

bool CloverviewPlusUSB20Device::Read(void *szBuff, unsigned int bufferSize)
{
    bool RetVal = true;
    int bytes_rxed = 0;
    ULONG delay_time = 2000;//USBTIMEOUT;
    if(dev_handle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        bytes_rxed = usb_bulk_read(dev_handle, in_pipe, reinterpret_cast<char*>(szBuff),
                                  bufferSize, USBTIMEOUT);
        if ((bytes_rxed) < 0) {
           this->libutils->u_log(LOG_USB, "usb_bulk_read() fails");
           RetVal = false;
        } else {
            break;
        }
#if 0
        if (this->IsCorrectProtocol(szBuff, bytes_rxed)) {
            break;
        }
#endif
        if(i == 5) {
            this->libutils->u_log(LOG_USB, "USB I/O ERROR RETRYING R/W\n");
            return false;
        }
        srand((unsigned)time(0));
        int random_integer;
        int lowest=1, highest=10;
        int range=(highest-lowest)+1;
        random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
#if defined XFSTK_OS_WIN
        Sleep(random_integer);
#else
        usleep(random_integer*10);
#endif
    }
    this->libutils->u_log(LOG_USB, "%s - %s(%d)", __FUNCTION__, szBuff, bytes_rxed);
    return RetVal;
}

bool CloverviewPlusUSB20Device::GetAck(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = true;
    ULONG delay_time = 2000;//USBTIMEOUT;
    if(dev_handle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        *bytes_rxed = usb_bulk_read(dev_handle, in_pipe, reinterpret_cast<char*>(szBuff),
                                  512, USBTIMEOUT);
        if ((*bytes_rxed) <= 0) {
           this->libutils->u_log(LOG_USB, "usb_bulk_read() fails");
           RetVal = false;
        }
        if (this->IsCorrectProtocol(szBuff, bytes_rxed)) {
            break;
        }
        if(i == 5) {
            this->libutils->u_log(LOG_USB, "USB I/O ERROR RETRYING R/W\n");
            return false;
        }
        srand((unsigned)time(0));
        int random_integer;
        int lowest=1, highest=10;
        int range=(highest-lowest)+1;
        random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
#if defined XFSTK_OS_WIN
        Sleep(random_integer);
#else
        usleep(random_integer*10);
#endif
    }
    this->libutils->u_log(LOG_USB, "%s - %s(%d)", __FUNCTION__, szBuff, *bytes_rxed);
    return RetVal;
}

int CloverviewPlusUSB20Device::Detect()
{
    return 0;
}
bool CloverviewPlusUSB20Device::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = false;
    if(!this->libutils) {
        return RetVal;
    }
    else {
        if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RUPHS", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RSUPHS", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RSUPH", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "PSFW1", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "PSFW2", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "SSFW", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "SSBS", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RESET", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "VEDFW", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "OSIP Sz", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ROSIP", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RIMG", 0, 1) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "EOIU", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "MFLD", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "CLVT", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(*bytes_rxed == 4){
            RetVal = true;
        }
    }
    return RetVal;
}

bool CloverviewPlusUSB20Device::IsSupportedDevice(unsigned short vendorid, unsigned short productid)
{
    if (vendorid == this->id_vendor &&
        productid == this->id_product) {
        return true;
    } else {
        return false;
    }
}

void CloverviewPlusUSB20Device::GetUsbsn(char * usbsn)
{
    //this->libutils->u_log( LOG_USB, "USBSN: %s", this->usbsn);
    memcpy(usbsn, this->usbsn, 17);
}
#endif
//////////////////////////////////////////
CloverviewPlusSerialDevice::CloverviewPlusSerialDevice()
{
}

int CloverviewPlusSerialDevice::GetNumberAvailableDevices()
{
    return 0;
}

int CloverviewPlusSerialDevice::GetNumberAvailableDevices(DeviceInfos* )
{
    int RetVal = 0;
    return RetVal;
}

bool CloverviewPlusSerialDevice::SetUtilityInstance(void *UtilityInstance)
{
    if(UtilityInstance) {
        return true;
    }
    return false;
}

void* CloverviewPlusSerialDevice::GetDeviceHandle(int )
{
    return NULL;
}

bool CloverviewPlusSerialDevice::SetDeviceHandle(void *DeviceHandle)
{
    bool RetVal = false;
    if(DeviceHandle != NULL) {
        this->CurrentDeviceHandle = DeviceHandle;
        RetVal = true;
    }
    return RetVal;
}

bool CloverviewPlusSerialDevice::Open()
{
    bool RetVal = false;
    return RetVal;
}

bool CloverviewPlusSerialDevice::Init()
{
    bool RetVal = false;
    return RetVal;
}

bool CloverviewPlusSerialDevice::Abort()
{
    return true;
}

bool CloverviewPlusSerialDevice::Write(void *, unsigned int )
{
    bool RetVal = false;
    return RetVal;
}

bool CloverviewPlusSerialDevice::Read(void *, unsigned int )
{
    bool RetVal = false;
    return RetVal;
}

bool CloverviewPlusSerialDevice::GetAck(void *, unsigned int *)
{
    bool RetVal = false;
    return RetVal;
}

int CloverviewPlusSerialDevice::Detect()
{
    return 0;
}
bool CloverviewPlusSerialDevice::IsCorrectProtocol(void *, unsigned int *)
{
    bool RetVal = false;
    return RetVal;
}

void CloverviewPlusSerialDevice::GetUsbsn(char * )
{

}

bool CloverviewPlusSerialDevice::IsSupportedDevice(unsigned short , unsigned short )
{
    return true;
}

/////////////////////////////////////
