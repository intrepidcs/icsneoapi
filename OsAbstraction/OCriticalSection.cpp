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

#include "OCriticalSection.h"  
#include <stdio.h>  

	
#include <pthread.h> 	
#include <semaphore.h> 


#if defined(QNX_OS) || defined(ANDROID)
//On some platforms, only one or the other is defined, but they both mean the same thing
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

OCriticalSection::OCriticalSection(void) 
{ 
	pthread_mutexattr_init(&cs_mutex_attr); 	
	pthread_mutexattr_settype(&cs_mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP); 
	pthread_mutex_init(&cs_mutex, &cs_mutex_attr); 

}  

OCriticalSection::~OCriticalSection(void) 
{ 
	pthread_mutex_destroy(&cs_mutex); 
	pthread_mutexattr_destroy(&cs_mutex_attr); 	
	
} 
void OCriticalSection::Lock() 
{ 
    
	pthread_mutex_lock(&cs_mutex); 
  
}  

void OCriticalSection::Unlock() 
{ 
 	
	pthread_mutex_unlock(&cs_mutex); 
 
} 


