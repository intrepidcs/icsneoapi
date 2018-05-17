/*
Copyright (c) 2016 Intrepid Control Systems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intrepid Control Systems nor the
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


#ifndef _COREMINI_HEADER_
#define _COREMINI_HEADER_

#include "icsnVC40.h"


#ifdef linux

#include <inttypes.h>
#define __int64  int64_t

#endif 


#ifdef __C30  
	// dspic compiler///why is this?
	/*
	#define icscm_uint16    unsigned int
	#define icscm_uint32    unsigned long
	#define icscm_uint8     unsigned char
    #define icscm_float32    float
    */
    #include "icsglb.h"
#ifndef NOT_RED_BASE    
    #include "serial2.h"
    #include "PWRMan.h"
#endif
        //#include "..\neoVIred\CoreMiniMap.h"
        //#include "..\neoVIred\neovi.h"    
        #include "led.h"    
        //#include "..\neoVIred\coremini_fs.h"

#else 
       #define POINTERS_ARE_32BITS
       
	//#define int8 unsigned char
	// #define int16 unsigned short

	#define icscm_int16     short
	#define icscm_uint16    unsigned short
	#define icscm_uint32    unsigned int
	#define icscm_int32     int
	#define icscm_uint8    unsigned char
    #define icscm_int64    __int64


    #define LOG_DATA_SIZE           512*2048

	#pragma pack(push)
	#pragma pack(2)

#endif

#ifdef linux

#define __int65 int64_t

#endif


#ifdef USING_STUDIO_8
	#define VS_MODIFIER __declspec(align(2))

       #define NO_INNER_BITFIELDS   //turn off the inner bitfields because VS C++ allocates a multiple of 4 bytes per bitfield
#else
	#define VS_MODIFIER
#endif

#define MAX_DATA_MEDIUM_SECTOR_SIZE_WORDS	(264/2) // max size of sector used per read/write of data medium



// version of the core mini exec
#define COREMINI_VERSION         18
// core mini file type
#define COREMINI_FILETYPE        0x0907



#define  COREMINI_PREBUFFER_MSGS_MAX    32768

#define   COREMINI_KEY                  {0x12437744,0x2384000,0x22173156,0x19792123};


#ifdef _USE_INTERNAL_RAM_FOR_USB_OUTGOING_BUFFER_
// fro debugging we have 8K only for coremini
// RED will crash if larger file was loaded
#define COREMINI_HEAP_SIZE (512*3) // over 512*3 
#else
#define COREMINI_HEAP_SIZE (16u*1024u)
#endif
#define COREMINI_FLASH_LOC	(64u*1024u)
extern icscm_uint8 ucharConfigurationArray[COREMINI_HEAP_SIZE];

// core mini network index type   
// when ExtendedNetworkIndexBit2 = 0
// when ExtendedNetworkIndexBit = 0
#define COREMINI_NETWORK_HSCAN1                  0            // first channel of dual wire CAN  (internal CAN controller)
#define COREMINI_NETWORK_MSCAN1                  1            // second channel of dual wire CAN  (internal CAN controller)
#define COREMINI_NETWORK_LIN1                    2            // first channel of LIN
#define COREMINI_NETWORK_LIN2                    3            // second channel of LIN
#define COREMINI_NETWORK_VIRTUAL                 4            // analog inputs, virtual data
#define COREMINI_NETWORK_HSCAN2                  5            // third channel of dual wire CAN  (MCP2515 based controller)
#define COREMINI_NETWORK_LSFTCAN1                6            // first channel of LSFT CAN (can be internal (for RED) or external (FIRE))
#define COREMINI_NETWORK_SWCAN1                  7            // first channel of SWCAN (can be internal (for RED) or external (FIRE))
                                    
// when ExtendedNetworkIndexBit2 = 0
// when ExtendedNetworkIndexBit = 1
#define COREMINI_NETWORK_OTHER_HSCAN3            (0+8)            // fourth channel of dual wire CAN  (MCP2515 based controller)
#define COREMINI_NETWORK_OTHER_GMCGI             (1+8)            // GMCGI Bus
#define COREMINI_NETWORK_OTHER_J1850_VPW         (2+8)            // first channel of J1850 VPW
#define COREMINI_NETWORK_OTHER_LIN3              (3+8)            // third channel of LIN  (external device)
#define COREMINI_NETWORK_OTHER_LIN4              (4+8)            // fourth channel of LIN  (external device)
#define COREMINI_NETWORK_OTHER_J1708             (5+8)            // first channel of J1708
#define COREMINI_NETWORK_OTHER_SWCAN2            (6+8)            // second channel of single wire can (neoVI ECU)
#define COREMINI_NETWORK_OTHER_LSFTCAN2          (7+8)            // second channel of LSFT  can (neoVI ECU)
                                    
// when ExtendedNetworkIndexBit2 = 1 (must be non CAN native networks)
// when ExtendedNetworkIndexBit = 0 
#define COREMINI_NETWORK_OTHER_KLINE1              (0+16)           // first channel of KLINE  (external device)
#define COREMINI_NETWORK_OTHER_KLINE2              (1+16)           // second channel of KLINE  (external device)
#define COREMINI_NETWORK_OTHER_KLINE3              (2+16)           // third channel of KLINE  (external device)
#define COREMINI_NETWORK_OTHER_KLINE4              (3+16)    		// fourth channel of KLINE  (external device)
#define COREMINI_NETWORK_OTHER_RS232               (4+16)       	// RS232
#define COREMINI_NETWORK_OTHER_UART                (5+16)       	// UART    
#define COREMINI_NETWORK_OTHER_UART2               (6+16)       	// UART2



#define COREMINI_NUM_NETWORKS   21           // CHANGING THIS VALUE WILL BREAK COREMINI VERSIONING

#define COREMINI_NET_SWAP_MODE_NONE			0
#define COREMINI_NET_SWAP_MODE_HSCAN_MSCAN	1

#define GetCoreMiniNetID(m)\
	(	(	((CoreMiniMsgBitsGeneric*)(m))->NETWORKINDEX						) 	|\
		(	((CoreMiniMsgBitsGeneric*)(m))->ExtendedNetworkIndexBit  ? 0x08 : 0	) 	|\
		(	((CoreMiniMsgBitsGeneric*)(m))->ExtendedNetworkIndexBit2 ? 0x10 : 0	)	)
			
#define SetCoreMiniNetID(m,id) do{\
	((CoreMiniMsgBitsGeneric*)(m))->NETWORKINDEX = id&0x7;\
	((CoreMiniMsgBitsGeneric*)(m))->ExtendedNetworkIndexBit  = (id&0x08) ? 1 : 0;\
	((CoreMiniMsgBitsGeneric*)(m))->ExtendedNetworkIndexBit2 = (id&0x10) ? 1 : 0;\
	}while(0);
	
#define CopyCoreMiniNetID(m1,m2) do{\
	((CoreMiniMsgBitsGeneric*)(m1))->NETWORKINDEX =					\
	((CoreMiniMsgBitsGeneric*)(m2))->NETWORKINDEX;					\
	((CoreMiniMsgBitsGeneric*)(m1))->ExtendedNetworkIndexBit  = 	\
	((CoreMiniMsgBitsGeneric*)(m2))->ExtendedNetworkIndexBit;		\
	((CoreMiniMsgBitsGeneric*)(m1))->ExtendedNetworkIndexBit2 =		\
	((CoreMiniMsgBitsGeneric*)(m2))->ExtendedNetworkIndexBit2;		\
	}while(0);
	
typedef union _CM_LARGE_INTEGER {
    struct {
        icscm_uint32 LowPart;
        icscm_uint32  HighPart;
    };
    icscm_int64 QuadPart;
} CM_LARGE_INTEGER;

// this defines the flexible value structure for expressions
typedef union _CM_FIXED_POINT_VALUE {
    struct {
        union {
                icscm_uint32  ValueFractionPart;
                struct {
                   icscm_int16    ValueInt16FractionLSB;
                   icscm_int16    ValueInt16FractionMSB;
                };
              };
        union {
            icscm_int32   ValueInt32;
            struct {
                   icscm_int16    ValueInt16;
                   icscm_int16    ValueInt16PartMSB;
            };
            struct {
                   icscm_uint8   ValueInt8;
                   icscm_uint8   ValueInt8PartMSB;
                   icscm_int16   ValueInt8Part16MSB;
            };
        };
    };
    icscm_int64 ValueLarge;
} CM_FIXED_POINT_VALUE;


#define CM_FIXED_POINT_TO_DOUBLEVALUE   0.00000000023283064365386962890625
#define CM_DOUBLEVALUE_TO_FIXED_POINT   4294967296.0


// the number of fixed point counter per 25 nano second ticks
#define NATIVE_TIMESTAMP_RESOLUTION_25NS    0x6666666                  // 0.025 represented as CM_FIXED_POINT_VALUE
#define NATIVE_TIMESTAMP_RESOLUTION_10US    0xA00000000                // 10 represented as CM_FIXED_POINT_VALUE


#define SECOND_TIMESTAMP_RESOLUTION_32_BIT_25NS    0x6B                  	// 0.000000025 represented as CM_FIXED_POINT_VALUE
#define SECOND_TIMESTAMP_RESOLUTION_64_BIT_25NS    0x6B5FCA6AF2ULL            // 107.37418239982798695564270019531 represented as CM_FIXED_POINT_VALUE


#define NATIVE_TIMESTAMP_COUNTS_TO_US          40

// this struct declares a pointer type so that both 16bit and 32 bit pointer architectures can
// share the core mini executable file
#ifdef POINTERS_ARE_32BITS
        typedef struct _CM_POINTER {
        union {
                void * pData;
                icscm_uint32 iIndex;
                icscm_int32   ValueInt32;
                };
        }  CM_POINTER;
#else
                typedef struct _CM_POINTER{
        union {
                struct {
                void * pData;
                unsigned :16;
                };
                struct {
                        icscm_uint16 iIndex;
                        unsigned :16;
                };
                icscm_int32   ValueInt32;
             };
        } CM_POINTER;
#endif


// coremini extensions
#define CM_CALLBACKTYPE_APP_SIGNAL				0
#define CM_CALLBACKTYPE_MESSAGE					1

#define CM_GETSET_SET_APP_SIGNAL				0
#define CM_GETSET_GET_APP_SIGNAL				1

typedef struct  _CoreMiniMsgBitsCAN
{
	// CxTRB0SID
	unsigned IDE:1;
	unsigned SRR:1;
	unsigned SID:11;
	unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS

	// CxTRB0EID
	unsigned EID:12;
	unsigned TXMSG:1;
	unsigned TXAborted:1;
	unsigned TXLostArb:1;
	unsigned TXError:1;

	//CxTRB0DLC
	unsigned DLC:4;
	unsigned RB0:1; // ExtendedNetworkIndexBit2 must be 0 for all CAN
	unsigned IVRIF:1;
	unsigned HVEnable:1; // must be cleared before passing into CAN driver
	unsigned ExtendedNetworkIndexBit:1;//DO NOT CLOBBER THIS
	unsigned RB1:1;
	unsigned RTR:1;
	unsigned EID2:6;

} CoreMiniMsgBitsCAN;

typedef struct _CoreMiniMsgBitsCANFD
{
	// CxTRB0SID
	unsigned IDE : 1;
	unsigned SRR : 1;
	unsigned SID : 11;
	unsigned EDL : 1;
	unsigned BRS : 1;
	unsigned ESI : 1;

	// CxTRB0EID
	unsigned EID : 12;
	unsigned TXMSG : 1;
	unsigned TXAborted : 1;
	unsigned TXLostArb : 1;
	unsigned TXError : 1;

	//CxTRB0DLC
	unsigned DLC : 4;
	unsigned RB0 : 1;// ExtendedNetworkIndexBit2 must be 0 for all CAN
	unsigned IVRIF : 1;
	unsigned HVEnable : 1;// must be cleared before passing into CAN driver
	unsigned ExtendedNetworkIndexBit : 1;//DO NOT CLOBBER THIS
	unsigned RB1 : 1;
	unsigned RTR : 1;
	unsigned EID2 : 6;

} CoreMiniMsgBitsCANFD;

typedef VS_MODIFIER struct  _CoreMiniMsg 
{
	union 
	{
		icscm_uint16 CxTRB0SID16;

#ifndef NO_INNER_BITFIELDS
		struct {
			unsigned IDE:1;
			unsigned SRR:1;
			unsigned SID:11;
			unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS
		} CxTRB0SID;
        
		struct {
			unsigned ErrRxOnlyBreak:1;
			unsigned ErrRxOnlyBreakSync:1;
			unsigned ID:11;
			unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS
		} CxLIN3;
		
		struct {
			icscm_uint8 D8;
			unsigned options:4;
			unsigned TXMSG:1;
			unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS
		} C1xJ1850;
        
		struct {
			icscm_uint8 D8;
			unsigned options:4;
			unsigned TXMSG:1;
			unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS
		} C1xISO;
        
		struct {
			icscm_uint8 D8;
			unsigned options:4;
			unsigned TXMSG:1;
			unsigned NETWORKINDEX:3;//DO NOT CLOBBER THIS
		} C1xJ1708;
#endif

	};


	union 
	{
		icscm_uint16 CxTRB0EID16;

#ifndef NO_INNER_BITFIELDS
		struct {
			unsigned EID:12;
			unsigned TXMSG:1;
			unsigned TXAborted:1;
			unsigned TXLostArb:1;
			unsigned TXError:1;
		} CxTRB0EID;
		
		struct {
			unsigned char LINByte9;
			unsigned ErrTxRxMismatch:1;
				unsigned TxChkSumEnhanced:1;
				unsigned TXMaster:1;
				unsigned TXSlave:1;
				unsigned ErrRxBreakNot0:1;
				unsigned ErrRxBreakTooShort:1;
				unsigned ErrRxSyncNot55:1;
				unsigned ErrRxDataGreater8:1;
		} CxLIN;
		
		struct {
			icscm_uint8 D9;
			icscm_uint8 D10;
		} C2xJ1850;
        
		struct {
			icscm_uint8 D9;
			icscm_uint8 D10;
		} C2xISO;
        
		struct {
			icscm_uint8 D9;
			icscm_uint8 D10;
		} C2xJ1708;
#endif
	};

	union 
	{
		// For use by CAN
		icscm_uint16 CxTRB0DLC16;

#ifndef NO_INNER_BITFIELDS
		struct {
			unsigned DLC:4;
			unsigned RB0:1;
			unsigned IVRIF:1;
			unsigned HVEnable:1; // must be cleared before passing into CAN driver
			unsigned ExtendedNetworkIndexBit:1;//DO NOT CLOBBER THIS
			unsigned RB1:1;
			unsigned RTR:1;
			unsigned EID:6;
		} CxTRB0DLC;
		
		struct {
			unsigned len:4;
	        unsigned ExtendedNetworkIndexBit2:1;//DO NOT CLOBBER THIS
			unsigned UpdateSlaveOnce:1;
			unsigned HasUpdatedSlaveOnce:1;
			unsigned ExtendedNetworkIndexBit:1;//DO NOT CLOBBER THIS
			unsigned :8;
		} CxLIN2;
	    
		// For use by JVPW
		struct {
                        unsigned len:4;
                        unsigned ExtendedNetworkIndexBit2:1;//DO NOT CLOBBER THIS
                        unsigned just_tx_timestamp:1;
                        unsigned first_seg:1;
                        unsigned ExtendedNetworkIndexBit:1; // do not clobber ExtendedNetworkIndexBit
                        icscm_uint8 D11;
		} C3xJ1850;
                         
		// For use by the ISO/KEYWORD
		struct {
			unsigned len:4;   
	                unsigned ExtendedNetworkIndexBit2:1;//DO NOT CLOBBER THIS
			unsigned FRM:1;
			unsigned INIT:1;
			unsigned ExtendedNetworkIndexBit:1; // do not clobber ExtendedNetworkIndexBit  
			unsigned D11:8;
		} C3xISO;
		
		struct {
			unsigned len:4;  
			unsigned ExtendedNetworkIndexBit2:1;//DO NOT CLOBBER THIS
			unsigned FRM:1;
			unsigned :1; 
			unsigned ExtendedNetworkIndexBit:1; // do not clobber ExtendedNetworkIndexBit  
			unsigned pri:8;
		} C3xJ1708;
#endif
	};

	union 
	{
		struct {
			unsigned char	CxTRB0D0;
			unsigned char	CxTRB0D1;
		};
		icscm_uint16    uiCxTRB0D0_D1;
	};

	unsigned char	CxTRB0D2;
	unsigned char	CxTRB0D3;
	unsigned char	CxTRB0D4;
	unsigned char	CxTRB0D5;
	unsigned char	CxTRB0D6;
	unsigned char	CxTRB0D7;

	union 
	{
		icscm_uint16	CxTRB0STAT;
		icscm_uint16	J1850_TX_ID;
	};

	union 
	{
		struct {
		icscm_uint32    uiTimeStamp10uS;
		icscm_uint32    uiTimeStamp10uSMSB;
		};
		icscm_int64     uiTimeStampLarge;
	};
} CoreMiniMsg;

typedef VS_MODIFIER struct _CoreMiniMsgExtendedHdr
{
	CoreMiniMsg stMsg;
	icscm_uint16 NetworkID;
	icscm_uint16 Length;
} CoreMiniMsgExtendedHdr;

#ifdef __C30

#else 
	// win32 types
	#define icscm_uint16    unsigned short
	#define icscm_uint32    unsigned int
	#define icscm_uint8    unsigned char

	#pragma pack(pop)
#endif


#endif



