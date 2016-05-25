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

#include "OEvent.h"
#include "cicsneoVI.h"
#include "OSAbstraction.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <string.h>
#include "ftdilib.h"

void* p_neoVIObjects[256];
bool g_bEnableAutoFirmwareUpdate = false;
bool g_bEnableLocalSockServer = false;

void icsneoInitializeAPI(void)
{
	int i;

	for(i = 0; i < 256; i++)
		p_neoVIObjects[i] = 0;			
}

bool ValidateNeoVIObject(void* hObject)
{
	int NeoObjectIndex;
	bool bFound = false;

	if(hObject == 0)
		return false;

	for(NeoObjectIndex = 0; NeoObjectIndex < 256; NeoObjectIndex++)
	{
		if(p_neoVIObjects[NeoObjectIndex] == 0 || p_neoVIObjects[NeoObjectIndex] != hObject)
			continue;

		bFound =  true;	
		break;

	}

	return bFound;
}

cicsneoVI *GetNeoVIObject(void)
{
	short i;
	bool bFound = false;
	cicsneoVI *pObj = NULL;
	
	for(i = 0; i < 256; i++)
	{
		if(p_neoVIObjects[i] != 0)
			continue;

		bFound =  true;	
		
		break;
	}
	
	if(!bFound)
		return NULL;

    pObj = new cicsneoVI;
 	
	p_neoVIObjects[i] = (void*) pObj;
	
	return  pObj;
}

void FreeNeoVIObject(cicsneoVI *pNeoVI)
{
	short i;
	bool bFound = false;
		
	for(i = 0; i < 256; i++)
	{
		if(p_neoVIObjects[i] == 0 || p_neoVIObjects[i] != (void*) pNeoVI)
			 continue;
		
		bFound = true;	
		break;
	}
	
	if(bFound)
	{
		delete ((cicsneoVI *) p_neoVIObjects[i]);
		p_neoVIObjects[i] = 0;
	}
}


int icsneoFindNeoDevices(unsigned long DevType, NeoDevice *pDevice, int *num)
{
	int ret = 0;
	int ctr = 0;
	int PID;
	struct ftdi_context ftdic;
    struct ftdi_device_list *ftdiDevList;
	struct ftdi_device_list *ListItemCur;
	char SerialNum[128],Manufacturer[128],Description[128];
	int iSerialNum;
	bool bMatch;
	
	if(ftdi_init(&ftdic) < 0)
        return ret;	
		
	
	PID = 0x0601;
    ret = ftdi_usb_find_all(&ftdic, &ftdiDevList, 0x093c, PID);		
	
	if(ret < 1)	 //either failed or 0 if there are no devices plugged-in	
		return ret;

	ListItemCur = ftdiDevList;
	
	strcpy(SerialNum,"\0");
	
	while(ListItemCur != NULL)
	{
		bMatch = false;
		
		int ret = ftdi_usb_get_strings(&ftdic, ListItemCur->dev, Manufacturer, 128, Description, 128, SerialNum, 128);	
		iSerialNum = atoi(SerialNum);
		
		switch(DevType)
		{		

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

    *num = ctr;

	return ctr;
}

int  icsneoOpenNeoDevice(NeoDevice *pNeoDevice,	
								 void **hObject, 
								 unsigned char * bNetworkIDs,
								 int bConfigRead, 
								 int iOptions)
{
	cicsneoVI *pNeoVIObj;
	
	int ret;
	
	if(!pNeoDevice)
		return 0;
	
    pNeoVIObj = GetNeoVIObject();

	if(!pNeoVIObj)
		return 0;

	*hObject = (void*) pNeoVIObj;

	if(!pNeoVIObj->OpenDevice(pNeoDevice))
    {
        FreeNeoVIObject(pNeoVIObj);
        *hObject = 0;
        printf("OpenDevice Failed with ret = %d\n",ret);
        return 0;
    }
		
	return 1;
}

int  icsneoTxMessages(void* hObject, icsSpyMessage *pMsg,	int lNetworkID, int lNumMessages)
{
	if(!ValidateNeoVIObject(hObject))
		return false;
	
	cicsneoVI *pOb = (cicsneoVI *) hObject;

 	return pOb->TransmitMessages(pMsg, lNetworkID, lNumMessages);
}

int icsneoClosePort(void* hObject, int * pNumberOfErrors)
{
	bool bActuallyClosed;
	
	if(!ValidateNeoVIObject(hObject))
		return false;

	cicsneoVI *pOb = (cicsneoVI *) hObject;
	
	if(!pOb->CloseDevice())
		return false;
	
	return true;
}

void  icsneoFreeObject(void* hObject)
{
	if(!ValidateNeoVIObject(hObject))
		return;

	FreeNeoVIObject((cicsneoVI *)hObject);
}

int  icsneoGetMessages(void* hObject, icsSpyMessage * pMsg, int * pNumberOfMessages,
							   int * pNumberOfErrors)
{
	int iRetVal;

	if(!ValidateNeoVIObject(hObject))
		return false;

	cicsneoVI *pOb = (cicsneoVI *) hObject;

	iRetVal = pOb->ReadOutMessages(pMsg, *pNumberOfMessages);

	return iRetVal;
}

void icsneoShutdownAPI(void)
{
	int i;
	cicsneoVI *pOb;
	bool bActuallyClosed;

	for(i = 0; i < 256; i++)
	{
		if (p_neoVIObjects[i] != 0)
		{
			pOb = (cicsneoVI *) p_neoVIObjects[i];
			pOb->CloseDevice();
			delete pOb;
			p_neoVIObjects[i] = 0;
		}
	}
}

int  icsneoWaitForRxMessagesWithTimeOut(void* hObject, unsigned int iTimeOut)
{
	int iRetVal;

	if(!ValidateNeoVIObject(hObject))
		return false;
	
	cicsneoVI *pOb = (cicsneoVI *) hObject;

	iRetVal = pOb->WaitForRxMessagesWithTimeOut(iTimeOut);

	return iRetVal;
}


int  icsneoGetTimeStampForMsg(void* hObject, icsSpyMessage *pMsg, double *pTimeStamp)
{
	int iRetVal;

	if(!ValidateNeoVIObject(hObject))
		return false;

	cicsneoVI *pOb = (cicsneoVI *) hObject;

	iRetVal = pOb->GetTimeStampForMsg(pMsg, pTimeStamp);

	return iRetVal;	
}