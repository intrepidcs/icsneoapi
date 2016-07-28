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

/////////////////////////// Linux implementation ///////////////////////////// 
#include <time.h> 


OEvent::OEvent(const char *Name, bool bManualReset, bool bInitialState)
{	 
	m_bEventIsSignaled = bInitialState; 
	m_bManualReset = bManualReset; 	
	pthread_cond_init(&cond, 0); 
	pthread_mutex_init(&mutex, 0); 
} 

OEvent::~OEvent()
{   
	pthread_cond_destroy(&cond); 
	pthread_mutex_destroy(&mutex); 
}   

void OEvent::Set(void)
{    
	pthread_mutex_lock(&mutex);
	m_bEventIsSignaled = true;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex); 
}  

#include <time.h>

unsigned long OEvent::Wait(unsigned long dwMilliseconds)
{ 
	int rc = 0; 
	pthread_mutex_lock(&mutex); 	
	unsigned long nsecs; 
	if(!m_bEventIsSignaled)
	{ 	
		struct timespec ts; 
		int seconds = dwMilliseconds / 1000; 	
		dwMilliseconds %= 1000; 	
		clock_gettime(CLOCK_REALTIME, &ts); 
		ts.tv_sec += seconds; 	
		nsecs = dwMilliseconds * 1000000; 	

		if(nsecs + ts.tv_nsec >= 1000000000) 	
		{ 		
			ts.tv_sec++; 
			nsecs = nsecs - 1000000000; 	
			ts.tv_nsec += nsecs; 	
		} 	
		else		
			ts.tv_nsec += dwMilliseconds * 1000000; 
		
		rc = pthread_cond_timedwait(&cond, &mutex, &ts);
		if(!m_bManualReset && (rc == 0)) //accorind to CreateEventA, If this parameter is FALSE, the function creates an auto-reset event object, and system automatically resets the event state to nonsignaled after a single waiting thread has been released.
			m_bEventIsSignaled = false;
	} 
	pthread_mutex_unlock(&mutex); 	

	return rc; 

}  

void OEvent::Reset(void)
{ 
	pthread_mutex_lock(&mutex); 
	m_bEventIsSignaled = false; 
	pthread_mutex_unlock(&mutex);

} 


bool OEvent::IsSet(unsigned long dwMilliseconds)
{ 
	unsigned long RetVal;
	
	
	
	RetVal = Wait(dwMilliseconds);
	
	switch(RetVal)
	{
		case WAIT_OBJECT_0:
		/* event was signaled, return true*/
			return true;
	
	    /*everything else falls to return false*/
			break;
	}
	
	return false;
} 



