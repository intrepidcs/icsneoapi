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

#ifndef _ICSNEO40API_H
#define _ICSNEO40API_H

#include "icsnVC40.h"

#ifdef _WIN32
    #define ICSCC _stdcall
#else
    #define ICSCC
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ICSCC icsneoInitializeAPI(void);
void ICSCC icsneoShutdownAPI(void);
int  ICSCC icsneoGetDLLVersion();
int  ICSCC icsneoValidateHObject(void *hObject);
int  ICSCC icsneoFindNeoDevices(unsigned long DeviceTypes, NeoDevice *pNeoDevice, int *pNumDevices);
int  ICSCC icsneoFindRemoteNeoDevices(char *pIPAddress, NeoDevice *pNeoDevice, int *pNumDevices);
int  ICSCC icsneoOpenNeoDevice(NeoDevice *pNeoDevice, void **hObject, unsigned char *bNetworkIDs, int bConfigRead, int bSyncToPC);
int  ICSCC icsneoOpenPortEx(int lPortNumber, int lPortType, int lDriverType,  int lIPAddressMSB, 
                        int lIPAddressLSBOrBaudRate, int bConfigRead, unsigned char *bNetworkID,  void **hObject);
int  ICSCC icsneoClosePort(void *hObject, int *pNumberOfErrors);
int  ICSCC icsneoGetMessages(void *hObject, icsSpyMessage *pMsg, int *pNumberOfMessages, int *pNumberOfErrors);
int  ICSCC icsneoWaitForRxMessagesWithTimeOut(void *hObject, unsigned int iTimeOut);
int  ICSCC icsneoTxMessages(void *hObject, icsSpyMessage *pMsg,	int lNetworkID, int lNumMessages);
int  ICSCC icsneoTxMessagesEx(void *hObject, icsSpyMessage *pMsg, int lNetworkID, int lNumMessages, int *NumTxed,int reserved2);
int  ICSCC icsneoSetBitRate(void *hObject, int BitRate, int NetworkID);
int  ICSCC icsneoGetTimeStampForMsg(void *hObject, icsSpyMessage *pMsg, double *pTimeStamp);
void ICSCC icsneoFreeObject(void *hObject);
int  ICSCC icsneoGetLastAPIError(void *hObject, unsigned long *pErrorNumber);
int  ICSCC icsneoGetErrorInfoW(unsigned long ulErrorNumber, wchar_t *szErrorDescriptionShort, wchar_t *szErrorDescriptionLong,
						int *iMaxLengthShort, int *iMaxLengthLong, int *iErrorSeverity, int *iRestartNeeded);
int  ICSCC icsneoGetErrorInfo(unsigned long ulErrorNumber, char *szErrorDescriptionShort, char *szErrorDescriptionLong, 
                        int *iMaxLengthShort, int *iMaxLengthLong, int *iErrorSeverity, int *iRestartNeeded);
int  ICSCC icsneoGetErrorMessages(void *hObject, int *pErrorMsgs, int *pNumberOfErrors);			
int  ICSCC icsneoEnableNetworkRXQueue(void *hObject, int iEnable);
int  ICSCC icsneoGetVCAN3Settings(void *hObject, SVCAN3Settings *pSettings, int iNumBytes);
int  ICSCC icsneoSetVCAN3Settings(void *hObject, SVCAN3Settings *pSettings, int iNumBytes, int bSaveToEEPROM);
int  ICSCC icsneoGetFireSettings(void *hObject, SFireSettings *pSettings, int iNumBytes);
int  ICSCC icsneoSetFireSettings(void *hObject, SFireSettings *pSettings, int iNumBytes, int bSaveToEEPROM);
int  ICSCC icsneoGetDeviceParameters(void *hObject, char *pParameters, char *pValues, short ValuesLength);
int  ICSCC icsneoSetDeviceParameters(void *hObject, char *pParmValue, int *pErrorIndex, int bSaveToEEPROM);
int  ICSCC icsneoGetSerialNumber(void *hObject, unsigned int *piSerialNumber);
int  ICSCC icsneoScriptStop(void *hObject);
int  ICSCC icsneoScriptStart(void *hObject, int iLocation);
int  ICSCC icsneoScriptLoad(void *hObject, const unsigned char * bin, unsigned long len_bytes, int iLocation);
int  ICSCC icsneoScriptClear(void *hObject, int iLocation);
int  ICSCC icsneoScriptStartFBlock(void *hObject,unsigned int fb_index);
int  ICSCC icsneoScriptStopFBlock(void *hObject,unsigned int fb_index);
int  ICSCC icsneoScriptGetFBlockStatus(void *hObject, unsigned int fb_index, int *piStatus);
int  ICSCC icsneoScriptReadISO15765_2_TxMessage(void *hObject, unsigned int iIndex, stCM_ISO157652_TxMessage *pTxMessage);
int  ICSCC icsneoScriptWriteISO15765_2_TxMessage(void *hObject, unsigned int iIndex,const stCM_ISO157652_TxMessage *pTxMessage);
int  ICSCC icsneoScriptReadAppSignal(void *hObject, unsigned int iIndex,double * dValue);
int  ICSCC icsneoScriptWriteAppSignal(void *hObject, unsigned int iIndex,double dValue);
int  ICSCC icsneoScriptGetScriptStatus(void *hObject, int *piStatus);
int  ICSCC icsneoOpenRemoteNeoDevice(const char *pIPAddress, NeoDevice *pNeoDevice,	int *hObject, unsigned char *bNetworkIDs, int iOptions);
int  ICSCC icsneoISO15765_TransmitMessage(void *hObject, unsigned long ulNetworkID, stCM_ISO157652_TxMessage *pMsg, unsigned long ulBlockingTimeout);
void ICSCC icsneoGetISO15765Status(void *hObject, int lNetwork, int lClearTxStatus, int lClearRxStatus, int *lTxStatus, int *lRxStatus);
void ICSCC icsneoSetISO15765RxParameters(void *hObject, int lNetwork, int lEnable, spyFilterLong *pFF_CFMsgFilter, icsSpyMessage *pTxMsg, int lCFTimeOutMs,
											 int lFlowCBlockSize,  int lUsesExtendedAddressing, int lUseHardwareIfPresent);
int  ICSCC icsneoDownloadISO15765_2_TXScript(void *hObject, unsigned int NetworkID);
int  ICSCC icsneoClearISO15765_2_TxScript(void* hObject);
int  ICSCC icsneoTransmitISO15765_2_Message(void *hObject, stCM_ISO157652_TxMessage *pTxMessage);
int  ICSCC icsneoGetConfiguration(void *hObject, unsigned char *pData, int *lNumBytes);
int  ICSCC icsneoSendConfiguration(void *hObject, unsigned char *pData, int lNumBytes);
int  ICSCC icsneoISO15765_EnableNetworks(void *hObject, unsigned long ulNetworks);
int  ICSCC icsneoISO15765_DisableNetworks(void *hObject);
int  ICSCC icsneoStartSockServer(void *hObject, int iPort);
int  ICSCC icsneoStopSockServer(void *hObject);

#ifdef __cplusplus
}
#endif

#endif