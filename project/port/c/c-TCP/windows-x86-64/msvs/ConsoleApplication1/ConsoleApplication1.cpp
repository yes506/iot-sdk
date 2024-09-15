#if 1

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

//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>




#pragma comment(lib, "libiotmakers-3rdparty-interface.lib")

#include "iotmakers_dll_impl.h"

/*
#ifdef __cplusplus
extern "C" {
#endif

	int ImRelease(int hndl);

#ifdef __cplusplus
};
#endif
*/


 /**
	 리소스 설정(제어) 요청 처리 핸들러
 */
static int OnResourceSetRequestHandler(void* pktBody, char* dev_id, char* resource_id, char* properties_in_jstr)
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
static int OnResourceRetrieveOneRequestHandler(void* pktBody, char* dev_id, char* resource_id)
{
	printf("dev_id = %s\n", dev_id);
	printf("resource_id = %s\n", resource_id);

	const char* properties = "{"
		"\"ectestString\": \"hello\", "
		"\"ectestDouble\": 99.55, "
		"\"ectestInteger\": 100, "
		"\"ectestBoolean\": true"
		"}";

	int rc = ImResourceRetrieveSetResource(pktBody, resource_id, (char*)properties);
	if (rc < 0) {
		printf("fail im_resource_notification_append()\n");
		return IM_RESP_CODE_5000_InternalError;
	}

	return IM_RESP_CODE_2000_OK;
}

/**
	본 디바이스(dev_id)의 모든 리소스 조회 요청 처리 핸들러
*/
static int OnResourceRetrieveAllRequestHandler(void* pktBody, char* dev_id)
{
	printf("dev_id = %s\n", dev_id);

	// First One
	const char* properties = "{"
		"\"ectestString\": \"hello\", "
		"\"ectestDouble\": 99.55, "
		"\"ectestInteger\": 100, "
		"\"ectestBoolean\": true"
		"}";

	const char* resource_id = "testAllTypeUri";

	int rc = ImResourceRetrieveSetResource(pktBody, (char*)resource_id, (char*)properties);
	if (rc < 0) {
		printf("fail ImResourceRetrieveSetResource()\n");
		return -1;
	}

	// Second One
	const char* properties2 = "{"
		"\"ectestInteger\": 100, "
		"\"ectestBoolean\": true"
		"}";

	const char* resource_id2 = "testAllTypeUri";
	rc = ImResourceRetrieveAppendResource(pktBody, (char*)resource_id2, (char*)properties2);
	if (rc < 0) {
		printf("fail ImResourceRetrieveAppendResource()\n");
		return IM_RESP_CODE_5000_InternalError;
	}

	return IM_RESP_CODE_2000_OK;
}


static int example_resource_send_notification(int hndl)
{
	int rc = -1;

	// 1. 전송 패킷 준비
	rc = ImSysResourceNotificationInit(hndl);
	if (rc < 0) {
		printf("fail hndl()\n");
		return -1;
	}



	const char* properties = "{"
		"\"ectestString\": \"hello\", "
		"\"ectestDouble\": 99.55, "
		"\"ectestInteger\": 100, "
		"\"ectestBoolean\": true"
		"}";

	const char* resource_id = "testAllTypeUri";

	// 2. 패킷에 리소스 추가
	rc = ImSysResourceNotificationAppendResource(hndl, (char*)resource_id, (char*)properties);
	if (rc < 0) {
		printf("fail ImResourceNotificationAppendResource()\n");
		return -1;
	}

	// 3. 패킷 전송
	rc = ImSysResourceNotificationSend(hndl);
	if (rc < 0) {
		printf("fail ImResourceNotificationSend()\n");
		return -1;
	}

	return rc;
}





/**
 *   Usage : simple_app {ec_ip} {ec_port} {dev_id} {dev_pw} {dev_gw}
 */
int main(int argc, char* argv[])
{
	int ret = 0;

#if 0
	if (argc != 6) {
		printf("Usage : %s {ec_ip} {ec_port} {chn_id} {chn_pw} {sys_id}\n", argv[0]);
		exit(0);
	}

	char* ec_ip = argv[1];
	int  ec_port = atoi(argv[2]);
	char* chn_id = argv[3];
	char* chn_pw = argv[4];
	char* sys_id = argv[5];


	IM_SERVER_ADDR = "dev-gw-iotmakers.cloud.kt.com";
	IM_SERVER_PORT = 30024;
	IM_DEV_ID = "testbyw111";
	IM_DEV_PW = "12312";
	IM_DEV_GW = "KT_3RD_BYW";
	IM_LOGLEVEL = 3;	# 1:ERR, 2 : INFO, 3 : DEBUG

#else

	const char* ec_ip = "dev-gw-iotmakers.cloud.kt.com";
	const int  ec_port = 30024;
	const char* chn_id = "testbyw111";
	const char* chn_pw = "12312";
	const char* sys_id = "KT_3RD_BYW";
#endif

	printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n",
		ec_ip, ec_port, (char*)chn_id, (char*)chn_pw, (char*)sys_id);


	/********************************************************
	*  IoTMakers 디바이스 클라이언트 핸들러
	********************************************************/

	///////////////////////////////////////////////////
	// IoTMakers Device Init; 1=printfOR, 2=INFO, 3=DEBUG
	///////////////////////////////////////////////////
	int log_level = 3;

	int hndl = ImInit(log_level);
	if (hndl < 0) {
		printf("fail ImInit()\n");
		return 0;
	}

	///////////////////////////////////////////////////
	// Register Device Control Handler
	///////////////////////////////////////////////////
	ImSetOnSysResourceSetRequestHandler(hndl, OnResourceSetRequestHandler);
	ImSetOnSysResourceRetrieveOneRequestHandler(hndl, OnResourceRetrieveOneRequestHandler);
	ImSetOnSysResourceRetrieveAllRequestHandler(hndl, OnResourceRetrieveAllRequestHandler);

	///////////////////////////////////////////////////
	// 고속전송 옵션
	///////////////////////////////////////////////////
	ImTurnCircuitBreakerOff(hndl);
	//ImTurnResponseWaitOff(hndl);


	///////////////////////////////////////////////////
	// Connect to Server
	///////////////////////////////////////////////////
	if (ImConnectTo(hndl, (char*)ec_ip, ec_port) < 0) {
		return 0;
	}

	///////////////////////////////////////////////////
	// Auth System
	///////////////////////////////////////////////////
	if (ImAuthDevice(hndl, (char*)chn_id, (char*)chn_pw, (char*)sys_id) < 0) {
		return 0;
	}

	while (1 == 1)
	{
		//////////////////////////////////////
		// 센싱데이터 전송 예
		ret = example_resource_send_notification(hndl);
		if (ret < 0) {
			break;
		}

		ImMSleep(hndl, 1 * 1000);

		///////////////////////////////////////////////////
		// NetIO polling
		///////////////////////////////////////////////////
		if (ImPoll(hndl) < 0) {
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






#else
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

//#include "iotmakers_dll_impl.h"

#pragma comment(lib, "libiotmakers-3rdparty-interface.lib")


#ifdef __cplusplus
extern "C" {
#endif

int ImRelease(int hndl);

#ifdef __cplusplus
};
#endif



//void im_client_set_loglevel(int);

int main()
{
    std::cout << "Hello World!\n";
	
	ImRelease(0);
	
//im_client_set_loglevel(3);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
#endif
