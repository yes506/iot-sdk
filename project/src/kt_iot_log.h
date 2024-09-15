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



#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "util/util.h"

#define PRINT(...) printf(__VA_ARGS__)

#if DEBUG
extern int g_log_level;	//0 No debug, 1 Error, 2 Warning, 3 Debug

#define LOG(level, ...)                                                     \
  do {                                                                         \
	if(g_log_level==0) break;							\
	PRINT("%s %s: %s <%s:%d>: ", im_util_get_timestamp_now_str(), level, __FILE__, __func__, __LINE__);            \
    PRINT(__VA_ARGS__);                                                        \
    PRINT("\n");                                                               \
  } while (0)


#define DBG(...) if(g_log_level>=3)LOG("DBG", __VA_ARGS__)
#define WRN(...) if(g_log_level>=2)LOG("WRN", __VA_ARGS__)
#define INF(...) if(g_log_level>=2)LOG("INF", __VA_ARGS__)
#define ERR(...) if(g_log_level>=1)LOG("ERR", __VA_ARGS__)


#define LOGbytes(bytes, length)                                             \
  do {                                                                         \
	if(g_log_level<3) break;							\
    PRINT("HEX: %s <%s:%d>: ", __FILE__, __func__, __LINE__);                \
    int i;                                                                 \
    for(i=0;i<length;i++){if(i%16==0)PRINT("\n");else if (i%8==0)PRINT(" |"); PRINT(" %02X", (unsigned char)bytes[i]);    }\
    PRINT("\n");                                                               \
  } while (0)


inline static void hex(unsigned char* data, int size)
{
  int i, rowsize, offset;
  char *bufpos, buffer[80];	/* XXX handle buffer overflow */
  
	if (size<=0){
		return;
	}

  for( offset = 0; size;  )
	{
		rowsize = ( size > 16 ) ? 16 : size;
		sprintf( buffer, "%04x: ", offset );
		bufpos = buffer + strlen( buffer );
	
		if ( rowsize >= 8 ) {
			for( i = 0; i < 8; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
			strcat( bufpos, "| " ); bufpos += 2;
			for( ; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}else{
			for( i = 0; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}

		for( ; i < 16; i++ ) {
		    strcat( buffer, "   " );
		};

		strcat( buffer, "  " );
		bufpos = buffer + strlen( buffer );
		for( i = 0; i < rowsize; i++, bufpos++ ) {
			sprintf( bufpos, "%c", ( isprint(data[i]) && (data[i]!='%') ) ? data[i] : '.' );
		};

		size -= rowsize;
		data += rowsize;
		offset += rowsize;
	
		*bufpos = '\0';

		PRINT("%s\n", buffer);
	}
}

#define LOGHex(tag, bytes, length)                                      \
  do {                                                                  \
	if(g_log_level < 3) break;                                            \
    PRINT("HEX: %s <%s:%d>: %s[%d bytes]\n", __FILE__, __func__, __LINE__, tag, length);  \
    hex(bytes, length);                                                  \
  } while (0)




#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "IoTMakersDeviceCore"
#define  ERR(...)  if(g_log_level>=1)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG,__VA_ARGS__)
#define  WAN(...)  if(g_log_level>=2)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG,__VA_ARGS__)
#define  INF(...)  if(g_log_level>=2)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG,__VA_ARGS__)
#define  DBG(...)  if(g_log_level>=3)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,__VA_ARGS__)
#define  LOG(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG,__VA_ARGS__)
#endif




#else   // DEBUG
#define LOG(...)
#define DBG(...)
#define INF(...)
#define WRN(...)
#define ERR(...)
#define LOGipaddr(endpoint)
#define LOGbytes(bytes, length)
#define LOGHex(tag, bytes, length)
#endif

#endif /* _LOG_H */
