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

#include "OEvent.h"
#include "cicsneoVI.h"
#include "OSAbstraction.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <string>
#include <string.h>
#include "CFTDILib.h"

class GetNeoVIObject;
OCriticalSection neovi_objects_lock;
std::set<cicsneoVI*> neovi_objects;

bool g_bEnableAutoFirmwareUpdate = false;
bool g_bEnableLocalSockServer = false;

static void InitializeNeoAPI(void)
{
    neovi_objects.clear();
    g_bEnableAutoFirmwareUpdate = false;
    g_bEnableLocalSockServer = false;		
}

static void ShutdownNeoAPI(void)
{
    neovi_objects_lock.Lock();
    std::set<cicsneoVI*>::iterator iter = neovi_objects.begin();
    for (; iter != neovi_objects.end(); ++iter) {
        cicsneoVI* obj = *iter;
        if (!obj) {
            continue;
        }
        obj->CloseDevice();
        delete obj;
        obj = NULL;
    }
    neovi_objects_lock.Unlock();
}

static cicsneoVI* ValidateNeoVIObject(void* hObject)
{
    cicsneoVI* ret = NULL;
    neovi_objects_lock.Lock();
    std::set<cicsneoVI*>::iterator it = neovi_objects.find((cicsneoVI*)hObject);
    if(it != neovi_objects.end())
        ret = *it;
    neovi_objects_lock.Unlock();
    return ret;
}

static cicsneoVI* MakeNeoVIObject()
{
    cicsneoVI *pObj = new cicsneoVI();
    neovi_objects_lock.Lock();
    neovi_objects.insert(pObj);
    neovi_objects_lock.Unlock();
    return pObj;
}

extern "C" void __attribute__((visibility("default"))) FreeNeoVIObject(cicsneoVI *pNeoVI)
{
    if(!pNeoVI)
        return;
    neovi_objects_lock.Lock();
    std::set<cicsneoVI*>::iterator iter = neovi_objects.find(pNeoVI);
    if(iter != neovi_objects.end())
    {
        delete pNeoVI;
        neovi_objects.erase(iter);
    }
    neovi_objects_lock.Unlock();
}

bool is_initialized_by_constructor = false;

extern "C" void __attribute__((visibility("default"))) icsneoInitializeAPI(void)
{
    //puts("Shared Library is being initialized\n");
    if (is_initialized_by_constructor) {
        return;
    }
    InitializeNeoAPI();
}


extern "C" void __attribute__((visibility("default"))) icsneoShutdownAPI(void)
{
    ShutdownNeoAPI();	
}

__attribute__((constructor)) void initialize_shared_library(void)
{
    icsneoInitializeAPI();
    is_initialized_by_constructor = true;
}

__attribute__((deconstructor)) void deinitialize_shared_library(void)
{
    icsneoShutdownAPI();
    is_initialized_by_constructor = false;
}
    
extern "C" int __attribute__((visibility("default"))) icsneoClosePort(void* hObject, int * pNumberOfErrors)
{
    cicsneoVI *pOb = ValidateNeoVIObject(hObject);
    if (!pOb) {
        return 0;
    }

    *pNumberOfErrors = 0;

    return pOb->CloseDevice() ? 1 : 0;
}


extern "C" void __attribute__((visibility("default"))) icsneoFreeObject(void* hObject)
{
    FreeNeoVIObject(ValidateNeoVIObject(hObject));
}


extern "C" int __attribute__((visibility("default"))) icsneoGetMessages(void* hObject, icsSpyMessage * pMsg, int * pNumberOfMessages,
                               int * pNumberOfErrors)
{
    int iRetVal;

    cicsneoVI *pOb = ValidateNeoVIObject(hObject);
    if (!pOb) {
        return 0;
    }

    iRetVal = pOb->ReadOutMessages(pMsg, *pNumberOfMessages);
    
    *pNumberOfErrors = 0;

    return iRetVal;
}


extern "C" int __attribute__((visibility("default")))icsneoTxMessages(void* hObject, icsSpyMessage *pMsg,	int lNetworkID, int lNumMessages)
{
    cicsneoVI *pOb = ValidateNeoVIObject(hObject);
    if (!pOb) {
        return 0;
    }

     return pOb->TransmitMessages(pMsg, lNetworkID, lNumMessages);
}

extern "C" int __attribute__((visibility("default"))) icsneoFindNeoDevices(unsigned long DeviceTypes, NeoDevice *pNeoDevice, int *pNumDevices)
{
    int found = CFTDILibLinux::FindneoVIs(pNeoDevice, DeviceTypes);
    if (found < 0)
        return 0;
    if(pNumDevices)
        *pNumDevices = found;
    return 1;
}
 

extern "C" int __attribute__((visibility("default"))) icsneoOpenNeoDevice(NeoDevice *pNeoDevice,	
                                 void** hObject, 
                                 unsigned char * bNetworkIDs,
                                 int bConfigRead, 
                                 int iOptions)
{
    if(!pNeoDevice || !hObject)
        return 0;

    if(*hObject)
        FreeNeoVIObject(ValidateNeoVIObject(*hObject));

    cicsneoVI* pNeoVIObj = MakeNeoVIObject();

    if(!pNeoVIObj)
        return 0;

    *hObject = pNeoVIObj;

    if(!pNeoVIObj->OpenDevice(pNeoDevice))
    {
        FreeNeoVIObject(pNeoVIObj);
        *hObject = 0;
        return 0;
    }
        
    return 1;
}

extern "C" int __attribute__((visibility("default"))) icsneoGetTimeStampForMsg(void* hObject, icsSpyMessage *pMsg, double *pTimeStamp)
{	           
    cicsneoVI *pOb = ValidateNeoVIObject(hObject);
    if (!pOb || !pTimeStamp) {
        return 0;
    }
   
    return pOb->GetTimeStampForMsg(pMsg, pTimeStamp);
}

extern "C" int __attribute__((visibility("default"))) icsneoWaitForRxMessagesWithTimeOut(void* hObject, unsigned int iTimeOut)
{
    
    cicsneoVI *pOb = ValidateNeoVIObject(hObject);
    if (!pOb) {
        return 0;
    }

    return pOb->WaitForRxMessagesWithTimeOut(iTimeOut);
}
