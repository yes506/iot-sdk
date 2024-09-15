/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

#ifdef HAVE_THREAD

#ifndef __THREAD__
#define __THREAD__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(WIN32) || defined(WIN64)
	#include <Windows.h>
	#define thread_type		HANDLE
	#define thread_id_type	DWORD
	#define thread_return_type	DWORD
	#define thread_fn		LPTHREAD_START_ROUTINE

	#define mutex_type		HANDLE
	#define cond_type		HANDLE
	#define sem_type		HANDLE

#else
	#include <pthread.h>
	#include <semaphore.h>
	#define thread_type		pthread_t
	#define thread_id_type	pthread_t
	#define thread_return_type void*
	typedef thread_return_type (*thread_fn)(void*);
	#define mutex_type pthread_mutex_t*
	typedef struct { pthread_cond_t cond; pthread_mutex_t mutex; } cond_type_struct;
	typedef cond_type_struct *cond_type;
	typedef sem_t *sem_type;

	cond_type Thread_create_cond();
	int Thread_signal_cond(cond_type);
	int Thread_wait_cond(cond_type condvar, int timeout);
	int Thread_destroy_cond(cond_type);

#define WINAPI
#endif

thread_id_type thread_getid();

mutex_type thread_create_mutex();
int thread_lock_mutex(mutex_type mutex);
int thread_unlock_mutex(mutex_type mutex);
void thread_destroy_mutex(mutex_type mutex);

#ifdef __cplusplus
}
#endif

#endif


#endif // HAVE_THREAD
