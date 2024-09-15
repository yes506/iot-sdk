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


#ifdef __cplusplus
extern "C"
{
#endif

EXPORT int __stdcall ImInit(int debug_level);
EXPORT int __stdcall ImRelease(int hndl);

EXPORT int __stdcall ImConnect_to(int hndl, char* ec_ip, int ec_port);
EXPORT int __stdcall ImConnect(int hndl);
EXPORT int __stdcall ImDisconnect(int hndl);
EXPORT int __stdcall ImTurnCircuitBreakerOff(int hndl);
EXPORT int __stdcall ImTurnResponseWaitOff(int hndl);

EXPORT int __stdcall ImAuth_device(int hndl, char *im_dev_id,	char *im_dev_pw, char *im_dev_gw);
EXPORT int __stdcall ImPoll(int hndl);
EXPORT int __stdcall ImMSleep(int hndl, int msec);

EXPORT int __stdcall ImSendData(int hndl, char *json_str, char *resource_name);
EXPORT int __stdcall ImSendDataOnDev(int hndl, char *json_str, char *resource_name, char *dev_id);


EXPORT void __stdcall ImSet_strdata_handler(int hndl, char*(*func)(void *cli, char *devName, char *resource_name, char *prop_name, char *prop_value));
EXPORT void __stdcall ImSet_numdata_handler(int hndl, double(*func)(void *cli, char *devName, char *resource_name, char *prop_name, double prop_value));
EXPORT void __stdcall ImSet_intdata_handler(int hndl, int(*func)(void *cli, char *devName, char *resource_name, char *prop_name, int prop_value));
EXPORT void __stdcall ImSet_floatdata_handler(int hndl, double(*func)(void *cli, char *devName, char *resource_name, char *prop_name, double prop_value));
EXPORT void __stdcall ImSet_booldata_handler(int hndl, int(*func)(void *cli, char *devName, char *resource_name, char *prop_name, int prop_value));
EXPORT void __stdcall ImSet_endOfControl_handler(int hndl, int(*func)(void *cli, char *devName, char *resource_name, void *rep_body));

EXPORT int __stdcall ImSetReportForControl(void *rep_body, char *resource_name, char *json_str);                                                                                    


#ifdef __cplusplus
}
#endif
