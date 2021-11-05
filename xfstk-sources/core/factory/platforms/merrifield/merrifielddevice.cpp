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
#include "merrifielddevice.h"
#include "../../common/xfstktypes.h"
#include "../../common/xfstkcommon.h"

extern IntelSoCGlobalDeviceState IntelSoCGlobalDeviceCurrentState;
MerrifieldDevice::MerrifieldDevice(unsigned long device)
{
#ifdef XFSTK_OS_WIN
    deviceFactory = NULL;
#endif
    if(!device)
        this->_device = MERRIFIELD_DEVICE_ID;
    else
        this->_device = device;
    this->PhysicalTransportDevice = NULL;
}
MerrifieldDevice::~MerrifieldDevice()
{
    if(this->PhysicalTransportDevice != NULL)
    {
        delete this->PhysicalTransportDevice;
        this->PhysicalTransportDevice = NULL;
    }

#ifdef XFSTK_OS_WIN
    if(deviceFactory)
    {
        delete deviceFactory;
        deviceFactory = NULL;
    }
#endif
}
int MerrifieldDevice::GetNumberAvailableDevices()
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->GetNumberAvailableDevices();
    return RetVal;
}

int MerrifieldDevice::GetNumberAvailableDevices(DeviceInfos* socdevices)
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->GetNumberAvailableDevices(socdevices);
    return RetVal;
}

bool MerrifieldDevice::SetUtilityInstance(void *UtilityInstance)
{
    int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->SetUtilityInstance(UtilityInstance);
    return RetVal;
}

void* MerrifieldDevice::GetDeviceHandle(int DeviceNumber)
{
    void *RetVal = NULL;
    RetVal = this->PhysicalTransportDevice->GetDeviceHandle(DeviceNumber);
    return RetVal;
}
bool MerrifieldDevice::SetDeviceHandle(void *DeviceHandle)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->SetDeviceHandle(DeviceHandle);
    return RetVal;
}
bool MerrifieldDevice::SetTransport(DeviceTransportType Transport)
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
            this->PhysicalTransportDevice = (IDevice *) deviceFactory->CreateDevice(this->_device);
            if(this->PhysicalTransportDevice) {
                RetVal = true;
            }
        };
        break;
#else
        this->PhysicalTransportDevice = (IDevice *) new MerrifieldUSB20Device(this->_device);
        if(this->PhysicalTransportDevice) {
            RetVal = true;
        }
        break;
#endif
    case XFSTK_SERIAL:
        this->PhysicalTransportDevice = (IDevice *) new MerrifieldSerialDevice();
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

#if 0
        this->PhysicalTransportDevice = (IDevice *) new MerrifieldUSB30Device(this->_device);
        if(this->PhysicalTransportDevice) {
            RetVal = true;
        }
        break;
#else
        RetVal = false;
        break;
#endif
    case XFSTK_NOT_SPECIFIED:
        RetVal = false;
        break;
    default:
        RetVal = false;
        break;
    }
    return RetVal;
}

bool MerrifieldDevice::Open()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Open();
    return RetVal;
}

bool MerrifieldDevice::Init()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Init();
    return RetVal;
}

bool MerrifieldDevice::Abort()
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Abort();
    return RetVal;
}

bool MerrifieldDevice::Write(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Write(pBuffer, bufferSize);
    return RetVal;
}

bool MerrifieldDevice::Read(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->Read(pBuffer, bufferSize);
    return RetVal;
}

bool MerrifieldDevice::GetAck(void *pAck, unsigned int *ackSize)
{
    bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->GetAck(pAck, ackSize);
    return RetVal;
}

int MerrifieldDevice::Detect()
{   int RetVal = 0;
    RetVal = this->PhysicalTransportDevice->Detect();
    return RetVal;
}

bool MerrifieldDevice::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{   bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->IsCorrectProtocol(szBuff, bytes_rxed);
    return RetVal;
}

bool MerrifieldDevice::IsSupportedDevice(unsigned short vendorid, unsigned short productid)
{   bool RetVal = false;
    RetVal = this->PhysicalTransportDevice->IsSupportedDevice(vendorid, productid);
    return RetVal;
}

void MerrifieldDevice::GetUsbsn(char * usbsn)
{
    this->PhysicalTransportDevice->GetUsbsn(usbsn);
}

#ifdef XFSTK_OS_WIN

#else

//////////////////////////////////////
MerrifieldUSB20Device::MerrifieldUSB20Device(unsigned long device)
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
    USBTIMEOUT = 5000;
    if(device == MOOREFIELD_DEVICE_ID)
        this->id_product = MOOREFIELD_PRODUCT_ID;
    else if(device == MERRIFIELD_DEVICE_ID)
        this->id_product = MERRIFIELD_PRODUCT_ID;
    else
        this->id_product = CARBONCANYON_PRODUCT_ID;
    this->id_vendor = INTEL_VENDOR_ID;
    if(!IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized) {
        usb_init();
        IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized = true;
        if(IntelSoCGlobalDeviceCurrentState.IsUsb20DebugEnabled) {
            usb_set_debug(255);
        }
    }
}
MerrifieldUSB20Device::~MerrifieldUSB20Device()
{
    if(dev_handle) {
        usb_close(dev_handle);
        dev_handle = NULL;
    }
}

int MerrifieldUSB20Device::GetNumberAvailableDevices()
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

int MerrifieldUSB20Device::GetNumberAvailableDevices(DeviceInfos* socdevices)
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

void* MerrifieldUSB20Device::GetDeviceHandle(int DeviceNumber)
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

bool MerrifieldUSB20Device::SetDeviceHandle(void *DeviceHandle)
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
        memset(this->usbsn, 0, sizeof(this->usbsn));
        ret = usb_get_string_simple(dev_handle, ((struct usb_device *)DeviceHandle)->descriptor.iSerialNumber, this->usbsn,128);
        usb_close(dev_handle);
        dev_handle = NULL;
        RetVal = true;
    }
    return RetVal;
}
bool MerrifieldUSB20Device::VerifyDeviceHandle(struct usb_device* Handle)
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
                    memset(myusbsn, 0, sizeof(myusbsn));
                    usb_get_string_simple(dev_handle, dev_port->descriptor.iSerialNumber, myusbsn,128);
                    if(strcpy(myusbsn, this->usbsn)) {
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
void MerrifieldUSB20Device::FindDevice(void)
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
        memset(this->usbsn, 0, sizeof(this->usbsn));
        usb_get_string_simple(dev_handle, dev_port->descriptor.iSerialNumber, this->usbsn,128);
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

bool MerrifieldUSB20Device::Open()
{
    bool RetVal = true;
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    this->Init();

    FindDevice();
    if(dev_handle == NULL)
        RetVal = false;
    return RetVal;
}

bool MerrifieldUSB20Device::Init()
{
    bool RetVal = true;
    this->dev_handle = NULL;
    this->in_pipe    = 0;
    this->out_pipe   = 0;
    this->numinits = 0;
    this->reqnuminits = 1;
    return RetVal;
}
bool MerrifieldUSB20Device::SetUtilityInstance(void *UtilityInstance)
{
    if(UtilityInstance) {
        this->libutils = (MerrifieldUtils *) UtilityInstance;
        return true;
    }
    return false;
}

bool MerrifieldUSB20Device::Abort()
{
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    if(dev_handle){
        usb_close(dev_handle);
        dev_handle = NULL;
    }
    return true;
}

bool MerrifieldUSB20Device::Write(void *pBuffer, unsigned int bufferSize)
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

bool MerrifieldUSB20Device::Read(void *szBuff, unsigned int bufferSize)
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

bool MerrifieldUSB20Device::GetAck(void *szBuff, unsigned int *bytes_rxed)
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

int MerrifieldUSB20Device::Detect()
{
    return 0;
}
bool MerrifieldUSB20Device::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = false;
    if(!this->libutils) {
        return RetVal;
    }
    else {
        if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RUPHS", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DIFWI", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DCFI00", 0, 0) != -1){
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
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DCSDB", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "UCSDB", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(*bytes_rxed == 4){
            RetVal = true;
        }
    }
    return RetVal;
}

bool MerrifieldUSB20Device::IsSupportedDevice(unsigned short vendorid, unsigned short productid)
{
    if (vendorid == this->id_vendor &&
        productid == this->id_product) {
        return true;
    } else {
        return false;
    }
}

void MerrifieldUSB20Device::GetUsbsn(char * usbsn)
{
    strcpy(usbsn, this->usbsn);
}
//////////////////////////////////////////
#endif

#if 0
//////////////////////////////////////
MerrifieldUSB30Device::MerrifieldUSB30Device(unsigned long device)
{
    this->dev = NULL;
    this->dev_handle = NULL;
    this->target = 0;
    this->numinits = 0;
    this->reqnuminits = 1;
    this->libutils = NULL;
    USBTIMEOUT = 5000;
    if(device == MOOREFIELD_DEVICE_ID)
        this->id_product = MOOREFIELD_PRODUCT_ID;
    else if(device == MERRIFIELD_DEVICE_ID)
        this->id_product = MERRIFIELD_PRODUCT_ID;
    else
        this->id_product = CARBONCANYON_PRODUCT_ID;
    this->id_vendor = INTEL_VENDOR_ID;
    if(!IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized) {
        usb_init();
        IntelSoCGlobalDeviceCurrentState.IsUsb20Initialized = true;
        if(IntelSoCGlobalDeviceCurrentState.IsUsb20DebugEnabled) {
            usb_set_debug(255);
        }
    }
}
MerrifieldUSB30Device::~MerrifieldUSB30Device()
{
    if(dev_handle) {
        libusb_close(dev_handle);
        dev_handle = NULL;
    }
    libusb_exit(this->context);
}

int MerrifieldUSB30Device::GetNumberAvailableDevices()
{
    struct libusb_device **dev = NULL;
    struct usb_bus *bus = NULL;
    ssize_t numTotaldev = 0;
    int availnumdevs = 0;
    libusb_init(&this->context);
    numTotaldev = libusb_get_device_list(this->context,&dev);

    libusb_device_descriptor descript;
    for(ssize_t i=0;i<numTotaldev;i++)
    {
        if(!libusb_get_device_descriptor(dev[i],&descript))
        {
            if(this->IsSupportedDevice(descript.idVendor,descript.idProduct))
                availnumdevs++;
        }
    }

    libusb_free_device_list(dev,1);

    return availnumdevs;
}


int MerrifieldUSB30Device::GetNumberAvailableDevices(DeviceInfos* socdevices)
{
    int RetVal = 0;
    return RetVal;
}

void* MerrifieldUSB30Device::GetDeviceHandle(int DeviceNumber)
{

    struct libusb_device **dev = NULL;
    struct usb_bus *bus = NULL;
    libusb_device_handle* retVal = NULL;
    ssize_t numTotaldev = 0;
    int availnumdevs = 0;
    int error = 0;
    libusb_init(&this->context);
    numTotaldev = libusb_get_device_list(this->context,&dev);

    for(ssize_t i=0;i<numTotaldev;i++)
    {
        libusb_device_descriptor descript;
        if(!(error =libusb_get_device_descriptor(dev[i],&descript)))
        {
            if(this->IsSupportedDevice(descript.idVendor,descript.idProduct))
            {
                if(availnumdevs == DeviceNumber)
                {
                    if(error = libusb_open(dev[i],&retVal))

#ifdef UBUNTU10
                        printf("%s",libusb_error_name(error)) ;
#else
                        ;
#endif
                }
                else
                {
                    availnumdevs++;
                }
            }
        }else
        {
            #ifdef UBUNTU10
                                    printf("%s",libusb_error_name(error)) ;
            #else
                                    ;
            #endif
        }
    }

    libusb_free_device_list(dev,1);

    return reinterpret_cast<void*>(retVal);
}

bool MerrifieldUSB30Device::SetDeviceHandle(void *DeviceHandle)
{
    bool RetVal = false;
    int ret = 0;
    unsigned char* tmpSer = new unsigned char[USBSN_BUFFER_SIZE];

    struct libusb_device* devs = NULL;
    libusb_device_descriptor descript;

    if(DeviceHandle != NULL) {
        this->CurrentDeviceHandle = DeviceHandle;

        devs = libusb_get_device(reinterpret_cast<libusb_device_handle*>(DeviceHandle));
 #ifdef UBUNTU10
        int usbSpeedCheck = libusb_get_device_speed(devs);

        if(usbSpeedCheck != LIBUSB_SPEED_SUPER)
            printf("\nUSB 3.0 Not Enabled!!!!\n");
        else
            printf("\nUSB 3.0 Enabled!!!!\n");
#endif
        if(devs == NULL) {
            return false;
        }
        libusb_open(devs,reinterpret_cast<libusb_device_handle**>(&DeviceHandle));

        libusb_get_device_descriptor(devs,&descript);
        memset(tmpSer, 0, USBSN_BUFFER_SIZE);
        libusb_get_string_descriptor_ascii(reinterpret_cast<libusb_device_handle*>(DeviceHandle),descript.iSerialNumber,tmpSer,USBSN_BUFFER_SIZE);
        memcpy(this->usbsn,tmpSer,USBSN_BUFFER_SIZE);
        RetVal = true;
    }
    return RetVal;
}
bool MerrifieldUSB30Device::VerifyDeviceHandle(struct usb_device* Handle)
{
    return false;
}
void MerrifieldUSB30Device::FindDevice(void)
{

    int targetsfound = 0;
    bool targetsearchcomplete = false;
    bool found = false;




    struct libusb_device **dev = NULL;
    libusb_device_handle* retVal = NULL;
    libusb_device_descriptor descript;
    unsigned char* tmpSer = new unsigned char[USBSN_BUFFER_SIZE];
    ssize_t numTotaldev = 0;
    int availnumdevs = 0;
    int error = 0;
    libusb_init(&this->context);
    numTotaldev = libusb_get_device_list(this->context,&dev);

    if(this->CurrentDeviceHandle != NULL) {
       targetsearchcomplete = true;
       this->dev = libusb_get_device(reinterpret_cast<libusb_device_handle*>(this->CurrentDeviceHandle));
       this->libutils->u_log(LOG_USB, "USB Device found - Externally Specified for MT");
       this->libutils->u_log(LOG_USB, "USB Device Handle - %x", this->CurrentDeviceHandle);
       found = true;
    }
    else {

        for(ssize_t i=0;i<numTotaldev;i++)
        {
            libusb_device_descriptor descript;
            if(!(error =libusb_get_device_descriptor(dev[i],&descript)))
            {
                if(this->IsSupportedDevice(descript.idVendor,descript.idProduct))
                {
#if defined XFSTK_MULTIDEVICE
                   if(targetsfound != this->target) {
                       targetsfound++;
                       continue;
                   }
                   this->dev = dev[i];
                   targetsearchcomplete = true;
#else
                   this->dev_port = bus->devices;
#endif
                    #ifdef UBUNTU10
                                            printf("%s",libusb_error_name(error)) ;
                    #else
                                            ;
                    #endif
                   found = true;
                }
            }else
            {
                #ifdef UBUNTU10
                                        printf("%s",libusb_error_name(error)) ;
                #else
                                        ;
                #endif
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
        error = libusb_open(this->dev,&this->dev_handle);
        this->libutils->u_log( LOG_USB,"USB dev_handle -- %x", dev_handle);
        if(dev_handle == NULL) {
            #ifdef UBUNTU10
                                    printf("%s",libusb_error_name(error)) ;
            #else
                                    ;
            #endif
            return;
        }

        error = libusb_get_device_descriptor(this->dev,&descript);
        if(error)
        {
            #ifdef UBUNTU10
                                    printf("%s",libusb_error_name(error)) ;
            #else
                                    ;
            #endif
        }
        else
        {
            memset(tmpSer, 0, USBSN_BUFFER_SIZE);
            libusb_get_string_descriptor_ascii(dev_handle,descript.iSerialNumber,tmpSer,USBSN_BUFFER_SIZE);
            memcpy(this->usbsn,tmpSer,USBSN_BUFFER_SIZE);
        }

        int failed_claim = 0;
        int failed_device = 0;
        while(1)
        {


            unsigned char tmpDesc[MAX_BUFFER_SIZE];
            libusb_config_descriptor* config;
            error = libusb_get_active_config_descriptor(this->dev,&config);

            if(error)
            {
                #ifdef UBUNTU10
                                        printf("%s",libusb_error_name(error)) ;
                #else
                                        ;
                #endif
            }
            else
            {

                for(int i= 0; i<config->interface->altsetting->bNumEndpoints;i++)
                {
                    if(config->interface->altsetting->endpoint[i].bEndpointAddress & LIBUSB_ENDPOINT_IN)
                        this->in_pipe = config->interface->altsetting->endpoint[i].bEndpointAddress/* & LIBUSB_ENDPOINT_ADDRESS_MASK*/;
                    else
                        this->out_pipe = config->interface->altsetting->endpoint[i].bEndpointAddress /*& LIBUSB_ENDPOINT_ADDRESS_MASK*/;
                }

            }


            if(!(libusb_claim_interface(dev_handle,interfaceDescript.bInterfaceNumber))) {
                break;
            }
            else
            {
                failed_claim++;

                if(failed_claim > 10) {
                    libusb_close(dev_handle);
                    dev_handle = NULL;
                    this->libutils->u_abort("LOST DEVICE");
                    break;
                }
            }

        }
    }
        return;
}

bool MerrifieldUSB30Device::Open()
{
    bool RetVal = true;
    int opensattempted = 0;
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    this->Init();

    FindDevice();

    if(dev_handle == NULL)
        RetVal = false;
    return RetVal;
}

bool MerrifieldUSB30Device::Init()
{
    bool RetVal = true;
    this->dev_handle = NULL;
    this->in_pipe    = 0;
    this->out_pipe   = 0;
    this->numinits = 0;
    this->reqnuminits = 1;
    return RetVal;
}
bool MerrifieldUSB30Device::SetUtilityInstance(void *UtilityInstance)
{
    if(UtilityInstance) {
        this->libutils = (MerrifieldUtils *) UtilityInstance;
        return true;
    }
    return false;
}

bool MerrifieldUSB30Device::Abort()
{
    this->libutils->u_log(LOG_USB, "%s", __PRETTY_FUNCTION__);
    // After OS download is done if call following, it will seg fault
    // May be due to the device is reset too soon?
    // So for the moment don't call this one.
#if 0
    usb_release_interface(dev_handle, dev_port->config->interface->altsetting->bInterfaceNumber);
#endif
    if(dev_handle){
        libusb_close(dev_handle);
        dev_handle = NULL;
    }
    this->CurrentDeviceHandle = NULL;
    return true;
}

bool MerrifieldUSB30Device::Write(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = true;
    this->libutils->u_log(LOG_USB, "%s --->%s", __PRETTY_FUNCTION__, this->libutils->makestring((PUCHAR)pBuffer, bufferSize).c_str());
    int bytes_written = 0;
    int error  = 0;
    ULONG delay_time = 2000;    //USBTIMEOUT;
    if(dev_handle == NULL) {
        return true;
    }
    for(ULONG i = 0; i < delay_time; i++){
        error = libusb_bulk_transfer(dev_handle, out_pipe, reinterpret_cast<unsigned char*>(pBuffer),
                                      bufferSize, &bytes_written,USBTIMEOUT);
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

bool MerrifieldUSB30Device::Read(void *szBuff, unsigned int bufferSize)
{
    bool RetVal = true;
    int bytes_rxed = 0;
    ULONG delay_time = 2000;//USBTIMEOUT;
    int error = 0;
    if(dev_handle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        error = libusb_bulk_transfer(dev_handle, in_pipe, reinterpret_cast<unsigned char*>(szBuff),
                                      bufferSize, &bytes_rxed, USBTIMEOUT);
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

bool MerrifieldUSB30Device::GetAck(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = true;
    ULONG delay_time = 2000;//USBTIMEOUT;
    int error = 0;
    if(dev_handle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        error = libusb_bulk_transfer(dev_handle, in_pipe, reinterpret_cast<unsigned char*>(szBuff),
                                     512, reinterpret_cast<int*>(bytes_rxed), USBTIMEOUT);
        if ((*bytes_rxed) < 0) {
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

int MerrifieldUSB30Device::Detect()
{
    bool RetVal = false;
    return 0;
}
bool MerrifieldUSB30Device::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = false;
    if(!this->libutils) {
        return RetVal;
    }
    else {
        if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RUPHS", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DIFWI", 1, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DCFI00", 0, 0) != -1){
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
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DCSDB", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "UCSDB", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(*bytes_rxed == 4){
            RetVal = true;
        }
    }
    return RetVal;
}

bool MerrifieldUSB30Device::IsSupportedDevice(unsigned short vendorid, unsigned short productid)
{
    if (vendorid == this->id_vendor &&
        productid == this->id_product) {
        return true;
    } else {
        return false;
    }
}

void MerrifieldUSB30Device::GetUsbsn(char * usbsn)
{
    strcpy(usbsn, this->usbsn);
}
//////////////////////////////////////////

#endif
MerrifieldSerialDevice::MerrifieldSerialDevice()
{
    this->CurrentDeviceHandle = NULL;
    this->serialPort = NULL;
}

int MerrifieldSerialDevice::GetNumberAvailableDevices()
{
#if 0
    bool RetVal = false;
    char sotxinbuffer[2048] = "";
    unsigned int bytes_rxed;
    baudrate = 115200;
    for(int i = 6; i < 16; i++) {
        HANDLE ret = this->OpenCom(i, baudrate);
        if(ret != NULL) {
            SetDeviceHandle(ret);
#if 1
            //Check for SoTx
            if(this->GetAck(sotxinbuffer, &bytes_rxed)){
                RetVal = true;
                break;
            }

#else
            RetVal = true;
#endif
        }
    }

    if(RetVal)
        this->CloseCom(this->GetDeviceHandle(1));

    return RetVal ? 1:0;
#else
    return 1;
#endif
}
bool MerrifieldSerialDevice::SetUtilityInstance(void *UtilityInstance)
{
    if(UtilityInstance) {
        this->libutils = (MerrifieldUtils *) UtilityInstance;
        return true;
    }
    return false;
}

void* MerrifieldSerialDevice::GetDeviceHandle(int )
{
    return this->CurrentDeviceHandle;
}

bool MerrifieldSerialDevice::SetDeviceHandle(void* DeviceHandle)
{
    bool RetVal = false;
    if(DeviceHandle != NULL) {
        this->CurrentDeviceHandle = DeviceHandle;
        RetVal = true;
    }
    return RetVal;
}

bool MerrifieldSerialDevice::Open()
{
     this->libutils->u_log(LOG_ENTRY, "%s", __PRETTY_FUNCTION__);
     bool RetVal = false;
     char idrqoutbuffer[16] = "IDRQ";
     char idrqinbuffer[513] = "";
     unsigned int bytes_rxed;
     baudrate = 115200;
     HANDLE ret;
     if(this->serialPort == 0)
     {
         int* tmp = (int*)this->GetDeviceHandle(0);
         if(tmp!=NULL)
            this->serialPort = *tmp;
         else
             return false;
     }


#ifndef XFSTK_OS_WIN
     for(int i = 0; i < 32; i++) {
              ret = this->OpenCom(serialPort, baudrate);
             if(ret != NULL) {
                 SetDeviceHandle(ret);

                 //Check for SoTx, some issue to get SoTx, disable it for now
                 while(!this->GetAck(idrqinbuffer, &bytes_rxed))//{
                 {
                     this->libutils->u_log(LOG_USB, "%s - Waiting for device to issue SoTx", __FUNCTION__ );
                     sleep(2);
                 }
                     RetVal = true;

                     //Then check IDRQ
                     if(this->Write(idrqoutbuffer, 4)) {
                       while(!this->GetAck(idrqinbuffer, &bytes_rxed));

                         sleep(1);
                         //Check for "MRFD"
                         if(this->GetAck(idrqinbuffer, &bytes_rxed)){
                             return true;
                         }
                     }
                 }
         }
         return RetVal;


#else

     for(int i = 1; i < 16; i++) {
         ret = this->OpenCom(serialPort, baudrate);
                if(ret != NULL) {
                    SetDeviceHandle(ret);

                        //Then check IDRQ
                        if(this->Write(idrqoutbuffer, 4)) {

                            Sleep(500);

                            //Check for "MRFD"
                            if(this->GetAck(idrqinbuffer, &bytes_rxed)){
                                RetVal = true;
                            }
                        }
                }

				CloseHandle(ret);
            }
            return RetVal;

#endif


}

bool MerrifieldSerialDevice::Init()
{
    bool RetVal = true;
    return RetVal;
}

bool MerrifieldSerialDevice::Abort()
{
    return true;
}

bool MerrifieldSerialDevice::Write(void *pBuffer, unsigned int bufferSize)
{
    bool RetVal = true;
    this->libutils->u_log(LOG_USB, "%s --->%s", __PRETTY_FUNCTION__, this->libutils->makestring((PUCHAR)pBuffer, bufferSize).c_str());
    uint32 bytes_written = 0;
    ULONG delay_time = 20000;    //USBTIMEOUT;
    if(this->CurrentDeviceHandle == NULL) {
        return true;
    }
    for(ULONG i = 0; i < delay_time; i++){
        bytes_written = this->WriteCom(reinterpret_cast<unsigned char*>(pBuffer), bufferSize,this->GetDeviceHandle(1));

        if(bytes_written < bufferSize) {
            bytes_written = 0;
            return false;
        }
        else {
            break;
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

bool MerrifieldSerialDevice::Read(void *szBuff, unsigned int bufferSize)
{
    bool RetVal = true;
    int bytes_rxed = 0;
    ULONG delay_time = 20000;//USBTIMEOUT;
    if(this->CurrentDeviceHandle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        bytes_rxed = this->ReadCom(reinterpret_cast<unsigned char*>(szBuff), bufferSize, this->GetDeviceHandle(1));
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

bool MerrifieldSerialDevice::GetAck(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = true;
    ULONG delay_time = 20000;//USBTIMEOUT;
    if(this->CurrentDeviceHandle == NULL) {
        return false;
    }
    for(ULONG i = 0; i < delay_time; i++){
        *bytes_rxed = this->ReadCom(reinterpret_cast<unsigned char*>(szBuff), 512,this->GetDeviceHandle(1));
        if ((*bytes_rxed) <= 0) {
           this->libutils->u_log(LOG_USB, "usb_bulk_read() fails");
           RetVal = false;
        }

        if (this->IsCorrectProtocol(szBuff, bytes_rxed)) {
            break;
        } else {
            this->libutils->u_log(LOG_USB, "SCUDEBUG -- %s", reinterpret_cast<unsigned char*>(szBuff));
            RetVal = false;
        }
        if(i == 10) {
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

int MerrifieldSerialDevice::Detect()
{
    return 0;
}

bool MerrifieldSerialDevice::IsCorrectProtocol(void *szBuff, unsigned int *bytes_rxed)
{
    bool RetVal = false;
    if(!this->libutils) {
        return RetVal;
    }
    else {
        if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RUPHS", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DIFWI", 1, 0) != -1){
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
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DFRM", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DxxM", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DXBL", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RTBD", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "RUPH", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "HLT$", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "HLT0", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "DCFI00", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "MRFD", 0, 0, 1) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER00", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER01", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER02", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER03", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER04", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER10", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER11", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER12", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER13", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER20", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER21", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER22", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "ER25", 0, 0) != -1){
            RetVal = true;
        }
        else if(this->libutils->strstr_lowercase_strip((char *)szBuff, "SoTx", 0, 0) != -1){
            RetVal = true;
        }
        else if(*bytes_rxed == 4){
            RetVal = true;
        }
    }
    return RetVal;
}

bool MerrifieldSerialDevice::IsSupportedDevice(unsigned short , unsigned short )
{
    return true;
}
void MerrifieldSerialDevice::GetUsbsn(char * )
{

}

/////////////////////////////////////
#ifndef XFSTK_OS_WIN




HANDLE MerrifieldSerialDevice::OpenCom(int port, int baud)
{

    std::string portStr = "";
    std::stringstream tempStr;
    std::map<int,int> baudR;
    baudR[110]    = B110;
    baudR[300]    = B300;
    baudR[600]    = B600;
    baudR[1200]   = B1200;
    baudR[2400]   = B2400;
    baudR[4800]   = B4800;
    baudR[9600]   = B9600;
    baudR[19200]  = B19200;
    baudR[38400]  = B38400;
    baudR[57600]  = B57600;
    baudR[115200] = B115200;



    tempStr <<"/dev/ttyS";
    tempStr << port;
    portStr = tempStr.str();

    this->comHandle = open(portStr.c_str(), O_RDWR | O_NOCTTY );

    if(this->comHandle == -1)
    {
        return NULL;
    }

    tcgetattr(this->comHandle,&oldComSetting);

    memset(&newComSetting,0,sizeof(newComSetting));

    newComSetting.c_cc[VTIME] =  5;
    newComSetting.c_iflag=IGNPAR;
    newComSetting.c_cflag = baudR[baud]|CS8|CREAD;
    newComSetting.c_lflag=0;

    tcflush(this->comHandle,TCIOFLUSH);
    tcsetattr(this->comHandle,TCSANOW,&newComSetting);
    return (void*)this->comHandle;

}

void MerrifieldSerialDevice::CloseCom(HANDLE )
{

    tcsetattr(this->comHandle,TCSANOW,&oldComSetting);
    close(this->comHandle);
}

int MerrifieldSerialDevice::ReadCom(unsigned char* strBf, int size, HANDLE )
{
    return read(this->comHandle,strBf,size);

}

int MerrifieldSerialDevice::WriteCom(unsigned char* strBf, int size, HANDLE )
{
    return write(this->comHandle,strBf,size);

}

#else
std::string bRates = "110 300 600 1200 2400 4800 9600 19200 38400 57600 115200 128000 256000";

HANDLE MerrifieldSerialDevice::OpenCom(int port, int baud)
{
    DCB	comCTRLSettings;
    COMMTIMEOUTS comTimeout;
    std::string settingStr, portStr = "";
    HANDLE comHandle;


    std::stringstream temp;
    temp << baud;
    std::string sBaudRate = temp.str();

    if(bRates.find(sBaudRate,0)!= std::string::npos)
    {
        settingStr = "baud="+sBaudRate+" parity=N data=8  stop=1";
    }else
    {
        //error finding valid baud
        return(NULL);
    }



    temp.str("");
    temp << "\\\\.\\COM";
    temp << port;
    portStr = temp.str();


    comHandle = CreateFileA(portStr.c_str(),
                        GENERIC_READ|GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

    if(comHandle == INVALID_HANDLE_VALUE)
    {
        //error creating the
        return(NULL);
    }

    memset(&comCTRLSettings, 0, sizeof(comCTRLSettings));  /* clear the new struct  */
    comCTRLSettings.DCBlength = sizeof(comCTRLSettings);

    if(!BuildCommDCBA(settingStr.c_str(),(LPDCB)&comCTRLSettings))
    {
      printf("unable to set comport dcb settings\n");
      CloseHandle(comHandle);
      return (NULL);
    }

    if(!SetCommState(comHandle,(LPDCB)&comCTRLSettings))
    {
      printf("unable to set comport cfg settings\n");
      CloseHandle(comHandle);
      return(NULL);
    }

    comTimeout.ReadIntervalTimeout = 300;
    comTimeout.ReadTotalTimeoutConstant = 50;
    comTimeout.ReadTotalTimeoutMultiplier= 300;
    comTimeout.WriteTotalTimeoutConstant=50;
    comTimeout.WriteTotalTimeoutMultiplier=300;

    if(!SetCommTimeouts(comHandle,&comTimeout))
    {
      printf("unable to set comport time-out settings\n");
      CloseHandle(comHandle);
      return(NULL);
    }

    return comHandle;


}
void MerrifieldSerialDevice::CloseCom(HANDLE handle)
{
    CloseHandle(handle);
}



int MerrifieldSerialDevice::ReadCom(unsigned char* strBf, int size, HANDLE handle)
{
    int bytesRead=0;
    LPVOID tester = (LPVOID)strBf;
    ReadFile(handle,tester,(DWORD)size,(LPDWORD)((void *)&bytesRead),NULL);

    return bytesRead;

}
int MerrifieldSerialDevice::WriteCom(unsigned char* strBf, int size, HANDLE handle)
{

    int bytesRead=0;
    LPCVOID tester = (LPCVOID)strBf;

    WriteFile(handle,tester,(DWORD)size,(LPDWORD)((void *)&bytesRead),NULL);

    return bytesRead;

}


#endif
