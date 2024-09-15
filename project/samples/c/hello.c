/** 
 *  Copyright (c) 2021 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "iotmakers_dll_impl.h"

/**

EXPORT int __stdcall ImInit(int debug_level);
EXPORT int __stdcall ImRelease(int hndl);

EXPORT int __stdcall ImConnectTo(int hndl, char* ec_ip, int ec_port);
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

EXPORT void __stdcall ImSetOnResourceSetRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr));
EXPORT void __stdcall ImSetOnResourceRetrieveOneRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id, char *resource_id));
EXPORT void __stdcall ImSetOnResourceRetrieveAllRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id));

EXPORT int __stdcall ImResourceRetrieveSetResource(void *pktBody, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceRetrieveAppendResource(void *pktBody, char *resource_id, char *properties_in_json);

*/



/**
	리소스 설정(제어) 요청 처리 핸들러
*/
static int OnResourceSetRequestHandler(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	printf("dev_id = %s\n", dev_id);
	printf("resource_id = %s\n", resource_id);
	printf("properties_in_jstr = %s\n", properties_in_jstr);

	// utility api 
	//printf("ectestBoolean = %d\n", im_body_get_value_bool(pktBody, "ectestBoolean"));

	return IM_RESP_CODE_2004_Changed;
}

/**
	특정 리소스(resource_id) 조회 요청 처리 핸들러
*/
static int OnResourceRetrieveOneRequestHandler(void *pktBody, char *dev_id, char *resource_id)
{
	printf("dev_id = %s\n", dev_id);
	printf("resource_id = %s\n", resource_id);

	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	int rc = ImResourceRetrieveSetResource(pktBody, resource_id, properties);
    if ( rc < 0 )    {
        printf("fail im_resource_notification_append()\n");
        return IM_RESP_CODE_5000_InternalError;
      }

	return IM_RESP_CODE_2000_OK;
}

/**
	본 디바이스(dev_id)의 모든 리소스 조회 요청 처리 핸들러
*/
static int OnResourceRetrieveAllRequestHandler(void *pktBody, char *dev_id)
{
	printf("dev_id = %s\n", dev_id);

	// First One
	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id = "testAllTypeUri";

	int rc = ImResourceRetrieveSetResource(pktBody, resource_id, properties);
    if ( rc < 0 )    {
        printf("fail ImResourceRetrieveSetResource()\n");
        return -1;
      }

	// Second One
	char *properties2 = "{"
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id2 = "testAllTypeUri";
	rc = ImResourceRetrieveAppendResource(pktBody, resource_id2, properties2);
    if ( rc < 0 )    {
        printf("fail ImResourceRetrieveAppendResource()\n");
        return IM_RESP_CODE_5000_InternalError;
      }

	return IM_RESP_CODE_2000_OK;
}


static int example_resource_send_notification(int hndl)
{
	int rc = -1;

	// 1. 전송 패킷 준비
	rc = ImResourceNotificationInit(hndl);
    if ( rc < 0 )    {
        printf("fail hndl()\n");
        return -1;
      }



	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id = "testAllTypeUri";

	// 2. 패킷에 리소스 추가
	rc = ImResourceNotificationAppendResource(hndl, resource_id, properties);
    if ( rc < 0 )    {
        printf("fail ImResourceNotificationAppendResource()\n");
        return -1;
      }

	// 3. 패킷 전송
	rc = ImResourceNotificationSend(hndl);
    if ( rc < 0 )    {
        printf("fail ImResourceNotificationSend()\n");
        return -1;
      }

	return rc;
}





/** 
 *   Usage : simple_app {ec_ip} {ec_port} {dev_id} {dev_pw} {dev_gw}
 */  
int main(int argc, char *argv[])
{
  int ret = 0;

  if ( argc != 6 )  {
    printf("Usage : %s {ec_ip} {ec_port} {dev_id} {dev_pw} {dev_gw}\n", argv[0]);
    exit(0);
  }

  char  *ec_ip = argv[1];
  int  ec_port = atoi(argv[2]);
  char *dev_id = argv[3];
  char *dev_pw = argv[4];
  char *svc_gw = argv[5];

  printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n", 
                        ec_ip, ec_port, dev_id, dev_pw, svc_gw);


  /********************************************************
  *  IoTMakers 디바이스 클라이언트 핸들러
  ********************************************************/

  ///////////////////////////////////////////////////
  // IoTMakers Device Init; 1=printfOR, 2=INFO, 3=DEBUG
  ///////////////////////////////////////////////////
  int log_level = 3;
  
  int hndl = ImInit(log_level);
  if ( hndl < 0 )		{
    printf("fail ImInit()\n");
    return 0;
  }
  
  ///////////////////////////////////////////////////
  // Register Device Control Handler
  ///////////////////////////////////////////////////
  ImSetOnResourceSetRequestHandler(hndl, OnResourceSetRequestHandler);
  ImSetOnResourceRetrieveOneRequestHandler(hndl, OnResourceRetrieveOneRequestHandler);
  ImSetOnResourceRetrieveAllRequestHandler(hndl, OnResourceRetrieveAllRequestHandler);

  ///////////////////////////////////////////////////
  // 고속전송 옵션
  ///////////////////////////////////////////////////
  //ImTurnCircuitBreakerOff(hndl);
  //ImTurnResponseWaitOff(hndl);


  ///////////////////////////////////////////////////
  // Connect to Server
  ///////////////////////////////////////////////////
  if ( ImConnectTo(hndl, ec_ip, ec_port) < 0 )	{
  	return 0;
  }
  
  ///////////////////////////////////////////////////
  // Auth Device
  ///////////////////////////////////////////////////
  if ( ImAuthDevice(hndl, dev_id, dev_pw, svc_gw) < 0 )	{
  	return 0;
  }
  
  while (1==1)
  {
	//////////////////////////////////////
	// 센싱데이터 전송 예
	ret = example_resource_send_notification(hndl);
  	if ( ret < 0 )			{
  		break;
  	}

    ImMSleep(hndl, 1*1000);

  	///////////////////////////////////////////////////
  	// NetIO polling
  	///////////////////////////////////////////////////
  	if ( ImPoll(hndl) < 0 )			{
  		break;
  	}
  	
  }
  
  ///////////////////////////////////////////////////
  // Disconnect
  ///////////////////////////////////////////////////
  ImDisconnect(hndl);
  
	///////////////////////////////////////////////////
	// Release
	///////////////////////////////////////////////////
	ImRelease(hndl);
   
  return 0;
}



