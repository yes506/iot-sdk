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
#include <stdarg.h>

#include "kt_iot_log.h"
#include "util/util.h"
#include "util/thread.h"
#include "client/client.h"

#include "packet/head/head.h"
#include "packet/body/body.h"
#include "packet/packet.h"

/**
	IMPacket req;

	im_sys_resource_notification_init()
	im_sys_resource_notification_append_properties()
	im_sys_resource_notification_send()
*/

/////////////////////////////////////////////////////////
// System Resource
int im_sys_resource_notification_init(im_client_tPtr cli)
{
#ifdef HAVE_THREAD
	unsigned long tid = thread_getid();
	IMPacketPtr nofi_req = (IMPacketPtr)im_resource_pkt_get_by_tid(cli, tid);
	if ( nofi_req == NULL )
	{
		return -1;
	}
	DBG("tid=[%ld], nofi_req=[%p]", tid, nofi_req);

#else
	IMPacketPtr nofi_req = (IMPacketPtr)(cli->res_nofi_pkt);
#endif

	im_packet_release(nofi_req); 

	int rc = im_packet411sys_req_init(nofi_req, cli->channelAccessToken);

	if ( rc < 0 )    {
	  ERR("im_packet411sys_req_init()");
	  return -1;
	}
	
	im_head_put_ext_str_val(&nofi_req->head, "operation", "NOTIFY");
	
	im_head_put_ext_str_val(&nofi_req->head, "to", "/systems/notifications");

	return 0;
}


int im_sys_resource_notification_append_resource(im_client_tPtr cli, char *resource_id, char *properties_in_json_str)
{
#ifdef HAVE_THREAD
	unsigned long tid = thread_getid();
	IMPacketPtr nofi_req = (IMPacketPtr)im_resource_pkt_get_by_tid(cli, tid);
	if ( nofi_req == NULL )
	{
		return -1;
	}
	DBG("tid=[%ld], nofi_req=[%p]", tid, nofi_req);

#else
	IMPacketPtr nofi_req = (IMPacketPtr)(cli->res_nofi_pkt);
#endif

	int rc = -1;

	JSON_Value *j_properties = json_parse_string_with_comments(properties_in_json_str);

	if ( j_properties == NULL )  {
	  im_packet_release(nofi_req);
	  ERR("Fail Parse; %s", properties_in_json_str);
	  return -1;
	}
	
	rc = im_body411sys_req_append_resource_representation(&nofi_req->body, cli->svc_gw, resource_id, j_properties);
	
	if ( rc < 0 )    {
	  ERR("im_body411sys_req_append_resource_representation()");
	  json_value_free(j_properties);
	  im_packet_release(nofi_req);
	  return -1;
	}

	json_value_free(j_properties);
	
	return 0;
}

int im_sys_resource_notification_send(im_client_tPtr cli)
{
  return im_resource_notification_send(cli);
}

int im_sys_resource_send_notification(im_client_tPtr cli, char *resource_id, char *properties_in_json_str)
{
	int rc = -1;

	rc = im_sys_resource_notification_init(cli);
    if ( rc < 0 )    {
        ERR("fail im_sys_resource_notification_init()");
        return -1;
      }

	rc = im_sys_resource_notification_append_resource(cli, resource_id, properties_in_json_str);
    if ( rc < 0 )    {
        ERR("fail im_sys_resource_notification_append_resource()");
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
////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
/** 525 제어수신 */
///////////////////////////////////////////////////////////////////
static int __parse_resource_url(char *resource_url, char *o_sys_id, char *o_resource_id)
{
	/*
	"/systems/{sys_id}/resources"			// 전체 리소스
	"/systems/{sys_id}/resources/{res_id}"	// 특정 리소스
	"/systems/testbyw111/resources/testAllTypeUri"
	*/
	char *sys_tok = "/systems/";
	char *res_tok = "/resources/";
	char *res_one_tok = "/resources";

	char *chBegin, *chEnd;

	if ( strstr(resource_url, sys_tok) == NULL )	{
		ERR("No systems in url");
		return -1;
	}

	if ( strstr(resource_url, res_tok) == NULL )	{
		/*
		ERR("No resources in url");
		return -1;
		*/
		if ( strstr(resource_url, res_one_tok) == NULL )	{
			ERR("No resources in url");
			return -1;
		} else {
			// 특정 리소스 url
			res_tok = res_one_tok;
		}
	} else {
		// 전체 리소스 url
		;
	}

	// Match device_id 
	chBegin = resource_url + strlen(sys_tok);
	chEnd = strstr(resource_url, res_tok);

	memcpy(o_sys_id, chBegin, (chEnd-chBegin));
	o_sys_id[(chEnd-chBegin)] = '\0';


	// Match resource_id
	chBegin = strstr(resource_url, res_tok) + strlen(res_tok);
	chEnd = resource_url + strlen(resource_url);

	memcpy(o_resource_id, chBegin, (chEnd-chBegin));
	o_resource_id[(chEnd-chBegin)] = '\0';

	return 0;
}


static void __resource_set_request_default_handler(IMPacketBodyPtr pktBody, char *sys_id, char *resource_id, char *properties_in_jstr)
{
	DBG("origin sys_id[%s],resource_id[%s]", sys_id, resource_id);
	DBG("properties_in_jstr = %s", properties_in_jstr);
	/*
	DBG("ectestBoolean = %d", im_body_get_value_bool(pktBody, "ectestBoolean"));
	*/
}

void im_packet525sys_request_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char sys_id[64];
	char resource_id[64];

	char* messageId = im_head_get_ext_messageId(&req->head);
	if ( messageId == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	char *resource_url = im_pktHeadExt_get_val(&req->head, "to");
	if ( resource_url == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	rc = __parse_resource_url(resource_url, sys_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url sys_id=[%s],  resource_id=[%s]", sys_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready ReportPacket
	/////////////////////////////////////////
	IMPacket rep;

	rc = im_packet525sys_rep_init(&rep, cli->channelAccessToken, messageId);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet525_rep_init()");
		ERR("====================================");
		return;
	}

	/////////////////////////////////////////
	// 2. 제어처리 
	/////////////////////////////////////////
	char* properties_in_jstr = im_body_get_serialized_string(&req->body);
	if ( cli->OnSysResourceSetRequestHandler == NULL )
	{
		__resource_set_request_default_handler(&req->body, sys_id, resource_id, properties_in_jstr);
		im_head_set_ext_responseCode(&rep.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnSysResourceSetRequestHandler(&req->body, sys_id, resource_id, properties_in_jstr);
		char retCodeBuff[16];
		sprintf(retCodeBuff, "%d", retCode);
		im_head_set_ext_responseCode(&rep.head, retCodeBuff);
	};


	/////////////////////////////////////////
	// 3. 제어레포트 전송(Report)
	//__if525_send_report(cli, resource_id, trx_id);
	/////////////////////////////////////////
	rc = im_transact_packet_with_response_handler(cli, &rep, NULL);
	if ( rc < 0 )    {
	  ERR("fail im_transact_packet_with_response_handler()");
	  im_packet_release(&rep);
	  return;
	}

	/////////////////////////////////////////
	// 4. release Report Packet
	/////////////////////////////////////////
	im_packet_release(&rep);

	return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
// 최종값 조회; 특정 리소스
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*

{
	"messageVersion": "1.0",
	"messageId": "1617093834902",
	"operation": "RETRIEVE",
	"to": "/systems/testbyw11/resources/testAllTypeUri"
}

*/
void im_packet711sys_request_one_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char sys_id[64];
	char resource_id[64];

	char* messageId = im_head_get_ext_messageId(&req->head);
	if ( messageId == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	char *resource_url = im_pktHeadExt_get_val(&req->head, "to");
	if ( resource_url == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	rc = __parse_resource_url(resource_url, sys_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url sys_id=[%s],  resource_id=[%s]", sys_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready response Packet
	/////////////////////////////////////////
	IMPacket res;

	rc = im_packet711sys_res_one_init(&res, cli->channelAccessToken, messageId);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet711_res_one_init()");
		ERR("====================================");
		return;
	}

	/////////////////////////////////////////
	// 2. 콜백처리 
	/////////////////////////////////////////
	if ( cli->OnSysResourceRetrieveOneRequestHandler == NULL )
	{
		im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnSysResourceRetrieveOneRequestHandler(&res.body, sys_id, resource_id);
		char retCodeBuff[16];
		sprintf(retCodeBuff, "%d", retCode);
		im_head_set_ext_responseCode(&res.head, retCodeBuff);
	};

	/////////////////////////////////////////
	// 3. 전송
	//__if525_send_report(cli, resource_id, trx_id);
	/////////////////////////////////////////
	rc = im_transact_packet_with_response_handler(cli, &res, NULL);
	if ( rc < 0 )    {
	  ERR("fail im_transact_packet_with_response_handler()");
	  im_packet_release(&res);
	  return;
	}

	/////////////////////////////////////////
	// 4. release Report Packet
	/////////////////////////////////////////
	im_packet_release(&res);

	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// 최종값 조회; 전체 리소스
/////////////////////////////////////////////////////////////////////////////////////////////////////
void im_packet711sys_request_all_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char sys_id[64];
	char resource_id[64];

	char* messageId = im_head_get_ext_messageId(&req->head);
	if ( messageId == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	char *resource_url = im_pktHeadExt_get_val(&req->head, "to");
	if ( resource_url == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	rc = __parse_resource_url(resource_url, sys_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url sys_id=[%s],  resource_id=[%s]", sys_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready response Packet
	/////////////////////////////////////////
	IMPacket res;

	rc = im_packet711sys_res_all_init(&res, cli->channelAccessToken, messageId);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet711_res_one_init()");
		ERR("====================================");
		return;
	}

	/////////////////////////////////////////
	// 2. 콜백처리 
	/////////////////////////////////////////
	if ( cli->OnSysResourceRetrieveAllRequestHandler == NULL )
	{
		im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnSysResourceRetrieveAllRequestHandler(&res.body, sys_id);
		char retCodeBuff[16];
		sprintf(retCodeBuff, "%d", retCode);
		im_head_set_ext_responseCode(&res.head, retCodeBuff);
	};

	/////////////////////////////////////////
	// 3. 전송
	//__if525_send_report(cli, resource_id, trx_id);
	/////////////////////////////////////////
	rc = im_transact_packet_with_response_handler(cli, &res, NULL);
	if ( rc < 0 )    {
	  ERR("fail im_transact_packet_with_response_handler()");
	  im_packet_release(&res);
	  return;
	}

	/////////////////////////////////////////
	// 4. release Report Packet
	/////////////////////////////////////////
	im_packet_release(&res);

	return;
}

#if 0
int im_resource_retrieve_set_resource(IMPacketBodyPtr body, char *resource_id, char *properties_in_json_str)
{
	int rc = -1;
	JSON_Value *j_properties = json_parse_string_with_comments(properties_in_json_str);

	if ( j_properties == NULL )  {
	  ERR("Fail Parse; %s", properties_in_json_str);
	  return -1;
	}
	
	// 단일 리소스 오브젝트
	rc = im_body711_res_set_resource_representation(body, resource_id, j_properties);
	
	if ( rc < 0 )    {
	  json_value_free(j_properties);
	  ERR("im_body711_req_set_resource_representation()");
	  return -1;
	}

	json_value_free(j_properties);
	
	return 0;
}


int im_resource_retrieve_append_resource(IMPacketBodyPtr body, char *resource_id, char *properties_in_json_str)
{
	int rc = -1;

	JSON_Value *j_properties = json_parse_string_with_comments(properties_in_json_str);

	if ( j_properties == NULL )  {
	  ERR("Fail Parse; %s", properties_in_json_str);
	  return -1;
	}
	
	// 다중 리소스 오브젝트 리스트
	rc = im_body711_res_append_resource_representation(body, resource_id, j_properties);
	
	if ( rc < 0 )    {
	  json_value_free(j_properties);
	  ERR("im_body411_req_new_resource_representation()");
	  return -1;
	}

	json_value_free(j_properties);
	
	return 0;
}

#endif
