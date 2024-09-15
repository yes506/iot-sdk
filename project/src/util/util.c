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

#if 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#if defined(WIN32)
#include <synchapi.h>	/* void Sleep(DWORD dwMilliseconds); */
#include <sys/timeb.h>
#include <time.h>
int gettimeofday (struct timeval *tv, void* tz)
{
    struct _timeb timebuffer;
    _ftime_s(&timebuffer);
    tv->tv_sec = timebuffer.time;
    tv->tv_usec = timebuffer.millitm * 1000;    
    return (0);
} 
#else
#include <sys/time.h>
#include <time.h>
#endif


static struct timeval g_time_of_day;

long im_util_gettimeofday_as_sec()
{   
	gettimeofday(&g_time_of_day, NULL);
    long tv_sec = (long)g_time_of_day.tv_sec;
	return tv_sec;
}

long im_util_gettimeofday_as_usec()
{   
	gettimeofday(&g_time_of_day, NULL);
	return (time_t)g_time_of_day.tv_usec;
}

int im_util_strftime_on_sec(long sec, char *strdate, char *fmt)
{
    int len = 0;
#if defined(_WIN64)
    struct tm new_tm;
    __time64_t long_time = (__time64_t)sec;
    _localtime64_s( &new_tm, &long_time );
    len = strftime(strdate, strlen(fmt)+32, fmt, &new_tm);
#else
    struct tm *new_tm;
    time_t long_time = (time_t)sec;

    new_tm = localtime(&long_time);
    len = strftime(strdate, strlen(fmt)+32, fmt, new_tm);
#endif
	return len;
}

char *im_util_strftime_now(char *buff, char *fmt)
{
	im_util_strftime_on_sec(im_util_gettimeofday_as_sec(), buff, fmt);
	return buff;
}

int im_util_gettime_now_msec()
{
	long usec = im_util_gettimeofday_as_usec();
	return (int)usec/1000;
}

unsigned long im_util_get_secs()
{
	return im_util_gettimeofday_as_sec();
}

unsigned long long im_util_get_unique_number() 
{
	static unsigned int counter = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  (unsigned long long)(tv.tv_sec + counter++);
}


char *im_util_get_timestamp_now_str()
{
	static char buff[128];
    im_util_strftime_now(buff, "%Y-%m-%dT%H:%M:%S.");
    sprintf(buff+strlen(buff), "%03d", im_util_gettime_now_msec());

	return buff;
}



int im_msleep(int msec)
{
/*
#if defined(WIN64)
	Sleep(msec);
#elif defined(WIN32)
	_sleep(msec);
*/

#if defined(WIN32)
	Sleep(msec);
#elif defined(FREERTOS)
	vTaskDelay(msec);
#else
	usleep(msec*1000);
#endif

	return 0;
}


#endif
