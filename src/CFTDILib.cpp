/*
Copyright (c) 2016 Intrepid Control Systems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdlib.h>
#include "CFTDILib.h"

CFTDILibLinux::CFTDILibLinux() 
{
	m_bDeviceOpen = false;
}


CFTDILibLinux::~CFTDILibLinux() 
{
	if(m_bDeviceOpen)
	{
#ifndef NO_FTDI
		//close it!
		ftdi_usb_close(&m_ftdic);
		ftdi_deinit(&m_ftdic);		
#endif
	}
}


int CFTDILibLinux::GetPID(unsigned long DevType)
{
	int PID;
	
	switch(DevType)
	{
		case NEODEVICE_VCAN3:
			PID = 0x0601;
			break;
	
		case NEODEVICE_FIRE:
			PID = 0x0701;
			break;
			
		case NEODEVICE_PLASMA_1_11:
		case NEODEVICE_PLASMA_1_12: 
		case NEODEVICE_PLASMA_1_13:
			PID = 0x0801;
			break;
			
		case NEODEVICE_ION_2:
		case NEODEVICE_ION_3:
			PID = 0x0901;
			break;
			
		case NEODEVICE_FIRE2:
			PID = 0x1000;
			break;
			
		case NEODEVICE_VCANRF:
		
			PID = 0x0601;

			break;
		
		default:
			PID = 0;
			break;
	}


	return PID;	
}

//just always pass in 255 to here please
int CFTDILibLinux::FindneoVIs(NeoDevice *pDevice, unsigned long DevTypes)
{
	int ret;
	int ctr = 0;
	int PID;
	struct ftdi_context ftdic;
    struct ftdi_device_list *ftdiDevList;
	struct ftdi_device_list *ListItemCur;
	char SerialNum[50];
	int iSerialNum;
	bool bMatch;
	size_t dev_type_it;
	unsigned long SUPPORTED_DEVICES[] = {
		NEODEVICE_VCAN3,
		NEODEVICE_FIRE,
		NEODEVICE_PLASMA_1_11,
		NEODEVICE_PLASMA_1_12,
		NEODEVICE_PLASMA_1_13,
		NEODEVICE_ION_2,
		NEODEVICE_ION_3,
		NEODEVICE_FIRE2

	};
	
#ifndef NO_FTDI

	if(ftdi_init(&ftdic) < 0)
        return -1;
		
	for (dev_type_it = 0 ; dev_type_it < (sizeof(SUPPORTED_DEVICES) / sizeof(unsigned long)) ; ++dev_type_it)
	{
		unsigned long DevType = SUPPORTED_DEVICES[dev_type_it];
		if((DevType & DevTypes) == 0)
			continue;
	
		PID = GetPID(DevType);

		ret = ftdi_usb_find_all(&ftdic, &ftdiDevList, 0x093c, PID);		
		
		if(ret < 1)	 //either failed or 0 if there are no devices plugged-in	
			continue;
		
		ListItemCur = ftdiDevList;
		
		while(ListItemCur != NULL)
		{
			bMatch = false;
			
			ftdi_usb_get_strings(&ftdic, ListItemCur->dev, NULL, 0, NULL, 0, SerialNum, 49);	
			iSerialNum = atoi(SerialNum);
			
			switch(DevType)
			{
				case NEODEVICE_FIRE:
					if(iSerialNum >= 50000 && iSerialNum <= 59999)
						bMatch = true;		
					if(iSerialNum >= 60200 && iSerialNum <= 79999)
						bMatch = true;
					break;
					
				case NEODEVICE_VCAN3:
                    if(iSerialNum >= 80000 && iSerialNum <= 120000)
                        bMatch = true;
                    else if(iSerialNum >= 120000 && iSerialNum <= 199999)
                        bMatch = true;
                    break;
				
				case NEODEVICE_ION_2:
				case NEODEVICE_ION_3:
					if(iSerialNum >= 400000 && iSerialNum <= 499999)
						bMatch = true;
				break;
				
				case NEODEVICE_PLASMA_1_11:
				case NEODEVICE_PLASMA_1_12:
				case NEODEVICE_PLASMA_1_13:
					if(iSerialNum >= 300000 && iSerialNum <= 399999)
					bMatch = true;
				break;	
				
				default:
					bMatch = true;
					break;
			}
			
			
			if(bMatch)
			{
				pDevice[ctr].DeviceType = DevType;
				pDevice[ctr].NumberOfClients = 0;
				pDevice[ctr].MaxAllowedClients = 1;
				pDevice[ctr].SerialNumber = iSerialNum;	
				ctr++;			
			}
			
			ListItemCur = ListItemCur->next;
		}

		ftdi_list_free(&ftdiDevList);
	}

	ftdi_deinit(&ftdic);
#endif

	return ctr;
}

bool CFTDILibLinux::OpenNeoVI(NeoDevice *pDevice)
{
	int PID;
	int iRetVal;
	char SerialNumber[20];
	
	if(m_bDeviceOpen)
		return true;
		
#ifndef NO_FTDI
		
	if(ftdi_init(&m_ftdic) < 0)
        return false;
	
	PID = GetPID(pDevice->DeviceType);
	
	if(PID == 0)
		return false;
		
	switch(pDevice->DeviceType)
	{
		case NEODEVICE_ION_2:		
		case NEODEVICE_ION_3:
			sprintf(SerialNumber, "%dic", pDevice->SerialNumber);
		break;
				
		case NEODEVICE_PLASMA_1_12:
		case NEODEVICE_PLASMA_1_13:
			sprintf(SerialNumber, "%dpc", pDevice->SerialNumber);
		break;
		
		default:
			sprintf(SerialNumber, "%d", pDevice->SerialNumber);
		break;
		
	}

    sprintf(SerialNumber, "%d", pDevice->SerialNumber);
	
	iRetVal = ftdi_usb_open_desc(&m_ftdic, 0x093C, PID, NULL, SerialNumber);
	
	if(iRetVal < 0)
		return false;
	
	m_bDeviceOpen = true;
	
	ftdi_set_baudrate(&m_ftdic, 500000);
	
	iRetVal = ftdi_read_data_set_chunksize(&m_ftdic, 16 * 1024);
#endif

	return true;
}

bool CFTDILibLinux::CloseDevice(void)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return true;
#ifndef NO_FTDI
		
	iRetVal = ftdi_usb_close(&m_ftdic);
	
	if(iRetVal < 0)
	{
		//error of some kind
	}
	
	ftdi_deinit(&m_ftdic);
	
	m_bDeviceOpen = false;
#endif	
	return true;
}

bool CFTDILibLinux::SetLatencyTimer(unsigned char latency)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return false;
#ifndef NO_FTDI
		
	iRetVal = ftdi_set_latency_timer(&m_ftdic, latency);
#endif

	return iRetVal == 0 ? true : false;
}

bool CFTDILibLinux::PurgeBuffers(void)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return false;
#ifndef NO_FTDI
		
	ftdi_usb_purge_rx_buffer(&m_ftdic);
	ftdi_usb_purge_tx_buffer(&m_ftdic);
#endif

	return true;
}


int CFTDILibLinux::Write(unsigned char *buf, int size)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return 0;
	
#ifndef NO_FTDI
	iRetVal = ftdi_write_data(&m_ftdic, buf, size);
#endif

	return iRetVal;
}


int CFTDILibLinux::Read(unsigned char *buf, int size)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
    {
        printf("device not opened!\n");
		return 0;
    }
	
	if(size > 4096)
		size = 4096;	
	
#ifndef NO_FTDI
	iRetVal = ftdi_read_data(&m_ftdic, buf, size);
#endif

	return iRetVal;	
}

void CFTDILibLinux::Reset(void)
{
	if(!m_bDeviceOpen)
		return;
		
#ifndef NO_FTDI
	ftdi_usb_reset(&m_ftdic);
#endif

}




