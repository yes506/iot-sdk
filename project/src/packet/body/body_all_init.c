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



#include "packet/body/body.h"

int im_body000_req_release(IMPacketBodyPtr body)
{
    im_body_release(body);
	return 0;
}


/** 장치 TCP 채널 인증 */
int im_body224_req_init(IMPacketBodyPtr body)
{
  	im_body_init_with_string(body, "{}");
	return 0;
}

/** KeepAlive TCP 채널 */
int im_body231_req_init(IMPacketBodyPtr body)
{
  	im_body_init_with_string(body, "{}");
	return 0;
}


/** 수집 통합데이터 */
int im_body411_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "[]");
	return 0;
}

/** 제어 수신 응답 */
int im_body525_res_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}

/** 제어 레포트 통보 */
int im_body525_rep_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}


/** 최종값; 특정 리소스 */
int im_body711_res_one_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}


/** 최종값; 전체 리소스 */
int im_body711_res_all_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "[]");
	return 0;
}

