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

#ifndef VSPY_DATA_C_STYLE_HEADER_FILE
#define VSPY_DATA_C_STYLE_HEADER_FILE

#include "icsSpyDataCommon.h"
/*
Public Type icsSpyMessage
    StatusBitField As Long '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long ' 4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long ' 4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte '1
    NumberBytesHeader As Byte ' 1
    NumberBytesData As Byte ' 1
    DescriptionID As Integer ' 2
    ArbIDOrHeader As Long    '// Holds (up to 3 byte 1850 header or 29 bit CAN header) '4
    Data(1 To 8) As Byte '8
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single ' 4
    MiscData As Byte
  
End Type */

typedef struct _icsSpyMessage // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;
	unsigned char NetworkID;		// 1
	unsigned char NodeID;
	unsigned char Protocol;
	unsigned char MessagePieceID;	// 1
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned char Data[8];
    union
    {
    	struct
        {
	    	unsigned long StatusBitField3;
	    	unsigned long StatusBitField4;
        };
	unsigned char AckBytes[8];
    };
    unsigned int ExtraDataPtr;
	unsigned char MiscData;
    }  icsSpyMessage;

/*
Public Type icsSpyMessageLong
    StatusBitField As Long ' 4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte ' 1
    NumberBytesHeader As Byte '
    NumberBytesData As Byte '2
    DescriptionID As Integer '2
    ArbIDOrHeader As Long    '// Holds (up to 3 byte 1850 header or 29 bit CAN header)
    DataMsb As Long
    DataLsb As Long
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single
    MiscData As Byte
    
End Type*/

typedef struct _icsSpyMessageLong// matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;	// 1
	unsigned char NetworkID;		// 1
	unsigned char NodeID;			// 1
	unsigned char Protocol;			// 1
	unsigned char MessagePieceID;	// 1
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned long DataMsb;
	unsigned long DataLsb;
	unsigned char AckBytes[8];
    unsigned int ExtraDataPtr;
	unsigned char MiscData;

        /*  THIS IS NOT ALLOWED IN C - DAVER
        icsSpyMessageLong_struct() {
                ExtraDataPtrEnabled = false;
                NumberBytesData = 0;
                ExtraDataPtr = 0;
        } */

    }  icsSpyMessageLong;



/*
Public Type icsSpyMessageJ1850
    StatusBitField As Long '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long '4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1 new
    ColorID As Byte ' 1
    NumberBytesHeader As Byte '1
    NumberBytesData As Byte '1
    DescriptionID As Integer '2
    Header(1 To 4) As Byte  '4  '// Holds (up to 3 byte 1850 header or 29 bit CAN header)
    Data(1 To 8) As Byte '8
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single '4
    MiscData As Byte
End Type */

typedef struct _icsSpyMessageJ1850// matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;	// 1
	unsigned char NetworkID;		// 1
	unsigned char NodeID;			// 1
	unsigned char Protocol;			// 1
	unsigned char MessagePieceID;	// 1
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    unsigned char Header[4];		// 4
    unsigned char Data[8];
    union
    {
    	struct
        {
	    	unsigned long StatusBitField3;
	    	unsigned long StatusBitField4;
        };
	unsigned char AckBytes[8];
    };			
    unsigned int ExtraDataPtr;
	unsigned char MiscData;
    }  icsSpyMessageJ1850;





#endif

