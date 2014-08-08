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

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "icsnVC40.h"
#include "icsneoLinuxAPI.h"
#include "OCriticalSection.h"
#include <stdlib.h>

int hObject = -1;

OCriticalSection csShutdown;

bool bShutDown;

void *ReadThread(void *lParam)
{
	int NumErrors = 0;
	int NumMsgs = 0;
	icsSpyMessage Msgs[20000];
	bool bDone = false;
  
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
	while(1)
	{	
	   //check for shutdown from the main thread
 	   csShutdown.Lock();
   	   bDone = bShutDown;
       csShutdown.Unlock();
       
       if(bDone)   
			break;
     			
		if(icsneoWaitForRxMessagesWithTimeOut(hObject, 100) == 0)
    		continue;
    		
    	if(icsneoGetMessages(hObject, Msgs, &NumMsgs, &NumErrors) == 0)
    		continue;     	
   	
     	for(int i = 0; i < NumMsgs; i++)
     	{
              printf("Network %d ArbID = %lX - Data Bytes: ", Msgs[i].NetworkID,Msgs[i].ArbIDOrHeader);
              for(int j = 0; j < Msgs[i].NumberBytesData; j++)
                    printf("%02X ", Msgs[i].Data[j]);
     		  printf("\n");           
     	}  	
	}	
    	
  	printf("ReadThread done\n");
 
	return 0;
}

int main(void)
{
	NeoDevice Nd[255];
	int iRetVal = 0;
	int i;
	int NumDevices = 255;
	int NumErrors;
	pthread_t thread;
	bool bExit = false;
	icsSpyMessage OutMsg;
	char DeviceType[25];
	char chIn;
	unsigned long DeviceTypes;
	
	bShutDown = false;
	
	printf("Starting......\n");
	
	DeviceTypes = NEODEVICE_VCAN3;
	
	iRetVal = icsneoFindNeoDevices(DeviceTypes, Nd, &NumDevices);
		
	if(!NumDevices)
	{
		printf("No devices found...exiting with ret = %d\n",iRetVal);
		return 0;		
	}
	
	printf("%d Devices found\n", NumDevices);

	for(i = 0; i < NumDevices; i++)
	{
		switch(Nd[i].DeviceType)
		{
			case NEODEVICE_VCAN3:
			
				strcpy(DeviceType, "ValueCAN3");
				
				break;
				
            default:
            
                strcpy(DeviceType, "unknown device");                
			
		}		
		printf("Device %d: ", i + 1);
		printf("Serial # %d Type = %s\n", Nd[i].SerialNumber, DeviceType); 
		
	}
	
	printf("\nOpening the first device\n");
	
	iRetVal = icsneoOpenNeoDevice(&Nd[0], &hObject, NULL, 1, 0/*DEVICE_OPTION_DONT_ENABLE_NETCOMS*/);

    if(iRetVal == 1)
    {
   		printf("Device was opened!\n\n");
    }
    else
    {
    	printf("Failed to open the device\n");
    	icsneoShutdownAPI();	
    	return 0;    	
    }     
    
    pthread_t threadid;    
    pthread_create(&threadid,NULL,&ReadThread,NULL); 

    while(!bExit)
    {
        printf("enter t to transmit\n");
        printf("enter e to exit\n");
     	chIn = getc(stdin);              
    	
    	switch(chIn)
    	{            
            case 't':            
                icsSpyMessage OutMsg;
                OutMsg.ArbIDOrHeader = 0x500;
                OutMsg.Data[0] = 0x01;
                OutMsg.Data[1] = 0x02;
                OutMsg.Data[2] = 0x03;
                OutMsg.Data[3] = 0x04;
                OutMsg.Data[4] = 0x05;
                OutMsg.Data[5] = 0x06;
                OutMsg.Data[6] = 0x07;
                OutMsg.Data[7] = 0x08;
                OutMsg.NumberBytesData = 8;
                OutMsg.NumberBytesHeader = 2;      
                iRetVal = icsneoTxMessages(hObject, &OutMsg, NETID_HSCAN, 1);
                break;    		

             case 'e':

    			bExit = true;
    			
    			break;		    		
    	}    
    }
    
    printf("Ending......\n");

   
    csShutdown.Lock();
    bShutDown = true;
    csShutdown.Unlock();        
    usleep(1000 * 1000); //let the read thread cdlose
   
    icsneoClosePort(hObject, &NumErrors);
  	icsneoFreeObject(hObject);	
  	icsneoShutdownAPI();   
          
    printf("Main thread done....\n");
      
	return 0;
}

