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

#ifndef VSPY_DATA_HEADER_FILE
#define VSPY_DATA_HEADER_FILE
                     

#include "icsSpyDataCStyle.h"

/*#include "icsSpyDataCommon.h"
struct _icsSpyMessage;
typedef struct _icsSpyMessage icsSpyMessage;
struct _icsSpyMessageLong;
typedef struct _icsSpyMessageLong icsSpyMessageLong;
struct _icsSpyMessageJ1850;
typedef struct _icsSpyMessageJ1850 icsSpyMessageJ1850;  */

class CSpyMessage
{
	public:
			CSpyMessage();
			CSpyMessage(const icsSpyMessage *);
			CSpyMessage(const icsSpyMessage &);
                        CSpyMessage(const icsSpyMessageLong &);
                        CSpyMessage(const CSpyMessage &);

                        ~CSpyMessage();
			icsSpyMessageJ1850 * ToRawJ1850();
			icsSpyMessage * ToRaw();
			const icsSpyMessageJ1850 * ToRawJ1850() const;
			const icsSpyMessage * ToRaw() const;
                        void Clear();
                        CSpyMessage & operator = (const CSpyMessage &);
                        CSpyMessage & operator = (const icsSpyMessage &);
                        CSpyMessage & operator = (const icsSpyMessageJ1850 & msg);
                        CSpyMessage & operator = (const icsSpyMessageLong & msg);

                        operator icsSpyMessage();
                        operator icsSpyMessageLong();
                        operator icsSpyMessageJ1850();
						operator const icsSpyMessage() const;
                        operator const icsSpyMessageLong() const;
                        operator const icsSpyMessageJ1850() const;



                        //public interface
                        
                        // icsSpyMessage
                        unsigned long &StatusBitField;
                        unsigned long &StatusBitField2;
                        unsigned long &TimeHardware;
                        unsigned long &TimeHardware2;
                        unsigned long &TimeSystem;
                        unsigned long &TimeSystem2;
                        unsigned char &TimeStampHardwareID;
                        unsigned char &TimeStampSystemID;
                        unsigned char &NetworkID;
                        unsigned char &NodeID;
                        unsigned char &Protocol;
                        unsigned char &MessagePieceID;
                        unsigned char &ExtraDataPtrEnabled;
                        unsigned char &NumberBytesHeader;
                        unsigned char &NumberBytesData;
                        short &DescriptionID;
                        long &ArbIDOrHeader;
                        unsigned char *Data;
                        unsigned char *AckBytes;
                        unsigned int &ExtraDataPtr;
                        unsigned char &MiscData;
                        // icsSpyMessageJ1850
                        unsigned char *Header;
                        //icsSpyMessageLong
                        unsigned long &DataMsb;
	                unsigned long &DataLsb;

                        unsigned long &StatusBitField3;
	    	        unsigned long &StatusBitField4;

	private:
                void SanityCheck();

		icsSpyMessage raw;

};

typedef CSpyMessage* PSpyMessage;

#endif

