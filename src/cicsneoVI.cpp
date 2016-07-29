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

#include "cicsneoVI.h"
#include "OSAbstraction.h"
#include "CFTDILib.h"
#include <sys/time.h>
#include <time.h>

cicsneoVI::cicsneoVI() 
{
    m_bDeviceOpen = false;
    m_pRxMsgAvailableEvent = new OEvent(NULL, true);	
    eReadState = eReadStateCallAPI;
    m_pRxThread = NULL;
    m_bneoShutdown = false;
    m_lReadDataCount = 0;   
    m_lReadDataIndexIn = 0;
    m_lReadDataIndexOut = 0;
    m_pcsReceivedData = new OCriticalSection();
    m_pMsgBuffer = new icsSpyMessage[MAX_INCOMING_MSG_BUFFER_SIZE + 1];
    m_InComingMsgCount = 0; 
    m_iRxThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL;
    m_iRxThreadAffinity = 0;
    m_bReceivedGoodPacketSinceOpen = false;
    // set the default network IDs (sixteen networks)
    for (int iCount =0;iCount < MAX_NETWORK_COLLECTION_SIZE; iCount++)
	{	
		m_bNetworkIDs[iCount] = iCount;
    }

    bReadData = new unsigned char[NEOVI_READBUFFSIZE];	
    bTempPacketBufferHost = new unsigned char[NEOVI_READBUFFSIZE];	
    m_ftdic = ftdi_new();

}
cicsneoVI::~cicsneoVI()
{
    if (m_bDeviceOpen)
        CloseDevice();

    delete m_pRxMsgAvailableEvent;
    delete m_pRxThread;
    m_bneoShutdown = true;
    delete m_pcsReceivedData;
    delete [] m_pMsgBuffer;
    delete [] bReadData;
    delete [] bTempPacketBufferHost;
    if (m_ftdic)
        ftdi_free(m_ftdic);
}

int cicsneoVI::ReadOutMessages(icsSpyMessage *msg, int &lNumberOfMessages)
{
    if(!m_bDeviceOpen)
    {
        printf("device not opened!\n");
		return 0;
    }     
    
    m_pcsReceivedData->Lock();
    // for each message in the que
    for (int lCount = 0; lCount < m_InComingMsgCount; lCount++)
    {
        *msg = m_pMsgBuffer[lCount];  // transfer the message value
        msg++;						// increment the message pointer
    }
    // reset the count ofAddMsgToRXQueue spy messages
	lNumberOfMessages = m_InComingMsgCount;
    m_InComingMsgCount = 0;
    
	m_pRxMsgAvailableEvent->Reset();
   
	// unlock the critical section
	m_pcsReceivedData->Unlock(); 
       
    return 1;
    
}

bool cicsneoVI::AddMsgToRXQueue(icsSpyMessage &stMsg, bool bSetEvent)
{
	bool bRetVal = false;

    m_pcsReceivedData->Lock();
   
	if(m_InComingMsgCount > MAX_INCOMING_MSG_BUFFER_SIZE)
    {
		m_pcsReceivedData->Unlock();
		return false;
    }
		 
        m_pMsgBuffer[m_InComingMsgCount] = stMsg;

		m_InComingMsgCount++;
        
		bRetVal = true;
		if(bSetEvent)
			m_pRxMsgAvailableEvent->Set();

	m_pcsReceivedData->Unlock();

	return bRetVal;
 }

bool cicsneoVI::TransmitMessages(icsSpyMessage *msg,unsigned long lNetworkID,unsigned long lNumMessages)
{
    int             num_bytes_data = 0;
    int             i = 0;
    unsigned char   data[128];
 
	switch (lNetworkID)
	{
	case NETID_DEVICE:
	case NETID_HSCAN:
    case NETID_HSCAN2:
    case NETID_MSCAN:
    case NETID_HSCAN3:
    case NETID_SWCAN:
    case NETID_SWCAN2:
    case NETID_LSFTCAN:
    case NETID_LSFTCAN2:
    case NETID_HSCAN4:
    case NETID_HSCAN5:
    case NETID_HSCAN6:
    case NETID_HSCAN7:   
    	break;
    default:
    	return false;
    } 

    data[0] = 0xAA;
    data[1] = NETID_RED;
    data[2] = 0; //data length goes here once we've built the frame
    data[3] = 0;
    data[4] = lNetworkID & 0xff;
    data[5] = (lNetworkID >> 8) & 0xff;
    data[6] = 0; // byte count byte for inner message
	data[7] = (msg->DescriptionID >> 8) & 0xff;
	data[8] = msg->DescriptionID & 0xff;

    i = 9;

	if(msg->StatusBitField & SPY_STATUS_XTD_FRAME)
	{
		data[i++] = (unsigned char)(msg->ArbIDOrHeader >> 21);	
	    data[i++] = (unsigned char)((((msg->ArbIDOrHeader & 0x001C0000) >> 13) & 0xFF) + (((msg->ArbIDOrHeader & 0x00030000) >> 16) & 0xFF) | 8); 
	    data[i++] = ((icsSpyMessageJ1850 *) msg)->Header[1];
	    data[i++] = ((icsSpyMessageJ1850 *) msg)->Header[0];
	}
	else
	{
	    data[i++] = (unsigned char)(msg->ArbIDOrHeader >> 3);	
	    data[i++] = (unsigned char)((msg->ArbIDOrHeader & 7) << 5);
	}

    data[i] = 0;

    if(msg->StatusBitField2 & SPY_STATUS2_HIGH_VOLTAGE)
    {
        if(lNetworkID == NETID_SWCAN || lNetworkID == NETID_SWCAN2)
            data[i] = 0x80;
    }

    if (msg->StatusBitField & SPY_STATUS_REMOTE_FRAME)
    {
    	data[i] |= msg->NumberBytesData | 0x40;
    }
    else
    {
        num_bytes_data = msg->NumberBytesData;

        if(msg->Protocol != SPY_PROTOCOL_CANFD)
        {
            if(num_bytes_data > 8)
                num_bytes_data = 8;

            data[i++] |= num_bytes_data;
        }
        else
        {
            data[i++] = 15;

            if(num_bytes_data <= 8)
                data[i] = num_bytes_data;
            else if(num_bytes_data <= 12)
            {
                num_bytes_data = 12;
                data[i] = 9;
            }
            else if(num_bytes_data <= 16)
            {
                num_bytes_data = 16;
                data[i] = 10;
            }
            else if(num_bytes_data <= 20)
            {
                num_bytes_data = 20;
                data[i] = 11;
            }
            else if(num_bytes_data <= 24)
            {
                num_bytes_data = 24;
                data[i] = 12;
            }
            else if(num_bytes_data <= 32)
            {
                num_bytes_data = 32;
                data[i] = 13;
            }
            else if(num_bytes_data <= 48)
            {
                num_bytes_data = 48;
                data[i] = 14;
            }
            else
            {
                num_bytes_data = 64;
                data[i] = 15;
            }

            data[i++] |= (msg->StatusBitField3 & SPY_STATUS3_CANFD_BRS) ? 0x80 : 0;

            // if there's more than 8 bytes we need an EDP
            if((num_bytes_data > 8) && (!msg->ExtraDataPtr || !msg->ExtraDataPtrEnabled))
                return false;
           
        }

        const unsigned char* src = (msg->ExtraDataPtrEnabled && msg->ExtraDataPtr) ?
            (const unsigned char*)msg->ExtraDataPtr : msg->Data;

        int src_count = msg->NumberBytesData < num_bytes_data ? msg->NumberBytesData : num_bytes_data;

        memcpy(&data[i], src, src_count); //copy the data over
        memset(&data[i + src_count], 0, num_bytes_data - src_count); //0 pad anything extra
    	
        i += num_bytes_data;
    }
    
    //fill in the byte count byte; this is number of bytes after the network id
    data[6] = (unsigned char)((lNetworkID & 0xf) + ((i - 6) << 4));

    //fill in the outer payload length -- for some unknown reason this needs to +1 the real length
    data[2] = (unsigned char)((i + 1) & 0xff);
    data[3] = (unsigned char)(((i + 1) >> 8) & 0xff);

    // align to 16 bit boundry
    // TODO: don't do this on a RAD-GALAXY
    if(i & 1) 
        data[i++] = 'A';

    if(!m_bDeviceOpen)
        return 0;    

	int written = ftdi_write_data(m_ftdic, data, i); 

    return written > 0;

}

int cicsneoVI::WaitForRxMessagesWithTimeOut(unsigned int iTimeOut)
{
	unsigned long dwResult = m_pRxMsgAvailableEvent->Wait(iTimeOut);
	return dwResult == WAIT_OBJECT_0 ? true : false;
}

bool cicsneoVI::OpenDevice(NeoDevice *pDevice)
{
	int PID;
	int iRetVal;
	char SerialNumber[20];
	
	if(m_bDeviceOpen)
		return true;		
	
	if(!m_ftdic)
        return false;
	
	PID = CFTDILibLinux::GetPID(pDevice->DeviceType);
	
	if(PID == 0)
		return false;

    sprintf(SerialNumber, "%d", pDevice->SerialNumber);
	
	iRetVal = ftdi_usb_open_desc(m_ftdic, 0x093C, PID, NULL, SerialNumber);
	if(iRetVal < 0)
		return iRetVal;
	
	m_bDeviceOpen = true;

    ftdi_usb_reset(m_ftdic);
	
	ftdi_set_baudrate(m_ftdic, 500000); 

    PurgeBuffers();  
        
    //Setup device before transmission or reception starts
    
    unsigned char disable_net_comms[] = {0xAA, 0x2B, 0x07,0x00,0xCE};
    ftdi_write_data(m_ftdic, disable_net_comms, sizeof(disable_net_comms));

    unsigned char clear_buffer[1024];
    while (ftdi_read_data(m_ftdic, clear_buffer, 1024) > 0); // clear out stale data
    
    unsigned char go_online[] = {
        0xAA, 0x1B, 0xA3, 0x42, 0xAA, 0x1B, 0xA1, 0x44, 0xAA, 0x9B, 0xA2, 0xB6, 0x9F,
        0x02, 0xDB, 0x6A, 0xD0, 0xF7, 0x6D, 0xF3, 0xAA, 0x0B, 0x06, 0x00, 0xB6, 0xAA,
        0x1B, 0xB7, 0x2E, 0xAA, 0x30, 0x02, 0x06, 0x10, 0xB8, 0xAA, 0x0B, 0x06, 0x00,
        0xB6, 0x41, 0xAA, 0x0B, 0x06, 0x00, 0xBC, 0x41, 0xAA, 0x2B, 0x07, 0x01, 0xCD
    };
    ftdi_write_data(m_ftdic, go_online, sizeof(go_online));
                  
    m_pRxThread = new CNeoVIReadThread();

	if(!m_pRxThread->Start(this, m_iRxThreadPriority,m_iRxThreadAffinity))
	 return false; 

	return true;
}

unsigned long cicsneoVI::CNeoVIReadThread::Run()
{    
    cicsneoVI *neovi = (cicsneoVI *)m_pParam;
	bool bOkayToRead = false;
	bool bResult = true;				
	unsigned long dwBytes; 
	unsigned long lCurrentTime;
	unsigned char *pData = NULL;					
	unsigned int read_size;
	unsigned int LastTime;
    int iRetVal;
      

    if(ftdi_read_data_get_chunksize(neovi->m_ftdic, &read_size) != 0)
        read_size = 4096;
	
	pData = new unsigned char[read_size];
    
    while(1)
    {        
        if(neovi->m_bneoShutdown)
			break; 
      
         iRetVal = ftdi_read_data(neovi->m_ftdic, pData, read_size);  
         
       
         if(iRetVal > 0)
         {
              struct timeval tv;
	
            if(gettimeofday(&tv, NULL) != 0)
                return 0;
	
            unsigned int seconds = tv.tv_sec;
            unsigned int microseconds = tv.tv_usec;
	
            lCurrentTime =  (seconds * 1000) + (microseconds / 1000);
            neovi->ParseInputStream(lCurrentTime, pData, iRetVal);  
         }
                      
    }
    
    if(pData)
		delete [] pData;  

	return 0;
}

bool cicsneoVI::ParseInputStream(unsigned long lCurrentTime, 
								 unsigned char *bDataOutput, 
								 unsigned long lNumberOfBytes)
{
    unsigned int iCount;
	unsigned int iNumBytesTotal;
	unsigned char bCurrentByte;
	unsigned int iTempPositionOut;
	unsigned int iNumBytesLeftHost;
    bool bLongModeNoChecksum=false;
    bool bSecondByteMode=false;
    int iSecondByteCount=0;
	unsigned int bCurrentByteIndexHost = 0;
	unsigned char bCheckSum = 0;
	unsigned int iTempNumBytes;
	bool bFoundStartByte=false;
	bool bFoundStartFrame=false;
	bool bFrameError=false; 
    
    ///////////////////////////////////////////////////////////////////////////
	// first, add the neCoreMiniToSpyw data to the FIFO
	///////////////////////////////////////////////////////////////////////////
    
    if ((m_lReadDataCount+lNumberOfBytes) > NEOVI_READBUFFSIZE)
	{
		// FIFO Error
		if (!m_bReadDataOverflow)
		{
			m_bReadDataOverflow=true;
		}
	}
	else
	{
		//add the bytes to the fifo
		for(iCount = 0; iCount < lNumberOfBytes;iCount++)
		{
			// add the byte
			bReadData[m_lReadDataIndexIn] = bDataOutput[iCount];
			// increae the count
			m_lReadDataCount++;	
			// advance the index pointer
			m_lReadDataIndexIn++;
			// rollover the fifo if necessary
			if (m_lReadDataIndexIn ==NEOVI_READBUFFSIZE)
				m_lReadDataIndexIn=0;
		}
		
	}
     // now find all the packets in the buffer
	iTempPositionOut = m_lReadDataIndexOut; 
	iTempNumBytes =m_lReadDataCount;				

	for(iCount = 0; iCount < iTempNumBytes; iCount++)
	{
		bCurrentByte = bReadData[iTempPositionOut];
		iTempPositionOut++;
		
		if(iTempPositionOut == NEOVI_READBUFFSIZE)
			iTempPositionOut = 0;
	
		if(bFoundStartByte) 
		{	
			bCheckSum += bCurrentByte;

            if(bSecondByteMode)
            {
                 iSecondByteCount++;
                 if(iSecondByteCount == 1)
                    iNumBytesLeftHost = bCurrentByte;
                 else if(iSecondByteCount == 2)
                 {
                    iNumBytesLeftHost += (((int)bCurrentByte)<<8);
                    iNumBytesLeftHost -= 3;
                    iNumBytesTotal = iNumBytesLeftHost + 1;
                    bSecondByteMode = false;
                 }
            }

			if((iNumBytesLeftHost > 0) || bSecondByteMode)
			{
				bTempPacketBufferHost[bCurrentByteIndexHost] = bCurrentByte;
				bCurrentByteIndexHost++;
                
				if(!bSecondByteMode)
					iNumBytesLeftHost--;

                if(iNumBytesLeftHost == 0 && bLongModeNoChecksum)
                {
                    m_bReceivedGoodPacketSinceOpen =true;
                    mMsg.TimeSystem = lCurrentTime;

                    // a zero check sum - this packet is good
                    ProcessRxPacket(lCurrentTime, bTempPacketBufferHost,  iNumBytesTotal);
                    
					// advance the FIFO (permanently)
                    bFoundStartByte=false;
                    bFoundStartFrame=false;
                    m_lReadDataIndexOut=iTempPositionOut;
                    m_lReadDataCount -= (iNumBytesTotal+2);

                }
			}
			else if(bCheckSum && !bLongModeNoChecksum)
			{
				// non-zero check sum means there was an error (ERROR)
				// advance the FIFO one byte and try again on next serial read
			
				m_lReadDataIndexOut++;
				
				if(m_lReadDataIndexOut==NEOVI_READBUFFSIZE)
					m_lReadDataIndexOut = 0;
				
				if(m_lReadDataCount)
					m_lReadDataCount--;
				
				// reset the temp position out counter
				iTempPositionOut = m_lReadDataIndexOut;
				
				// reset the found data flags
				bFoundStartByte = false;
				bFoundStartFrame = false;
				iCount = 0;
				iTempNumBytes =m_lReadDataCount + 1;
			}
			else
			{
				m_bReceivedGoodPacketSinceOpen =true;
				mMsg.TimeSystem = lCurrentTime;
				
				// a zero check sum - this packet is good
				ProcessRxPacket(lCurrentTime, bTempPacketBufferHost,  iNumBytesTotal);
				// advance the FIFO (permanently)
				bFoundStartByte=false;
				bFoundStartFrame=false;
				m_lReadDataIndexOut=iTempPositionOut;
				m_lReadDataCount -= (iNumBytesTotal+2);
				
			}
		}
		else if (bFoundStartFrame)
		{
			// set the start found byte and file pointer
			bCheckSum= bCurrentByte;
			bTempPacketBufferHost[0] = bCurrentByte;
			bFoundStartByte=true;
			iNumBytesLeftHost=(bCurrentByte >> 4) & 0xF;
            if (iNumBytesLeftHost==0)
            {
                bSecondByteMode = true;
                iSecondByteCount=0;
                bLongModeNoChecksum = true;
            }
            else
            {
                bSecondByteMode = false;
                bLongModeNoChecksum = false;
            }
			iNumBytesTotal= iNumBytesLeftHost+1;
			bCurrentByteIndexHost=1;
		}
		else if (bCurrentByte == 0xAA)
		{
			// set the start frame flag
			bFoundStartFrame=true;
			// reset the frame error flag
			bFrameError=false;		
		}   
		else
		{
			// this is a frame error
			if (!bFrameError)
				bFrameError=true;
                
			// ignore all bad bytes
			m_lReadDataIndexOut++;
			if (m_lReadDataIndexOut==NEOVI_READBUFFSIZE)
				m_lReadDataIndexOut=0;
			if (m_lReadDataCount)
				m_lReadDataCount--;
		}
	}       
    
	return true;                                     
}

CoreMiniMsg cicsneoVI::GenCoreMiniMsg(const unsigned short *sof)
{
	CoreMiniMsg stMsg;
	// copy the data into a struct matching memory format (works on intel only)
	memcpy( &stMsg,sof,sizeof(CoreMiniMsg));

	return stMsg;
}



bool cicsneoVI::ProcessRxPacketNeoRed(  unsigned long lCurrentTime,
                                        unsigned char *bPacket,
                                        unsigned long lNumberOfBytes)
{     
     
        // there should always be an even number of bytes.....
        if (lNumberOfBytes & 1)
        {
                return false;
        }

        mMsg.StatusBitField2 = 0;

        switch (bPacket[3])
        {          
        	case NETID_DEVICE:
			case NETID_HSCAN:
            case NETID_HSCAN2:
            case NETID_MSCAN:
            case NETID_HSCAN3:
            case NETID_SWCAN:
            case NETID_SWCAN2:
            case NETID_LSFTCAN:
            case NETID_LSFTCAN2:
            case NETID_HSCAN4:
            case NETID_HSCAN5:
            case NETID_HSCAN6:
            case NETID_HSCAN7:
                    Process3rdGenerationCANRx(lCurrentTime,bPacket,lNumberOfBytes);
                break;       

             default:
                return false;
                break;
        }

	return true;
}

bool cicsneoVI::ProcessRxPacket(unsigned long lCurrentTime, unsigned char *bPacket, unsigned long lNumberOfBytes)
{
	int iCmdByteCount;
	unsigned long iCount;
	bool	bAlreadyOverflown = false;
	bool bOverFlow = false;
	bool	bSyncMsg = false;
	unsigned long	bNumberBytes, lNumBytes = 0, lTimeStampIndex;
	unsigned long	lNumHeaderBytes, lNumISOBytes, lNumJBytes = 0, iNumIFRBytes = 0, lNumJ1708Bytes;
	unsigned long lNumJ1850VPWBytes;
	bool bMessageNetID,bMainPicBuffOver, bKeepRunning=false;
	bool bEndOfLongMessage = false;
	unsigned char * bBytePointer;
	unsigned long lNetwork;
	icsSpyMessageLong stTestMsg;
	int iFirstByte;
	int iChecksum;
	bool bChecksumError;
	unsigned char CheckSum = 0; //J1708
	bool bLongMsgSegment = false;
	short sNum;
    bool netid_already_set = false;
			
	bMainPicBuffOver =false;
			
	// adjust the pointer by the packet offset
	bMessageNetID = true;
	lTimeStampIndex = 1;
    
    lNetwork = bPacket[0]&0xF; 
          
    switch(lNetwork)
	{
        case NETID_RED:

            ProcessRxPacketNeoRed(lCurrentTime, bPacket, lNumberOfBytes);
            bMessageNetID = false;

            break;
    }		
    return true;
}

unsigned int cicsneoVI::RetrieveAppNetID(unsigned int device_net_id)
{
    return m_bNetworkIDs[device_net_id];
}   

bool cicsneoVI::Process3rdGenerationCANRx(    unsigned long lCurrentTime,
                                                unsigned char *bPacket,
                                                unsigned long lNumberOfBytes)
{     
  
    CoreMiniMsg stMsg = GenCoreMiniMsg((const unsigned short*)&bPacket[5]);      
    
	const CoreMiniMsgBitsCAN * stMsgBits = (const CoreMiniMsgBitsCAN*)&stMsg;

	icsSpyMessage mMsg = {0};

    // type of CAN ID
    if(stMsgBits->IDE) // (stMsg.CxTRB0SID.IDE)
    {
            mMsg.StatusBitField = SPY_STATUS_NETWORK_MESSAGE_TYPE + SPY_STATUS_XTD_FRAME;
                // 29 bit ID
            mMsg.ArbIDOrHeader = ((unsigned int) stMsgBits->SID)<<18; // ((unsigned int) stMsg.CxTRB0SID.SID)<<18;
            mMsg.ArbIDOrHeader += stMsgBits->EID << 6;
            mMsg.ArbIDOrHeader += stMsgBits->EID2;
    }
    else
    {

            mMsg.StatusBitField = SPY_STATUS_NETWORK_MESSAGE_TYPE;
            mMsg.ArbIDOrHeader = stMsgBits->SID; //stMsg.CxTRB0SID.SID;
    }

    mMsg.NumberBytesHeader= 4;
    mMsg.NumberBytesData = stMsgBits->DLC; // stMsg.CxTRB0DLC.DLC;

    if(	(stMsgBits->IDE==1 && stMsgBits->RTR==1) ||  
        (stMsgBits->IDE==0 && stMsgBits->SRR==1)	)
    {
            mMsg.StatusBitField = SPY_STATUS_REMOTE_FRAME;
            mMsg.Data[0]= 0;
            mMsg.Data[1]= 0;
            mMsg.Data[2]= 0;
            mMsg.Data[3]= 0;
            mMsg.Data[4]= 0;
            mMsg.Data[5]= 0;
            mMsg.Data[6]= 0;
            mMsg.Data[7]= 0;
    }
    else                                                      
    {
            mMsg.Data[0]= stMsg.CxTRB0D0;
            mMsg.Data[1]= stMsg.CxTRB0D1;
            mMsg.Data[2]= stMsg.CxTRB0D2;
            mMsg.Data[3]= stMsg.CxTRB0D3;
            mMsg.Data[4]= stMsg.CxTRB0D4;
            mMsg.Data[5]= stMsg.CxTRB0D5;
            mMsg.Data[6]= stMsg.CxTRB0D6;
            mMsg.Data[7]= stMsg.CxTRB0D7;
    }

    mMsg.TimeHardware =   stMsg.uiTimeStamp10uS;
    mMsg.TimeHardware2 = stMsg.uiTimeStamp10uSMSB;

    mMsg.NetworkID = (NETID_HSCAN);
    mMsg.Protocol = (SPY_PROTOCOL_CAN);
    
        //  is this a transmit complete report?
    if(stMsgBits->TXMSG) //  (stMsg.CxTRB0EID.TXMSG)
    {
                // status bitfield
                mMsg.StatusBitField += SPY_STATUS_TX_MSG;
                // transmit id
                mMsg.DescriptionID =  stMsg.CxTRB0STAT;
                // tx error bits
                if(stMsgBits->TXError) // (stMsg.CxTRB0EID.TXError) // tx error
                {
                        mMsg.StatusBitField |= SPY_STATUS_GLOBAL_ERR;
                        mMsg.StatusBitField |= SPY_STATUS_UNDEFINED_ERROR;
                }
                if(stMsgBits->TXLostArb) // (if (stMsg.CxTRB0EID.TXLostArb) // tx lost arb
                {
                        mMsg.StatusBitField |= SPY_STATUS_LOST_ARBITRATION;
                }
                if(stMsgBits->TXAborted) // (if (stMsg.CxTRB0EID.TXAborted) // tx aborted
                {
                        mMsg.StatusBitField |= SPY_STATUS_GLOBAL_ERR;
                        mMsg.StatusBitField |= SPY_STATUS_VSI_TX_UNDERRUN;
                }
    }

    if(stMsgBits->RB1)
    {
                //frame is an error frame, data bytes are already copied

                mMsg.StatusBitField |= SPY_STATUS_GLOBAL_ERR;  
                mMsg.StatusBitField2 = SPY_STATUS2_ERROR_FRAME;

                mMsg.ArbIDOrHeader =0;
                mMsg.NumberBytesHeader = 0;

                if(mMsg.Data[3] & 1)
                {
                        if(mMsg.Data[3] & 2)
                                mMsg.StatusBitField |= SPY_STATUS_BUS_SHORTED_PLUS;
                        else
                                mMsg.StatusBitField |= SPY_STATUS_BUS_RECOVERED;
                }
                else    
                        mMsg.StatusBitField |= SPY_STATUS_CAN_BUS_OFF; 

                mMsg.NumberBytesData = 3;

                mMsg.DescriptionID = 0;
        }

        mMsg.NetworkID = RetrieveAppNetID(bPacket[3]);    
        mMsg.Protocol = SPY_PROTOCOL_CAN;
        
        if(mMsg.ArbIDOrHeader > 0)
            AddMsgToRXQueue(mMsg, true);
                
        return true;
}

bool cicsneoVI::CloseDevice(void)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return true;
        
    m_pRxThread->End();

    unsigned char go_offline[] = {
        0xAA, 0x2B, 0x07, 0x00, 0xCE, 0xAA, 0x1B, 0xC3, 0x22, 0xAA, 0x2B, 0x07, 
        0x00, 0xCE, 0xAA, 0x30, 0x02, 0x06, 0x04, 0xC4
    };
    ftdi_write_data(m_ftdic, go_offline, sizeof(go_offline));
    		
	ftdi_usb_close(m_ftdic);
    ftdi_usb_reset(m_ftdic);
   
	m_bDeviceOpen = false;
    m_bneoShutdown = true;

	return true;
}

bool cicsneoVI::SetLatencyTimer(unsigned char latency)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return false;
		
	iRetVal = ftdi_set_latency_timer(m_ftdic, latency);

	return iRetVal == 0 ? true : false;
}

bool cicsneoVI::PurgeBuffers(void)
{
	int iRetVal;
	
	if(!m_bDeviceOpen)
		return false;

	iRetVal = ftdi_usb_purge_buffers(m_ftdic);

	return iRetVal == 0;
}

int cicsneoVI::GetTimeStampForMsg(icsSpyMessage *msg, double *pTimeStamp)
{
	if(pTimeStamp)
		*pTimeStamp = (NEOVI_RED_TIMESTAMP_2_25NS * ((double)msg->TimeHardware2)) + (NEOVI_RED_TIMESTAMP_1_25NS * ((double)msg->TimeHardware));
}
