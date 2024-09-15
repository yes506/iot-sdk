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

#include <stdio.h>
#include "client/client.h"


static int OnResourceSetRequestHandler(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	DBG("dev_id = %s", dev_id);
	DBG("resource_id = %s", resource_id);
	printf("properties_in_jstr = %s\n", properties_in_jstr);

	// utility api 
	DBG("ectestBoolean = %d", im_body_get_value_bool(pktBody, "ectestBoolean"));

#if 0
	int cnt = 3;
  while ( cnt-- > 0 )  {
	im_msleep(1*1000);
	printf("ectestBoolean = %d\n", im_body_get_value_bool(pktBody, "ectestBoolean"));
    }
#endif

	return IM_RESP_CODE_2004_Changed;
}

static int OnResourceRetrieveOneRequestHandler(void *pktBody, char *dev_id, char *resource_id)
{
	DBG("dev_id = %s", dev_id);
	DBG("resource_id = %s", resource_id);

	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	int rc = im_resource_retrieve_set_resource(pktBody, resource_id, properties);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_append()");
        return IM_RESP_CODE_5000_InternalError;
      }

	return IM_RESP_CODE_2000_OK;
}

static int OnResourceRetrieveAllRequestHandler(void *pktBody, char *dev_id)
{
	DBG("dev_id = %s", dev_id);

	// First One
	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id = "testAllTypeUri";

	int rc = im_resource_retrieve_append_resource(pktBody, resource_id, properties);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_append()");
        return -1;
      }

	// Second One
	char *properties2 = "{"
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id2 = "testAllTypeUri";
	rc = im_resource_retrieve_append_resource(pktBody, resource_id2, properties2);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_append()");
        return IM_RESP_CODE_5000_InternalError;
      }


	return IM_RESP_CODE_2000_OK;
}





/** 
 *  수집데이터 전송 예  
 */  
static int _TEST_example_send_data_in_json_string(im_client_tPtr cli)
{
#if 0
  //char *properties = "{\"ectestString\":\"hello\"}";
  
  /*
  char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true "
					  "}";

  char *properties = "{"
					"\"ectestStringArray\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"ectestIntegerArray\":[100, 200, 300], "
					"\"ectestDoubleArray\":[100.5, 200.5, 300.5], "
					"\"ectestBooleanArray\":[true,true,false,false]"
					  "}";


  char *properties = "{"
						"\"ectestObject\": {"
							"\"x\": 10,"
							"\"y\": 20,"
							"\"z\": 30"
						"}"
					  "}";

  char *properties = "{"
						"\"ectestObjectArray\": ["
							"{"
								"\"a\": 10,"
								"\"b\": 20,"
								"\"c\": 30"
							"},"
							"{"
								"\"x\": \"10\","
								"\"y\": \"20\","
								"\"z\": \"30\""
							"}"
						"]"
					  "}";
*/

#else
  char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true,"
					"\"ectestStringArray\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"ectestIntegerArray\":[100, 200, 300], "
					"\"ectestDoubleArray\":[100.5, 200.5, 300.5], "
					"\"ectestBooleanArray\":[true,true,false,false],"
						"\"ectestObjectArray\": ["
							"{"
								"\"a\": 10,"
								"\"b\": 20,"
								"\"c\": 30"
							"},"
							"{"
								"\"x\": \"10\","
								"\"y\": \"20\","
								"\"z\": \"30\""
							"}"
						"]"
					"}";
#endif
  
	char *resource_id = "testAllTypeUri";

	int rc = im_resource_send_notification(cli, resource_id, properties);

	if ( rc < 0)  {
		ERR("fail im_send_data()");
		return -1;
	}

	return 0;
}



static int example_resource_send_notification(im_client_tPtr cli)
{
  char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true,"
					"\"ectestStringArray\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"ectestIntegerArray\":[100, 200, 300], "
					"\"ectestDoubleArray\":[100.5, 200.5, 300.5], "
					"\"ectestBooleanArray\":[true,true,false,false],"
						"\"ectestObjectArray\": ["
							"{"
								"\"a\": 10,"
								"\"b\": 20,"
								"\"c\": 30"
							"},"
							"{"
								"\"x\": \"10\","
								"\"y\": \"20\","
								"\"z\": \"30\""
							"}"
						"]"
					"}";

	char *resource_id = "testAllTypeUri";

	int rc = im_resource_send_notification(cli, resource_id, properties);

	if ( rc < 0)  {
		ERR("fail im_send_data()");
		return -1;
	}

	printf("sent data\n");

	return 0;
}


static int example_resource_send_notification_multi(im_client_tPtr cli)
{
	int rc = -1;

	rc = im_resource_notification_init(cli);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_init()");
        return -1;
      }

	char *properties = "{"
					  "\"ectestString\": \"hello\", "
					  "\"ectestDouble\": 99.55, "
					  "\"ectestInteger\": 100, "
					  "\"ectestBoolean\": true"
					"}";

	char *resource_id = "testAllTypeUri";

	rc = im_resource_notification_append_resource(cli, resource_id, properties);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_append()");
        return -1;
      }

	rc = im_resource_notification_append_resource(cli, resource_id, properties);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_append()");
        return -1;
      }


	rc = im_resource_notification_send(cli);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_send()");
        return -1;
      }

	return rc;
}
//
//////////////////////////////////////////////////////////////////////////


#ifdef HAVE_THREAD

/********************************************************
*  수신 이벤트 폴링 Thread
********************************************************/
thread_return_type WINAPI _thread_im_poll(void *arg)
{
	int rc = 0;
	im_client_tPtr cli = (im_client_tPtr)arg;

	DBG("_thread_one");
	while (1)
	{
		rc = im_poll(cli);
		if ( rc < 0 )  {
		  break;
		}
		im_msleep(1*100);
	}

	return 0;
}


/********************************************************
*  리소스 변경 통지 전송 Thread
********************************************************/
thread_return_type WINAPI _thread_resource_noti(void *arg)
{
	int rc = 0;
	im_client_tPtr cli = (im_client_tPtr)arg;

	unsigned long tid = thread_getid();
	DBG("tid=[%ld]", tid);
	while (1)
	{
		rc = example_resource_send_notification_multi(cli);
		if ( rc < 0 )  {
		  break;
		}

		printf("sent ok; tid=[%ld]\n", tid);

		im_msleep(1*10);
	}

	return 0;
}
#endif




int main(int argc, char *argv[])
{
  int rc = -1;

  /********************************************************
  *  IoTMakers 디바이스 클라이언트 핸들러
  ********************************************************/
  im_client_t client;
  

  if ( argc != 6 )  {
    printf("Usage : %s {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}\n", argv[0]);
    exit(0);
  }

  char *ec_ip = argv[1];
  int ec_port = atoi(argv[2]);
  char *dev_id = argv[3];
  char *dev_pw = argv[4];
  char *svc_gw = argv[5];

  printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n", 
                        ec_ip, ec_port, dev_id, dev_pw, svc_gw);
  
  /********************************************************
  * 로그레벨 설정
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(3);


  /********************************************************
  *  핸들러 초기화
  ********************************************************/
  rc = im_client_init(&client);
  if ( rc < 0 )  {
    return -1;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (숫자형/문자형)
  ********************************************************/
  client.OnResourceSetRequestHandler = OnResourceSetRequestHandler;
  client.OnResourceRetrieveOneRequestHandler = OnResourceRetrieveOneRequestHandler;
  client.OnResourceRetrieveAllRequestHandler = OnResourceRetrieveAllRequestHandler;

  /********************************************************
  *  접속 종료시 이 지점부터 다시 시작
  ********************************************************/
_AGAIN:

  /********************************************************
  *   EC 접속 주소 지정 (TCP 표준 인터페이스)
  ********************************************************/
  rc = im_connect_to(&client, ec_ip, ec_port);


  /********************************************************
  *  디바이스 인증 요청
  ********************************************************/
  rc = im_auth_device(&client, dev_id, dev_pw, svc_gw);
  if ( rc < 0 )  {
    im_disconnect(&client);
    return -1;
  }


#ifdef HAVE_THREAD
	// 다중쓰레드 고속전송 옵션
	im_turn_circuit_breaker_off(&client);
	im_turn_response_wait_off(&client);

	// 수신 폴링 쓰레드
	thread_start(_thread_im_poll, &client);

	// 리소스 변경 통지 쓰레드
	thread_start(_thread_resource_noti, &client);

  while ( 1==1 )  {
		im_msleep(1*1000);
  }

#else
	// 
  while ( 1==1 )  {

    /********************************************************
    *  예제) 수집데이터 전송 
    ********************************************************/
	rc = example_resource_send_notification_multi(&client);
    if ( rc < 0 )  {
      // 전송실패시 loop종료
      break;
    }

	im_msleep(1*1000);

    /********************************************************
    *  송수신 이벤트 폴링
    ********************************************************/
    rc = im_poll(&client);
    if ( rc < 0 )  {
      // 송수신 처리 실패시 loop종료
      break;
    }
  }
#endif



  /********************************************************
  *  접속 해제
  ********************************************************/
  im_disconnect(&client);


  /********************************************************
  *  3초 후에 재접속 시도
  ********************************************************/
  //sleep(3);
  //goto _AGAIN;

  im_client_release(&client);


  return 0;
}











