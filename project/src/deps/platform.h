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



#ifndef IOTMAKERS_PLATFORM_H
#define IOTMAKERS_PLATFORM_H


#if defined(WIN32)
// #warning Please include winsock2.h before windows.h
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <sys/timeb.h>
#include <stdlib.h>

/*
#define im_msleep(msec) Sleep(msec)
//#define im_msleep(msec) _sleep(msec)
*/

#undef htons
#undef htonl
#undef ntohs
#undef ntohl

#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define NTOHS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define NTOHL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define htons HTONS 
#define htonl HTONL 
#define ntohs NTOHS 
#define ntohl NTOHL 

//defined(WIN32)
/////////////////////////////////////////////////////////////////

#elif defined(FREERTOS)

#include "lwip/inet.h"

//#define im_msleep(msec) vTaskDelay(msec)


#undef gethostbyname	
#undef socket			
#undef bind			
#undef connect			
#undef close			
#undef send			
#undef recv			
#undef select			


#define gethostbyname	lwip_gethostbyname	
#define socket			lwip_socket	
#define bind			lwip_bind	
#define connect			lwip_connect	
#define close			lwip_close	
#define send			lwip_send	
#define recv			lwip_recv	
#define select			lwip_select	


#else
// htons  htonl ntohs ntohl
#include <arpa/inet.h>  

// gettimeofday
#include <sys/time.h>   

// inet_aton, inet_addr, inet_network, inet_ntoa, inet_makeaddr, inet_lnaof, inet_netof
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//#define im_msleep(msec) usleep(msec*1000)


// defined (UNIX like)
/////////////////////////////////////////////////////////////////
#endif 


#endif
