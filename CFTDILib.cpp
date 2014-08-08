/*
Copyright (c) 2014 Intrepid Control Systems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
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
			
		case NEODEVICE_YELLOW:
		
			PID = 0x0701;
			
			break;
		
		default:
		
			PID = 0;
		
			break;
	}


	return PID;	
}

//just always pass in 255 to here please
int CFTDILibLinux::FindneoVIs(NeoDevice *pDevice, unsigned long DevType)
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
	
#ifndef NO_FTDI

	if(ftdi_init(&ftdic) < 0)
        return -1;
	
	PID = GetPID(DevType);

    ret = ftdi_usb_find_all(&ftdic, &ftdiDevList, 0x093c, PID);		
	
	if(ret < 1)	 //either failed or 0 if there are no devices plugged-in	
		return ret;
	
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
			
			case NEODEVICE_YELLOW:
			
				if((iSerialNum >= 40000 && iSerialNum <= 49999) ||
				   (iSerialNum >= 100000 && iSerialNum <= 199999))
					   bMatch = true;
			
				break;
			
			
			case NEODEVICE_VCAN3:
			
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




