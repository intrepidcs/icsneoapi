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

#include "OSAbstraction.h"
#include "OThread.h"
#include <stdio.h>

static void *CThreadRunFunction(void *lParam)
{
  int iStatus;
  
  OThread *pThread = (OThread *)lParam;

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  pThread->Run();
  
  pthread_exit((void *)&iStatus);
 
 return 0;
}




OThread::OThread(void)
{
 m_pParam = 0;
 m_bRunning = false;
  

 
 m_bShutdown = 0;
 m_thread = 0;



}

OThread::~OThread(void)
{

 if(m_bRunning)
  End();



}


bool OThread::Start(void *pParam, int Priority, int affinity)
{
 if(m_bRunning)
  return true;

 m_pParam = pParam;


 m_bShutdown = false;
 int max_priority;
 int min_priority;
 int mid_priority;
 int min_max_diff;
 int min_max_diff_quarter;
 
 struct sched_param SchedVal;
 int iRetVal;
 pthread_attr_t attr;
  
 pthread_attr_init(&attr);
 
 iRetVal = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
 
 if(iRetVal != 0)
 	return false;
 
 max_priority = sched_get_priority_max(SCHED_OTHER);
 min_priority = sched_get_priority_min(SCHED_OTHER);
 
 min_max_diff = max_priority - min_priority;
 mid_priority = (min_max_diff / 2) + min_priority;
 min_max_diff_quarter = min_max_diff / 4;
  
 switch(Priority)
 {
 	case THREAD_PRIORITY_ABOVE_NORMAL:
 	
 		SchedVal.sched_priority = max_priority - min_max_diff_quarter;
 	
 		break;
 	
 	case THREAD_PRIORITY_BELOW_NORMAL:
 	
 	    SchedVal.sched_priority = min_priority + min_max_diff_quarter;
 	    
 		break;
 	
 	case THREAD_PRIORITY_HIGHEST:
 	case THREAD_PRIORITY_TIME_CRITICAL:
 	
 		SchedVal.sched_priority = max_priority;
 	
 		break;
 		  
 	case THREAD_PRIORITY_IDLE:    
 	
 		SchedVal.sched_priority = min_priority;
 	
 		break;
 	
 	case THREAD_PRIORITY_NORMAL:
  	default:
 	
 		SchedVal.sched_priority = mid_priority;
 	 	
 		break; 	
 }

 if(SchedVal.sched_priority < min_priority)
 	SchedVal.sched_priority = min_priority;
 
 if(SchedVal.sched_priority > max_priority)
 	SchedVal.sched_priority = max_priority;
 	
 iRetVal = pthread_attr_setschedparam(&attr, &SchedVal);
 
 if(iRetVal != 0)
 	return false;
  
 iRetVal = pthread_create(&m_thread, NULL, CThreadRunFunction, this);

 if(iRetVal != 0)
 	return false;

 m_bRunning = true;

 return true;


}

void OThread::End(void)
{
	
	int iRetVal;
	void *Thread_Result;
	
	if(!m_thread)
		return;
	
	m_cs.Lock();
	m_bShutdown = true;
	m_cs.Unlock();
	
	OSleep(100);
	

		pthread_cancel(m_thread);

	

	iRetVal = pthread_join(m_thread, &Thread_Result);
	
		
	m_thread = 0;
	
	m_bRunning = false;

}


bool OThread::shut_down(unsigned long dwTimeout)
{
 bool bRetVal = false;
 	
    m_cs.Lock();
    bRetVal = m_bShutdown;
    m_cs.Unlock();
    
    if(bRetVal)
    	return true;
    	
    OSleep(dwTimeout);
	return false;

 return bRetVal;
}



