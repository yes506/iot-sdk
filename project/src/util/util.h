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


#ifndef IOTMAKERS_UTIL_H
#define IOTMAKERS_UTIL_H


/*
 * ********************************************************************************
 * util.h
 * ********************************************************************************
 */

#ifdef __cplusplus
extern "C"
{
#endif

long im_util_gettimeofday_as_sec();
long im_util_gettimeofday_as_usec();
int im_util_strftime_on_sec(long sec, char *strdate, char *fmt);
char *im_util_strftime_now(char *buff, char *fmt);
int im_util_gettime_now_msec();
unsigned long long im_util_get_unique_number() ;
int im_util_str_trim(char *szStr, int nLen);
int im_util_read_config(const char* config, char* ip, unsigned short* port, 
	                    char* extrSysId, char* deviceId, char* athnRqtNo, char* log_file_name);

unsigned long im_util_get_secs();

char *im_util_get_timestamp_now_str();

int im_msleep(int msec);


#ifdef __cplusplus
}
#endif


#endif
