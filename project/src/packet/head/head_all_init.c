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


#include "packet/head/head.h"

int im_head000_req_release(IMPacketHeadPtr head)
{
  im_head_release(head);
	return 0;
}


/** 장치 TCP 채널 인증 */
int im_head224_req_init(IMPacketHeadPtr head )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Request);
	im_head_set_MessageType(head, MessageType_AUTH_DEV);
	return 0;
}

int im_head224sys_req_init(IMPacketHeadPtr head )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Request);
	im_head_set_MessageType(head, MessageType_AUTH_SYS);
	return 0;
}




/** KeepAlive TCP 채널 */
int im_head231_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Request);
	im_head_set_MessageType(head, MessageType_KeepAlive);
	im_head_set_channelAccessToken(head, auth_code);
	return 0;
}


/** 수집 통합데이터 */
int im_head411_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Request);
	im_head_set_MessageType(head, MessageType_DevResourceNoti);
	im_head_set_channelAccessToken(head, auth_code);
	return 0;
}
int im_head411sys_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Request);
	im_head_set_MessageType(head, MessageType_SysResourceNoti);
	im_head_set_channelAccessToken(head, auth_code);
	return 0;
}




/** 제어 수신 응답 */
int im_head525_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_DevResourceUpdate);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}
int im_head525sys_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_SysResourceUpdate);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}

/** 제어 레포트 통보 */
int im_head525_rep_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Report);
	im_head_set_MessageType(head, MessageType_DevResourceUpdate);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}
int im_head525sys_rep_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Report);
	im_head_set_MessageType(head, MessageType_SysResourceUpdate);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}

/** 최종값; 특정 리소스 */
int im_head711_res_one_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_DevResourceRetrieveOne);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}

int im_head711sys_res_one_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_SysResourceRetrieveOne);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}


/** 최종값; 전체 리소스 */
int im_head711_res_all_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_DevResourceRetrieveAll);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}

int im_head711sys_res_all_init(IMPacketHeadPtr head, char *auth_code, char *msgid )
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
	im_head_set_MessageType(head, MessageType_SysResourceRetrieveAll);
	im_head_set_channelAccessToken(head, auth_code);
	im_head_set_ext_messageId(head, msgid);
	return 0;
}


