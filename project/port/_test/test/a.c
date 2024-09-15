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

static int example_send_data_in_json_string(im_client_tPtr cli)
{
  char *json_str = "{\"Temp\":25.5,"
					"\"Hum\":50, "
					"\"status\":\"hello\", "
					"\"TRUE\":true, "
					"\"FALSE\":false, "
					"\"DATE\":\"2020-10-10 10:10:10.100\", "

					"\"ARR_INT\":[0,1,2,3,4], "
					"\"ARR_FLT\":[0.5,1.5,2.5,3.5,4.5], "
					"\"ARR_STR\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"ARR_DATE\":[\"2020-10-11 10:10:10.100\", \"2020-10-12 10:10:10.100\"], "
					"\"ARR_BOOL\":[true, true, false]"
					"}";

	char *resource_name = "test";
	int rc = 0;

	//rc = im_send_data(cli, json_str, resource_name);

  return rc;
}



/*
export TZ=Asia/Seoul

setenv("TZ", "Australia/Brisbane", 1);
tzset();

*/

static int example_localtime(im_client_tPtr cli)
{
	char buff[128];


	setenv("TZ", "Australia/Brisbane", 1);
	//setenv("TZ", "Asia/Seoul", 1);
	//tzset();

    im_util_strftime_now(buff, "%Y-%m-%dT%H:%M:%S.");
    sprintf(buff+strlen(buff), "%03d", im_util_gettime_now_msec());
	printf("eventTime: %s\n", buff);

	return 0;
}


/** 
 *   @brief  biz_app MAIN
 *   Usage : simple_app {dev_id} {dev_pw} {svc_gw}
 */  
int main(int argc, char *argv[])
{
  int rc = -1;

  /********************************************************
  *  IoTMakers 디바이스 클라이언트 핸들러
  ********************************************************/
  im_client_t client;
  
  
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

    //rc = example_send_data_in_json_string(&client);
    example_localtime(&client);

  return 0;
}
