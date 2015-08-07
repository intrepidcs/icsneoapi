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

#ifndef _ICSNEOLINUXAPI_H
#define _ICSNEOLINUXAPI_H

int icsneoSendTextApi(void* hObject,const char * buffer,unsigned int iNumCharacters);
int icsneoGetTextApi(void* hObject,char * buffer,unsigned int iCharacterCapacity, unsigned int *piCharactersFilled,unsigned int iTimeoutMs);
void icsneoInitializeAPI(void); 
void icsneoShutdownAPI(void); 
int  icsneoFindNeoDevices(unsigned long DeviceTypes, NeoDevice *pNeoDevice, int *pNumDevices);
int  icsneoOpenNeoDevice(NeoDevice *pNeoDevice,	void **hObject, unsigned char * bNetworkIDs, int bConfigRead, int bSyncToPC);
int  icsneoEnableNetworkCom(void* hObject, int iEnable);
int  icsneoClosePort(void* hObject, int * pNumberOfErrors);
int  icsneoGetMessages(void* hObject, icsSpyMessage * pMsg, int * pNumberOfMessages, int * pNumberOfErrors);
int  icsneoWaitForRxMessagesWithTimeOut(void* hObject, unsigned int iTimeOut); //To avoid 'polling' for messages, which is very inefficient
int  icsneoTxMessages(void* hObject, icsSpyMessage *pMsg,	int lNetworkID, int lNumMessages);
int  icsneoSetBitRate(void* hObject, int BitRate, int NetworkID);
int  icsneoSetReflashDisplayCallbacks(void (*OnPrompt)(unsigned long), void (*OnReflashUpdate)(const wchar_t*,unsigned long));
int  icsneoGetTimeStampForMsg(void* hObject, icsSpyMessage *pMsg, double *pTimeStamp);
void icsneoFreeObject(void* hObject);
int  icsneoGetHWFirmwareInfo(void* hObject, stAPIFirmwareInfo* pInfo);
int  icsneoGetDLLFirmwareInfo(void* hObject, stAPIFirmwareInfo* pInfo);
int  icsneoGetStoredFirmwareInfo(unsigned long NeoDeviceType, char *VersionInfo);
int  icsneoGetLastAPIError(void* hObject, unsigned long *pErrorNumber);
int  icsneoGetErrorInfoW(unsigned long ulErrorNumber, wchar_t *szErrorDescriptionShort, wchar_t *szErrorDescriptionLong,
						int *iMaxLengthShort, int *iMaxLengthLong, int *iErrorSeverity, int *iRestartNeeded);
int  icsneoGetErrorInfo(unsigned long ulErrorNumber, char *szErrorDescriptionShort, char *szErrorDescriptionLong, 
                        int *iMaxLengthShort, int *iMaxLengthLong, int *iErrorSeverity, int *iRestartNeeded);
int  icsneoGetErrorMessages(void* hObject, int *pErrorMsgs, int *pNumberOfErrors);			
int  icsneoEnableNetworkRXQueue(void* hObject, int iEnable);
int  icsneoGetVCAN3Settings(void* hObject, SVCAN3Settings *pSettings, int iNumBytes);
int  icsneoSetVCAN3Settings(void* hObject, SVCAN3Settings *pSettings, int iNumBytes, int bSaveToEEPROM);
int  icsneoGetFireSettings(void* hObject, SFireSettings *pSettings, int iNumBytes);
int  icsneoSetFireSettings(void* hObject, SFireSettings *pSettings, int iNumBytes, int bSaveToEEPROM);
int  icsneoGetDeviceParameters(void* hObject, char *pParameters, char *pValues, short ValuesLength);
int  icsneoSetDeviceParameters(void* hObject, char *pParmValue, int *pErrorIndex, int bSaveToEEPROM);
int  icsneoSetRTC(void* hObject,const icsSpyTime *time);
int  icsneoGetRTC(void* hObject, icsSpyTime *time);
int  icsneoGetSerialNumber(void* hObject, unsigned int *piSerialNumber);
int icsneoISO15765_EnableNetworks(void* hObject, unsigned long ulNetworks);
int icsneoISO15765_DisableNetworks(void* hObject);
int icsneoISO15765_TransmitMessage(void* hObject, unsigned long ulNetworkID, 
								   stCM_ISO157652_TxMessage *pMsg, unsigned long ulBlockingTimeout);

void icsneoSetISO15765RxParameters(void* hObject,int lNetwork,int lEnable,spyFilterLong *pFF_CFMsgFilter,icsSpyMessage *pTxMsg,int lCFTimeOutMs,int lFlowCBlockSize,int lUsesExtendedAddressing, int lUseHardwareIfPresent);
//CoreMini script functions
int  icsneoScriptStop(void* hObject);
int  icsneoScriptStart(void* hObject, int iLocation);
int  icsneoScriptLoad(void* hObject, const unsigned char * bin, unsigned long len_bytes, int iLocation);
int  icsneoScriptClear(void* hObject, int iLocation);
int  icsneoScriptStartFBlock(void* hObject,unsigned int fb_index);
int  icsneoScriptStopFBlock(void* hObject,unsigned int fb_index);
int  icsneoScriptGetFBlockStatus(void* hObject, unsigned int fb_index, int *piStatus);
int  icsneoScriptReadISO15765_2_TxMessage(void* hObject, unsigned int iIndex, stCM_ISO157652_TxMessage *pTxMessage);
int  icsneoScriptWriteISO15765_2_TxMessage(void* hObject, unsigned int iIndex,const stCM_ISO157652_TxMessage * pTxMessage);
int icsneoScriptReadISO15765_2_RxMessage(void* hObject, unsigned int iIndex,  stCM_ISO157652_RxMessage *pRxMessage);
int icsneoScriptWriteISO15765_2_RxMessage(void* hObject,unsigned int iIndex,const stCM_ISO157652_RxMessage * pRxMessage);
int  icsneoScriptReadRxMessage(void* hObject, unsigned int iIndex, icsSpyMessage *pTxMessageMask, icsSpyMessage *pTxMessageValue);
int  icsneoScriptReadTxMessage(void* hObject, unsigned int iIndex, icsSpyMessage *pTxMessage);
int  icsneoScriptWriteRxMessage(void* hObject, unsigned int iIndex,const icsSpyMessage *pTxMessageMask,const  icsSpyMessage *pTxMessageValue);
int  icsneoScriptWriteTxMessage(void* hObject, unsigned int iIndex,const icsSpyMessage *pTxMessage);
int  icsneoScriptReadAppSignal(void* hObject, unsigned int iIndex,double * dValue);
int  icsneoScriptWriteAppSignal(void* hObject, unsigned int iIndex,double dValue);
int  icsneoScriptGetScriptStatus(void* hObject, int *piStatus);
	

int  icsneoReadSDCard(void* hObject,unsigned long iSectorIndex,unsigned char *data, unsigned long *bytesRead);
int  icsneoWriteSDCard(void* hObject,unsigned long iSectorIndex,const unsigned char *data);

int  icsneoScriptLoadReadBin(void* hObject, const unsigned char * bin, unsigned long len_bytes, int iLocation);
int icsneoOpenRemoteNeoDevice(int *hObject, char *pIPAddress, NeoDevice *pNeoDevice);



#endif