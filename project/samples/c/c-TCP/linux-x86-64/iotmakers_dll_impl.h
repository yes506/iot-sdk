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


/* combiner.h */
#include <stdio.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#define __stdcall
#endif 


/**
* 2021-03-05
*/
#define IM_RESP_CODE_2000_OK			2000
#define IM_RESP_CODE_2004_Changed		2004
#define IM_RESP_CODE_4000_BadRequest	4000
#define IM_RESP_CODE_4004_NotFound		4004
#define IM_RESP_CODE_5000_InternalError	5000


#ifdef __cplusplus
extern "C"
{
#endif

EXPORT int __stdcall ImInit(int debug_level);
EXPORT int __stdcall ImRelease(int hndl);

EXPORT int __stdcall ImConnectTo(int hndl, char* ec_ip, int ec_port);
EXPORT int __stdcall ImConnect(int hndl);
EXPORT int __stdcall ImDisconnect(int hndl);
EXPORT int __stdcall ImTurnCircuitBreakerOff(int hndl);
EXPORT int __stdcall ImTurnResponseWaitOff(int hndl);

EXPORT int __stdcall ImAuthDevice(int hndl, char *im_dev_id, char *im_dev_pw, char *im_dev_gw);
EXPORT int __stdcall ImPoll(int hndl);
EXPORT int __stdcall ImMSleep(int hndl, int msec);

EXPORT int __stdcall ImResourceSendNotification(int hndl, char *resource_id, char *properties_in_json);

EXPORT int __stdcall ImResourceNotificationInit(int hndl);
EXPORT int __stdcall ImResourceNotificationAppendResource(int hndl, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceNotificationSend(int hndl);

EXPORT void __stdcall ImSetOnResourceSetRequestHandler(int hndl, int(*func)(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr));
EXPORT void __stdcall ImSetOnResourceRetrieveOneRequestHandler(int hndl, int(*func)(void *pktBody, char *dev_id, char *resource_id));
EXPORT void __stdcall ImSetOnResourceRetrieveAllRequestHandler(int hndl, int(*func)(void *pktBody, char *dev_id));

EXPORT int __stdcall ImResourceRetrieveSetResource(void *pktBody, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceRetrieveAppendResource(void *pktBody, char *resource_id, char *properties_in_json);

#ifdef __cplusplus
}
#endif
