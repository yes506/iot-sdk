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


///////////////////////////////////////////////////////////////////
/** 411 수집전송 수신처리 */
///////////////////////////////////////////////////////////////////
static int response_packet_handler_411(im_client_tPtr cli, IMPacketPtr res)
{
	char* responseCode = im_head_get_ext_responseCode(&res->head) ;
	DBG("responseCode=[%s]", responseCode);

	if ( strcmp(IM_RESP_CODE_2000_OK_STR, responseCode) != 0 )	{
		return (-1);
	}

	return 0;
}

static void circuit_breaker(im_client_tPtr cli)
{
  if ( cli->circuit_breaker_sw == 0 )  {
    return;
  }

  if ( IOTMAKERS_CIRCUIT_BREAKER_IN_SEC > 0 )
  {
    while ( (im_util_gettimeofday_as_sec() - cli->circuit_breaker_timestamp) <  IOTMAKERS_CIRCUIT_BREAKER_IN_SEC )
    {
      WRN("IOTMAKERS_CIRCUIT_BREAKER_IN_SEC=[%d secs]", IOTMAKERS_CIRCUIT_BREAKER_IN_SEC);
      im_msleep(1*1000);
    }
    cli->circuit_breaker_timestamp = im_util_gettimeofday_as_sec(); 
  }
}



static int __is_string_valid(char *str)
{
	if ( str == NULL )	{
		return 0;
	}

	if ( str[0] == '\0' )
	{
		return 0;
	}

	return (1);
}

static int __is_number_integer(double number)
{
	char buff[64];
	sprintf(buff, "%g", number);
	if ( strchr(buff, '.') == NULL )		{
		return (1);
	}
	return 0;
}

static int __is_string_datetime(char* str)
{
	/*
	"2020-10-08 12:00:00.000"
	*/
	if ( strlen(str) != 23 )	{
		return 0;
	}

	if ( str[4]!='-' || str[7]!='-' || str[13]!=':' || str[16]!=':' || str[19]!='.')	{
		return 0;
	}

	return 1;
}

/**
	IMPacket req;

	im_resource_notification_init()
	im_resource_notification_append_properties()
	im_resource_notification_send()

	im_sys_resource_notification_init()
	im_sys_resource_notification_append_properties()
	im_sys_resource_notification_send()
*/

/////////////////////////////////////
// Device Resource
int im_resource_notification_init(im_client_tPtr cli)
{
#if 0 // 2022-03-24
	if ( __is_string_valid(cli->channelAccessToken) != (1) )
	{
	  ERR("Invalid cli->channelAccessToken");
	  return -1;
	}
#endif

#ifdef HAVE_THREAD
	unsigned long tid = thread_getid();
	IMPacketPtr nofi_req = (IMPacketPtr)im_resource_pkt_get_by_tid(cli, tid);
	if ( nofi_req == NULL )
	{
		return -1;
	}
	DBG("packet411 tid=[%ld], nofi_req=[%p]", tid, nofi_req);
#else
	IMPacketPtr nofi_req = (IMPacketPtr)(cli->res_nofi_pkt);
#endif

	im_packet_release(nofi_req); 

	int rc = im_packet411_req_init(nofi_req, cli->channelAccessToken);

	if ( rc < 0 )    {
	  ERR("im_packet411_req_init()");
	  return -1;
	}
	
	im_head_put_ext_str_val(&nofi_req->head, "operation", "UPDATE");
	
	im_head_put_ext_str_val(&nofi_req->head, "to", "/devices/notifications");

	return 0;
}


int im_resource_notification_append_resource(im_client_tPtr cli, char *resource_id, char *properties_in_json_str)
{
	if ( __is_string_valid(cli->dev_id) != (1) )
	{
	  ERR("Invalid cli->dev_id");
	  return -1;
	}

	if ( __is_string_valid(resource_id) != (1) )
	{
	  ERR("Invalid resource_id");
	  return -1;
	}

	if ( __is_string_valid(properties_in_json_str) != (1) )
	{
	  ERR("Invalid properties_in_json_str");
	  return -1;
	}

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
	
	rc = im_body411_req_append_resource_representation(&nofi_req->body, cli->dev_id, resource_id, j_properties);
	
	if ( rc < 0 )    {
	  ERR("im_body411_req_new_resource_representation()");
	  json_value_free(j_properties);
	  im_packet_release(nofi_req);
	  return -1;
	}

	json_value_free(j_properties);
	
	return 0;
}

int im_resource_notification_send(im_client_tPtr cli)
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

	circuit_breaker(cli);

    if ( cli->response_wait_sw == (1) )
    {
		rc = im_transact_packet_with_response_handler(cli, nofi_req, response_packet_handler_411);
    } else {
        // 2020-04-16 : 응답미대기
        rc = im_transact_packet_with_response_handler(cli, nofi_req, NULL);
    }


    if ( rc < 0 )    {
        ERR("fail im_transact_packet_with_response_handler()");
        im_packet_release(nofi_req);
        return -1;
      }

  im_packet_release(nofi_req);
  return 0;
}


//
/////////////////////////////////////////////////////////


int im_resource_send_notification(im_client_tPtr cli, char *resource_id, char *properties_in_json_str)
{
	int rc = -1;

	rc = im_resource_notification_init(cli);
    if ( rc < 0 )    {
        ERR("fail im_resource_notification_init()");
        return -1;
      }

	rc = im_resource_notification_append_resource(cli, resource_id, properties_in_json_str);
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



///////////////////////////////////////////////////////////////////
/** 525 제어수신 */
///////////////////////////////////////////////////////////////////
#if 1 // 2021-03-05 deprecated
int im_set_report_data_on_dev(IMPacketBodyPtr body, char *json_str, char *resource_id, char *dev_id)
{
  return 0;
}
#else
int im_if525_rep_set_data_in_json(IMPacketBodyPtr body, char *resource_id, char *json_str)
{
  return 0;
}
#endif



static int __parse_resource_url(char *resource_url, char *o_dev_id, char *o_resource_id)
{
	/*
	"/devices/{dev_id}/resources"			// 전체 리소스
	"/devices/{dev_id}/resources/{res_id}"	// 특정 리소스
	"/devices/testbyw111/resources/testAllTypeUri"
	*/
	char *dev_tok = "/devices/";
	char *res_tok = "/resources/";
	char *res_one_tok = "/resources";

	char *chBegin, *chEnd;

	if ( strstr(resource_url, dev_tok) == NULL )	{
		ERR("No devices in url");
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
	chBegin = resource_url + strlen(dev_tok);
	chEnd = strstr(resource_url, res_tok);

	memcpy(o_dev_id, chBegin, (chEnd-chBegin));
	o_dev_id[(chEnd-chBegin)] = '\0';

	// Match resource_id
	chBegin = strstr(resource_url, res_tok) + strlen(res_tok);
	chEnd = resource_url + strlen(resource_url);

	memcpy(o_resource_id, chBegin, (chEnd-chBegin));
	o_resource_id[(chEnd-chBegin)] = '\0';

	return 0;
}


static void __resource_set_request_default_handler(IMPacketBodyPtr pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	DBG("origin dev_id[%s],resource_id[%s]", dev_id, resource_id);
	DBG("properties_in_jstr = %s", properties_in_jstr);
	/*
	DBG("ectestBoolean = %d", im_body_get_value_bool(pktBody, "ectestBoolean"));
	*/
}

void im_packet525_request_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char dev_id[64];
	char resource_id[64];

	char* messageId = im_head_get_ext_messageId(&req->head);
	if ( messageId == NULL )	{
		ERR("messageId is NULL");
		return;
	}

	char *resource_url = im_pktHeadExt_get_val(&req->head, "to");
	if ( resource_url == NULL )	{
		ERR("resource_url is NULL");
		return;
	}

	rc = __parse_resource_url(resource_url, dev_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url dev_id=[%s],  resource_id=[%s]", dev_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready ReportPacket
	/////////////////////////////////////////
	IMPacket rep;

	rc = im_packet525_rep_init(&rep, cli->channelAccessToken, messageId);
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
	if ( cli->OnResourceSetRequestHandler == NULL )
	{
		__resource_set_request_default_handler(&req->body, dev_id, resource_id, properties_in_jstr);
		im_head_set_ext_responseCode(&rep.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnResourceSetRequestHandler(&req->body, dev_id, resource_id, properties_in_jstr);
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
void im_packet711_request_one_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char dev_id[64];
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

	rc = __parse_resource_url(resource_url, dev_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url dev_id=[%s],  resource_id=[%s]", dev_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready response Packet
	/////////////////////////////////////////
	IMPacket res;

	rc = im_packet711_res_one_init(&res, cli->channelAccessToken, messageId);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet711_res_one_init()");
		ERR("====================================");
		return;
	}

	/////////////////////////////////////////
	// 2. 콜백처리 
	/////////////////////////////////////////
	if ( cli->OnResourceRetrieveOneRequestHandler == NULL )
	{
		im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnResourceRetrieveOneRequestHandler(&res.body, dev_id, resource_id);
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
void im_packet711_request_all_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
	int rc = -1;
	char dev_id[64];
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

	rc = __parse_resource_url(resource_url, dev_id, resource_id);
	if ( rc < 0 )
	{
		ERR("__parse_resource_url(); to = %s", resource_url);
		return;
	}

	DBG("resource_url dev_id=[%s],  resource_id=[%s]", dev_id, resource_id);

	/////////////////////////////////////////
	// 1. set Ready response Packet
	/////////////////////////////////////////
	IMPacket res;

	rc = im_packet711_res_all_init(&res, cli->channelAccessToken, messageId);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet711_res_one_init()");
		ERR("====================================");
		return;
	}

	/////////////////////////////////////////
	// 2. 콜백처리 
	/////////////////////////////////////////
	if ( cli->OnResourceRetrieveAllRequestHandler == NULL )
	{
		im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_4005_MethodNotAllowed_STR);
	} else {
		int retCode = cli->OnResourceRetrieveAllRequestHandler(&res.body, dev_id);
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
