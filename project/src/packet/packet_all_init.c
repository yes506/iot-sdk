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


#include "packet/packet.h"

	
/** 장치 TCP 채널 인증 */
int im_packet224_req_init(IMPacketPtr pkt)
{
	im_head224_req_init(&pkt->head);
	im_body224_req_init(&pkt->body);
	return 0;
}

int im_packet224sys_req_init(IMPacketPtr pkt)
{
	im_head224sys_req_init(&pkt->head);
	im_body224_req_init(&pkt->body);
	return 0;
}


	
/** KeepAlive TCP 채널 */
int im_packet231_req_init(IMPacketPtr pkt, char *channelAccessToken)
{
	im_head231_req_init(&pkt->head, channelAccessToken);
	im_body231_req_init(&pkt->body);
	return 0;
}

/** 수집 통합데이터 */
int im_packet411_req_init(IMPacketPtr pkt, char *channelAccessToken)
{
	im_head411_req_init(&pkt->head, channelAccessToken);
	im_body411_req_init(&pkt->body);
	return 0;
}
int im_packet411sys_req_init(IMPacketPtr pkt, char *channelAccessToken)
{
	im_head411sys_req_init(&pkt->head, channelAccessToken);
	im_body411_req_init(&pkt->body);
	return 0;
}


/** 제어 수신 응답*/
int im_packet525_res_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head525_res_init(&pkt->head, channelAccessToken, msgid);
	im_body525_res_init(&pkt->body);
	return 0;
}
int im_packet525sys_res_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head525sys_res_init(&pkt->head, channelAccessToken, msgid);
	im_body525_res_init(&pkt->body);
	return 0;
}

/** 제어 레포트 통보 */
int im_packet525_rep_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head525_rep_init(&pkt->head, channelAccessToken, msgid);
	im_body525_rep_init(&pkt->body);
	return 0;
}
int im_packet525sys_rep_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head525sys_rep_init(&pkt->head, channelAccessToken, msgid);
	im_body525_rep_init(&pkt->body);
	return 0;
}

/** 최종값; 특정 리소스 */
int im_packet711_res_one_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head711_res_one_init(&pkt->head, channelAccessToken, msgid);
	im_body711_res_one_init(&pkt->body);
	return 0;
}
int im_packet711sys_res_one_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head711sys_res_one_init(&pkt->head, channelAccessToken, msgid);
	im_body711_res_one_init(&pkt->body);
	return 0;
}


/** 최종값; 전체 리소스 */
int im_packet711_res_all_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head711_res_all_init(&pkt->head, channelAccessToken, msgid);
	im_body711_res_all_init(&pkt->body);
	return 0;
}

int im_packet711sys_res_all_init(IMPacketPtr pkt, char *channelAccessToken, char *msgid)
{
	im_head711sys_res_all_init(&pkt->head, channelAccessToken, msgid);
	im_body711_res_all_init(&pkt->body);
	return 0;
}

