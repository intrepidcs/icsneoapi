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

#include <stdio.h>
#include <ftdi.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "icsnVC40.h"

class CFTDILibLinux
{
private:
	bool m_bDeviceOpen;
	struct ftdi_context m_ftdic;
	
public:

	CFTDILibLinux();
	~CFTDILibLinux();
	
	
	bool OpenNeoVI(NeoDevice *pDevice);
	bool CloseDevice(void);	
	bool SetLatencyTimer(unsigned char latency);
	bool PurgeBuffers(void);
	
	int Write(unsigned char *buf, int size);
	int Read(unsigned char *buf, int size);
	
	void Reset(void);

	//always pass in 255 NeoDevice structures
	static int FindneoVIs(NeoDevice *pDevice, unsigned long DevType);
	static int GetPID(unsigned long DevType);
	
};
