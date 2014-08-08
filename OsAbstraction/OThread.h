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
#ifndef _OTHREAD_H
#define _OTHREAD_H

#include "OSAbstraction.h"


 
 #include <pthread.h>
 #include "OCriticalSection.h"

 #define THREAD_PRIORITY_ABOVE_NORMAL   1
 #define THREAD_PRIORITY_BELOW_NORMAL   -1
 #define THREAD_PRIORITY_HIGHEST        2
 #define THREAD_PRIORITY_IDLE           -15
 #define THREAD_PRIORITY_NORMAL         0
 #define THREAD_PRIORITY_TIME_CRITICAL  15


class OThread
{
 bool m_bRunning;
 


 pthread_t m_thread;
 OCriticalSection m_cs;
 bool m_bShutdown;

 
protected:
	
	void *m_pParam;
	
	bool shut_down(unsigned long dwTimeout); //called by the Run function in the child class

public:
	OThread(void);
	~OThread(void);

	virtual unsigned long Run() = 0;

  //**** Available Windows thread priority selections
  //THREAD_PRIORITY_ABOVE_NORMAL
  //THREAD_PRIORITY_BELOW_NORMAL
  //THREAD_PRIORITY_HIGHEST
  //THREAD_PRIORITY_IDLE
  //THREAD_PRIORITY_NORMAL
  //THREAD_PRIORITY_TIME_CRITICAL

  bool Start(void *pParam, int ThreadPriority = THREAD_PRIORITY_NORMAL, int affinity = 0);

	virtual void End(void);
	
	bool IsRunning() {return m_bRunning;}		
};

#endif
