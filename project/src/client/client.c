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

#include "kt_iot_log.h"
#include "util/util.h"
#include "client/client.h"
#include "packet/head/head.h"
#include "packet/body/body.h"
#include "packet/packet.h"


int g_log_level = 3;


#ifdef HAVE_THREAD
//////////////////////////////////////////////////////////////////////////
//
static int im_init_resource_pkt_pool(im_client_tPtr cli)
{
  int idx;
  im_resource_pkt_pool_tPtr pool = &cli->resource_pkt_pool;
  
  for (idx = 0; idx < IM_RESOURCE_PACKET_POOL_SIZE; idx++)  {
      pool->tid[idx] = 0;
      pool->pkt[idx] = malloc(sizeof(IMPacket));
	  if ( pool->pkt[idx] == NULL )
	  {
		  ERR ("Out Of Memory");
		  return -1;
	  }

	im_packet_init((IMPacketPtr)(pool->pkt[idx]));

  }

  return 0;
}


static void im_release_resource_pkt_pool(im_client_tPtr cli)
{
  int idx;
  im_resource_pkt_pool_tPtr pool = &cli->resource_pkt_pool;
  
  for (idx = 0; idx < IM_RESOURCE_PACKET_POOL_SIZE; idx++)  {
      pool->tid[idx] = 0;
	  if ( pool->pkt[idx] != NULL )
	  {
		im_packet_release(pool->pkt[idx]);

		free(pool->pkt[idx]);
	  }
  }
}

static int im_found_resource_pkt_pool_by_tid(im_client_tPtr cli, unsigned long tid)
{
	int idx;
	im_resource_pkt_pool_tPtr pool = &cli->resource_pkt_pool;

	// 기존 할당된 슬랏 검색
    for (idx = 0; idx < IM_RESOURCE_PACKET_POOL_SIZE; idx++)  {
		if ( pool->tid[idx] == tid )
		{
			return idx;
		}
	}

	// 새 슬랏 할당
    for (idx = 0; idx < IM_RESOURCE_PACKET_POOL_SIZE; idx++)  {
		if ( pool->tid[idx] == 0 )
		{
			pool->tid[idx] = tid;
			return idx;
		}
	}

	ERR("Out Of Resource Packet Pool");

	return -1;
}



void* im_resource_pkt_get_by_tid(im_client_tPtr cli, unsigned long tid)
{
	im_resource_pkt_pool_tPtr pool = &cli->resource_pkt_pool;

	int idx = im_found_resource_pkt_pool_by_tid(cli, tid);
	if ( idx >= 0 )
	{
		return pool->pkt[idx];
	} 

	return NULL;
}

int im_resource_pkt_release_by_tid(im_client_tPtr cli, unsigned long tid)
{
	im_resource_pkt_pool_tPtr pool = &cli->resource_pkt_pool;

	int idx = im_found_resource_pkt_pool_by_tid(cli, tid);
	if ( idx < 0 )
	{
		ERR("Not Found Slot; Resource Packet Pool");
		return -1;
	} 
	
	pool->tid[idx] = 0;

	return 0;
}
#endif

//////////////////////////////////////////////////////////////////////////
int im_client_set_loglevel(int level)
{
  g_log_level = level;
  return 0;
}

int im_client_init(im_client_tPtr cli)
{
	INF("IoTMakers 3rdParty Device SDK ver%s", IM_CLIENT_VERSION);
	
	memset(cli, 0x00, sizeof(im_client_t));
	cli->keepalive_timestamp = im_util_gettimeofday_as_sec(); 
	cli->circuit_breaker_sw = (1); 
	cli->response_wait_sw = (1); 


	cli->OnResourceSetRequestHandler = NULL;
	cli->OnResourceRetrieveOneRequestHandler = NULL;
	cli->OnResourceRetrieveAllRequestHandler = NULL;

	
#ifdef HAVE_THREAD
	cli->mutex_sock_send = thread_create_mutex();

	if ( im_init_resource_pkt_pool(cli) < 0 )
	{
		im_release_resource_pkt_pool(cli);
		return -1;
	}
#else
	cli->res_nofi_pkt = malloc(sizeof(IMPacket));
	im_packet_init((IMPacketHeadPtr)cli->res_nofi_pkt);
#endif

	return 0;
}

void im_client_release(im_client_tPtr cli)
{

#ifdef HAVE_THREAD
	thread_destroy_mutex(cli->mutex_sock_send);
	im_release_resource_pkt_pool(cli);
#else
	if ( cli->res_nofi_pkt != NULL )
	{
		im_packet_release(cli->res_nofi_pkt);
		free(cli->res_nofi_pkt);
	}
#endif	

}



int im_client_set_tls_cafile(im_client_tPtr cli, char* ca_file)
{
  cli->tls_ca_file = ca_file;
  cli->tls_ca_path = NULL;
  return 0;
}

int im_client_set_tls_capath(im_client_tPtr cli, char* ca_path)
{
  cli->tls_ca_file = NULL;
  cli->tls_ca_path = ca_path;
  return 0;
}


int im_turn_circuit_breaker_off(im_client_tPtr cli)
{
  cli->circuit_breaker_sw = (0); 
  return 0;
}

int im_turn_response_wait_off(im_client_tPtr cli)
{
  cli->response_wait_sw = (0); 
  return 0;
}


int im_connect_to(im_client_tPtr cli, char *ip_str, int port)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsa_data;
	WSAStartup(wVersionRequested, &wsa_data);
#endif

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  if ( cli->tls_ca_file != NULL )    {
      net_tls_set_trusred_ca_file(conn_ctx, cli->tls_ca_file) ;
  }

  if ( cli->tls_ca_path != NULL )    {
      net_tls_set_trusred_ca_path(conn_ctx, cli->tls_ca_path) ;
  }

  int rc = net_tls_open_socket(conn_ctx);
  if ( rc < 0 )  {
    ERR("fail net_tls_open_socket()");
    return -1;
  }

  rc = net_tls_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tls_connect_ipv4()");
    return -1;
  }

#elif HAVE_MQTT
  int rc = net_mqtt_connect(conn_ctx, ip_str, port, IOTMAKERS_KEEPALIVE_IN_SEC);
  if ( rc < 0 )  {
    ERR("fail net_mqtt_connect()");
    return -1;
  }

#elif HAVE_HTTP
  int rc = net_http_connect(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_http_connect()");
    return -1;
  }
  return 0;

#else
  int rc = net_tcp_open_socket(conn_ctx);
  if ( rc < 0 )  {
    ERR("fail net_tcp_open_socket()");
    return -1;
  }

  rc = net_tcp_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#endif

  return 0;
}


int im_connect_to_via_srcip(im_client_tPtr cli, char *ip_str, int port, char *srcip_str)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsa_data;
	WSAStartup(wVersionRequested, &wsa_data);
#endif

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  int rc = net_tls_open_socket_n_bind_with_ip(conn_ctx, srcip_str);
  if ( rc < 0 )  {
    ERR("fail net_tls_init()");
    return -1;
  }

  rc = net_tls_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#else
  int rc = net_tcp_open_socket_n_bind_with_ip(conn_ctx, srcip_str);
  if ( rc < 0 )  {
    ERR("fail net_tcp_open_socket()");
    return -1;
  }

  rc = net_tcp_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#endif
  return 0;
}



#if 0
/**
  개방형IoT플랫폼 (deprecated)
*/
#define IM_EC_IP  "220.90.216.90"

#ifdef HAVE_MQTT
#define IM_EC_PORT  10030
#elif HAVE_HTTP
#define IM_EC_PORT  10010
#else 
#define IM_EC_PORT  10020
#endif

int im_connect(im_client_tPtr cli)
{
  return im_connect_to(cli, IM_EC_IP, IM_EC_PORT);
}
#endif





void im_disconnect(im_client_tPtr cli)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  net_tls_disconnect(conn_ctx);

#elif HAVE_MQTT
  net_mqtt_disconnect(conn_ctx);

#elif HAVE_HTTP
  net_http_disconnect(conn_ctx);

#else
  net_tcp_disconnect(conn_ctx);
  net_tcp_close_socket(conn_ctx);
#endif

#ifdef WIN32
	WSACleanup();
#endif
}



////////////////////////////////////////////////////////////
static void request_packet525_handler(im_client_tPtr cli, IMPacketPtr req);
static void request_packet711_one_handler(im_client_tPtr cli, IMPacketPtr req);
static void request_packet711_all_handler(im_client_tPtr cli, IMPacketPtr req);

static void request_packet525sys_handler(im_client_tPtr cli, IMPacketPtr req);
static void request_packet711sys_one_handler(im_client_tPtr cli, IMPacketPtr req);
static void request_packet711sys_all_handler(im_client_tPtr cli, IMPacketPtr req);

static void packet_handler(im_client_tPtr cli, IMPacketPtr ppkt)
{
  int reqMessageType = im_head_get_MessageType(&ppkt->head);

  switch (reqMessageType)	{
    case MessageType_DevResourceUpdate:	// 장치제어
      request_packet525_handler(cli, ppkt);
      break;
    case MessageType_SysResourceUpdate:	// 장치제어
      request_packet525sys_handler(cli, ppkt);
      break;
    case MessageType_DevResourceRetrieveOne:	// 특정 리소스 조회
      request_packet711_one_handler(cli, ppkt);
      break;
    case MessageType_SysResourceRetrieveOne:	// 특정 리소스 조회
      request_packet711sys_one_handler(cli, ppkt);
      break;
    case MessageType_DevResourceRetrieveAll:	// 전체 리소스 조회
      request_packet711_all_handler(cli, ppkt);
      break;
    case MessageType_SysResourceRetrieveAll:	// 전체 리소스 조회
      request_packet711sys_all_handler(cli, ppkt);
      break;
    case MessageType_DevResourceNoti:	// 수집전송에 대한 응답; 이게 여기로 들어오면 안됨.
    case MessageType_SysResourceNoti:	// 수집전송에 대한 응답; 이게 여기로 들어오면 안됨.
      DBG("Res for reqMessageType[0x%x]", reqMessageType);
      break;
    
    default:
    WRN("Not Supported methodType [%d]", reqMessageType);
  }

}


static int process_packet_from_server(im_client_tPtr cli)
{
  IMPacket pkt;
  IMPacketPtr ppkt = (IMPacketPtr)&pkt;

  int rc = im_packet_init(ppkt);
  if ( rc < 0 )    {
    ERR("im_packet_init()");
    return -1;
  }

  rc = im_conn_recv_packet(cli, ppkt);
  if ( rc < 0 )  {
    im_packet_release(ppkt);
    return -1;
  }

  // Call response_packet_handler()
  packet_handler(cli, ppkt);

  im_packet_release(ppkt);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

int im_poll(im_client_tPtr cli)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;
  int rc;

#if !defined(HAVE_MQTT) && !defined(HAVE_HTTP)

  if ( IOTMAKERS_KEEPALIVE_IN_SEC > 0 )
  {
    // 최근 IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC 이내에
    if ( (im_util_gettimeofday_as_sec() - cli->keepalive_timestamp) >  IOTMAKERS_KEEPALIVE_IN_SEC) 
    {
      cli->keepalive_timestamp = im_util_gettimeofday_as_sec(); 

	  // 패킷 송신 내역이 없으면, 또는 No wait 모드인 경우 세션 체크
      if ( cli->packet_count_out_check == cli->packet_count_out 
		  || cli->response_wait_sw == (0) )        {
        // 접속유지 요청
        if ( im_session_keepalive(cli) < 0 )        {
          return -1;
        };
      } 

      cli->packet_count_out_check = cli->packet_count_out; 
    }
  }

#endif

check_again:

#ifdef HAVE_THREAD
	thread_lock_mutex(cli->mutex_sock_send);
#endif	
  // 최대 1 msec 동안 블로킹됨
  rc = netio_available(conn_ctx);
  if ( rc < 0 ) {
    ERR("fail sock");
#ifdef HAVE_THREAD
	thread_unlock_mutex(cli->mutex_sock_send);
#endif	
    return -1;
  }
#ifdef HAVE_THREAD
	thread_unlock_mutex(cli->mutex_sock_send);
#endif	

  if ( rc == 0 ) {
    //DBG("no packet to read");
    DBG("sent=[%ld]pkts, recv=[%ld]pkts", cli->packet_count_out, cli->packet_count_in);
    return 0;
  }

  rc = process_packet_from_server(cli);
  if ( rc < 0 ) {
    ERR("fail process_packet_from_server()");
    return -1;
  }

  goto check_again;

  return 0;
}


int im_poll_loop(im_client_tPtr cli)
{
	if ( cli == NULL )
	{
		ERR("fail im_client_tPtr is NULL");
		return -1;
	}

	while ( im_poll(cli) >= 0)
	{
		;
	}
	
}


int im_transact_packet_with_response_handler(im_client_tPtr cli, IMPacketPtr req, 
                                              int (*response_packet_handler)(im_client_tPtr, IMPacketPtr))
{
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;


  int rc = im_conn_send_packet(cli, req);
  if ( rc < 0 )  {
    return -1;
  }

  if ( response_packet_handler == NULL )  {
    // 응답수신 불필요  
    // im_poll()에서 처리됨
    return 0;
  }

wait_data:
  // IOTMAKERS_NET_READ_TIMEOUT_IN_SEC 동안 블로킹
#ifdef HAVE_THREAD
	thread_lock_mutex(cli->mutex_sock_send);
    DBG("thread_lock_mutex()");
#endif	
  if ( netio_available_timeout(pconn_ctx, IOTMAKERS_NET_READ_TIMEOUT_IN_SEC) <= 0 ) {
    ERR("no data to read");
#ifdef HAVE_THREAD
	thread_unlock_mutex(cli->mutex_sock_send);
    DBG("thread_unlock_mutex()");
#endif	
    return -1;
  }
#ifdef HAVE_THREAD
	thread_unlock_mutex(cli->mutex_sock_send);
    DBG("thread_unlock_mutex()");
#endif	


  IMPacket res;
  IMPacketPtr pres = (IMPacketPtr)&res;

  rc = im_packet_init(pres);
  if ( rc < 0 )    {
    im_packet_release(req);
    ERR("im_packet_init()");
    return -1;
  }

  rc = im_conn_recv_packet(cli, pres);
  if ( rc < 0 )  {
    im_packet_release(req);
    ERR("im_conn_recv_packet()");
    return -1;
  }

#ifndef HAVE_HTTP
  char* req_trx = im_head_get_ext_messageId(&req->head) ;
  char* res_trx = im_head_get_ext_messageId(&pres->head) ;
  
  //DBG("MessageID: [%s] [%s]", req_trx, res_trx);

  if ( res_trx == NULL )  {
    WRN("No MessageID in Respose; drop packet");
    im_packet_release(pres);
    goto wait_data;
  }

  if ( strcmp(req_trx, res_trx) != 0 )  {
    WRN("MessageID misMatch; [%s][%s]", req_trx, res_trx);

	// 트랜잭션ID가 다르므로 별도의 핸들러에서 처리
    packet_handler(cli, pres);

    im_packet_release(pres);
    goto wait_data;
  }
#endif

  // Call response_packet_handler() back
  rc = response_packet_handler(cli, pres);

  im_packet_release(pres);

  return rc;
}


///////////////////////////////////////////////////////////////////
/** 224 장치 인증 */
///////////////////////////////////////////////////////////////////
static int response_packet_handler_224(im_client_tPtr cli, IMPacketPtr res)
{
	char* responseCode = im_head_get_ext_responseCode(&res->head) ;
	WRN("responseCode=[%s]", responseCode);
	if ( strcmp(IM_RESP_CODE_2000_OK_STR, responseCode) != 0 )	{
		return (-1);
	}

	memcpy(cli->channelAccessToken, (char*)im_head_get_channelAccessToken(&res->head), 16);
	WRN("channelAccessToken[0]=[0x%x]", (unsigned char)*cli->channelAccessToken);
	cli->isChAthnSuccess = (1);

#ifdef HAVE_HTTP
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;
  net_http_set_auth_tok(conn_ctx, cli->channelAccessToken);
#endif

	return 0;
}



int im_auth_device_with_nic_desc(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw, char *mac_addr, char *nic_desc)
{
  IMPacket req;

  // 테스트용 주석처리
  // int rc = im_packet224_req_init(&req);
  // if ( rc < 0 )    {
  //   ERR("im_packet_224_req_init()");
  //   return -1;
  // }

	// set device id
	char extMsg_to[64];
	sprintf(extMsg_to, "/devices/%s/sessions", dev_id);
	im_head_put_ext_str_val(&req.head, "to", extMsg_to);

	// set device pw
	im_body224_req_set_athnRqtNo(&req.body, dev_pw);

#if 0
  if ( mac_addr != NULL)  {
    im_body224_req_set_macAdr(&req.body, mac_addr);
  }

  if ( nic_desc != NULL)  {
    im_body224_req_set_sessnDesc(&req.body, nic_desc);
  }
#endif

  cli->isChAthnSuccess = (0);

  // 테스트용 주석처리
  // rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_224);
  // if ( rc < 0 )    {
  //   ERR("fail im_transact_packet_with_response_handler()");
  //   ERR("auth_device seems to be failed");
  //   im_packet_release(&req);
  //   return -1;
  // }

  im_packet_release(&req);
  
  // 테스트용 주석처리
  // if ( cli->isChAthnSuccess == (0) )  {
  //   ERR("fail auth_device");
  //   return -1;
  // }

  strcpy(cli->dev_id, dev_id);
  strcpy(cli->svc_gw, svc_gw);

  /* 
    set for netio module to read first 4byte for length
  */
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;
  netio_set_read_head_length_4(pconn_ctx);

  return 0;
}


int im_auth_device(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

  IMPacket req;

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, dev_id);
#endif

  int rc = im_auth_device_with_nic_desc(cli, dev_id, dev_pw, svc_gw, NULL, NULL);
  if ( rc < 0 )    {
    ERR("im_auth_device_with_nic_desc()");
    return -1;
  }

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, cli->channelAccessToken);
#endif

  return 0;
}




///////////////////////////////////////////////////////////////////
/** 224 시스템 인증 */
///////////////////////////////////////////////////////////////////
int im_auth_system_with_nic_desc(im_client_tPtr cli, char *chn_id, char *cnh_pw, char *sys_id, char *mac_addr, char *nic_desc)
{
  IMPacket req;

  int rc = im_packet224sys_req_init(&req);
  if ( rc < 0 )    {
    ERR("im_packet_224_req_init()");
    return -1;
  }

	// set device id
	char extMsg_to[64];
	sprintf(extMsg_to, "/systems/%s/sessions", chn_id);
	im_head_put_ext_str_val(&req.head, "to", extMsg_to);

	// set channel pw
	im_body224sys_req_set_athnRqtNo(&req.body, cnh_pw);

#if 0
  if ( mac_addr != NULL)  {
    im_body224_req_set_macAdr(&req.body, mac_addr);
  }

  if ( nic_desc != NULL)  {
    im_body224_req_set_sessnDesc(&req.body, nic_desc);
  }
#endif

  cli->isChAthnSuccess = (0);

  rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_224);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    ERR("auth_system seems to be failed");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  
  if ( cli->isChAthnSuccess == (0) )  {
    ERR("fail auth_system");
    return -1;
  }

  strcpy(cli->dev_id, chn_id);
  strcpy(cli->svc_gw, sys_id);

  /* 
    set for netio module to read first 4byte for length
  */
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;
  netio_set_read_head_length_4(pconn_ctx);

  return 0;
}


int im_auth_system(im_client_tPtr cli, char *chn_id, char *cnh_pw, char *sys_id)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

  IMPacket req;

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, chn_id);
#endif

  int rc = im_auth_system_with_nic_desc(cli, chn_id, cnh_pw, sys_id, NULL, NULL);
  if ( rc < 0 )    {
    ERR("im_auth_device_with_nic_desc()");
    return -1;
  }

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, cli->channelAccessToken);
#endif

  return 0;
}




///////////////////////////////////////////////////////////////////
/** 231 접속유지 */
///////////////////////////////////////////////////////////////////
static int response_packet_handler_231(im_client_tPtr cli, IMPacketPtr res)
{
  // DUMMY
  return 0;
}

int im_session_keepalive(im_client_tPtr cli)
{
  IMPacket req;

  //im_packet231_req_init (장비인증토큰)
  int rc = im_packet231_req_init(&req, cli->channelAccessToken);
  if ( rc < 0 )    {
    ERR("im_packet231_req_init()");
    return -1;
  }

  rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_231);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  
  return 0;
}



///////////////////////////////////////////////////////////////////
// Device Interface
static int im_client_send_525_res_ok(im_client_tPtr cli, char *msgid)
{
  IMPacket res;

  int rc = im_packet525_res_init(&res, cli->channelAccessToken, msgid);
  if ( rc < 0 )    {
    ERR("im_packet_525_res_init()");
    return -1;
  }

  im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_2000_OK_STR);

  // Does not handle response
  rc = im_conn_send_packet(cli, &res);
  if ( rc < 0 )    {
    im_packet_release(&res);
    ERR("im_client_send_packet()");
    return -1;
  }

  im_packet_release(&res);
  return rc;
}


  /** 525 제어요청 handler*/
static void request_packet525_handler(im_client_tPtr cli, IMPacketPtr req)
{
	// 수신 OK 응답
	int rc = im_client_send_525_res_ok(cli, im_head_get_ext_messageId(&req->head));
	if ( rc < 0 )    {
	  ERR("im_client_send_525_res_ok()");
	  return;
	}
	
	// 파싱 & Handler call
	im_packet525_request_handler_simple(cli, req);
	
	return;
}

  /** 711 최종값 조회 요청 handler*/
static void request_packet711_one_handler(im_client_tPtr cli, IMPacketPtr req)
{
    im_packet711_request_one_handler_simple(cli, req);
	return;
}

  /** 711 최종값 조회 요청 handler*/
static void request_packet711_all_handler(im_client_tPtr cli, IMPacketPtr req)
{
    im_packet711_request_all_handler_simple(cli, req);
	return;
}
//
////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// System Interface
///////////////////////////////////////////////////////////////////
static int im_client_send_525sys_res_ok(im_client_tPtr cli, char *msgid)
{
  IMPacket res;

  int rc = im_packet525sys_res_init(&res, cli->channelAccessToken, msgid);
  if ( rc < 0 )    {
    ERR("im_packet_525_res_init()");
    return -1;
  }

  im_head_set_ext_responseCode(&res.head, IM_RESP_CODE_2000_OK_STR);

  // Does not handle response
  rc = im_conn_send_packet(cli, &res);
  if ( rc < 0 )    {
    im_packet_release(&res);
    ERR("im_client_send_packet()");
    return -1;
  }

  im_packet_release(&res);
  return rc;
}


  /** 525 제어요청 handler*/
static void request_packet525sys_handler(im_client_tPtr cli, IMPacketPtr req)
{
	// 수신 OK 응답
	int rc = im_client_send_525sys_res_ok(cli, im_head_get_ext_messageId(&req->head));
	if ( rc < 0 )    {
	  ERR("im_client_send_525_res_ok()");
	  return;
	}
	
	// 파싱 & Handler call
	im_packet525sys_request_handler_simple(cli, req);
	
	return;
}

  /** 711 최종값 조회 요청 handler*/
static void request_packet711sys_one_handler(im_client_tPtr cli, IMPacketPtr req)
{
    im_packet711sys_request_one_handler_simple(cli, req);
	return;
}

  /** 711 최종값 조회 요청 handler*/
static void request_packet711sys_all_handler(im_client_tPtr cli, IMPacketPtr req)
{
    im_packet711sys_request_all_handler_simple(cli, req);
	return;
}
//
////////////////////////////////////////////////////////////////////////////////////

