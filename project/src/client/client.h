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



#ifndef IOTMAKERS_CLIENT_H
#define IOTMAKERS_CLIENT_H

#include <stdio.h>
#include "platform.h"
#include "kt_iot_log.h"
#include "packet/packet.h"
#include "netio/net_context.h"
#include "util/thread.h"



#define IM_CLIENT_VERSION       "1.1.10 2022-08-01"



#define IOTMAKERS_KEEPALIVE_IN_SEC          50
#define IOTMAKERS_MAX_CONTROL_DATA          32
#define IOTMAKERS_NET_READ_TIMEOUT_IN_SEC   3


/**
* 2021-03-05
*/
#define IM_RESP_CODE_2000_OK			2000
#define IM_RESP_CODE_2004_Changed		2004
#define IM_RESP_CODE_4000_BadRequest	4000
#define IM_RESP_CODE_4004_NotFound		4004
#define IM_RESP_CODE_5000_InternalError	5000


/*
개방형 simple api용 : 수집전송 주기를 최대 1초로 제한
*/
#define IOTMAKERS_CIRCUIT_BREAKER_IN_SEC  1


#define IM_RESOURCE_PACKET_POOL_SIZE   64

typedef struct im_resource_pkt_pool_t {

	unsigned long tid[IM_RESOURCE_PACKET_POOL_SIZE];
	
	void *pkt[IM_RESOURCE_PACKET_POOL_SIZE];	

} im_resource_pkt_pool_t, *im_resource_pkt_pool_tPtr;

/*
 * IoTMakers Client Instance
 *
 */
typedef struct im_client_t {

	char  svc_gw[128];
	char  dev_id[128];
	char  channelAccessToken[64+1];
	int isChAthnSuccess;
	
	// 리소스 업데이트 통지용 송신 버퍼
	// IMPacket / im_resource_notification_init()
#ifdef HAVE_THREAD
	// Thread 지원
	im_resource_pkt_pool_t  resource_pkt_pool;
#else
	void *res_nofi_pkt;	
#endif


	int (*OnResourceSetRequestHandler)(void* pktBody, char *dev_id, char *resource_id, char *properties_in_jstr);

	int (*OnResourceRetrieveOneRequestHandler)(void* pktBody, char *dev_id, char *resource_id);
	
	int (*OnResourceRetrieveAllRequestHandler)(void* pktBody, char *dev_id);

	int (*OnSysResourceSetRequestHandler)(void* pktBody, char *dev_id, char *resource_id, char *properties_in_jstr);

	int (*OnSysResourceRetrieveOneRequestHandler)(void* pktBody, char *dev_id, char *resource_id);
	
	int (*OnSysResourceRetrieveAllRequestHandler)(void* pktBody, char *dev_id);


  /*
    장치제어 요청 핸들러
  */
  int failControlCnt;

  /*
  개방형 simple api용 : 수집전송 주기를 최대 1초로 제한
  */
  int circuit_breaker_timestamp;

  connection_t conn_ctx;
  unsigned long packet_count_out;
  unsigned long packet_count_in;
  char *tls_ca_file;
  char *tls_ca_path;

  /*
  TCP 세션유지용
  */
  int keepalive_timestamp;
  int packet_count_out_check;


  /*
  circuit_breaker ON/OFF 스위치 (기본값:ON)
  */
  int circuit_breaker_sw;

  /*
  411 응답대기 ON/OFF 스위치 (기본값:ON)
  2020-04-17
  */
  int response_wait_sw;

    /*
    Java JNI사용시에 JNIEnv *env를 저장    
    */
    void *host_evn;
    void *host_object;


#ifdef HAVE_THREAD
	mutex_type mutex_sock_send;
#endif	

} im_client_t, *im_client_tPtr;




#ifdef __cplusplus
extern "C"
{
#endif


void* im_resource_pkt_get_by_tid(im_client_tPtr cli, unsigned long tid);
int im_resource_pkt_release_by_tid(im_client_tPtr cli, unsigned long tid);

// client.c
int im_client_set_loglevel(int level);
int im_client_init(im_client_tPtr cli);
void im_client_release(im_client_tPtr cli);
int im_client_set_tls_cafile(im_client_tPtr cli, char* ca_file);
int im_client_set_tls_capath(im_client_tPtr cli, char* ca_path);
int im_turn_circuit_breaker_off(im_client_tPtr cli);
int im_turn_response_wait_off(im_client_tPtr cli);
int im_connect_to_via_srcip(im_client_tPtr cli, char *ip_str, int port, char *srcip_str);
int im_connect_to(im_client_tPtr cli, char *ip_str, int port);
int im_connect(im_client_tPtr cli);
void im_disconnect(im_client_tPtr cli);

int im_poll(im_client_tPtr cli);
int im_transact_packet_with_response_handler(im_client_tPtr cli, IMPacketPtr req, int (*response_packet_handler)(im_client_tPtr, IMPacketPtr));
int im_auth_device_with_nic_desc(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw, char *mac_addr, char *nic_desc);
int im_auth_device(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw);

int im_auth_system_with_nic_desc(im_client_tPtr cli, char *chn_id, char *cnh_pw, char *sys_id, char *mac_addr, char *nic_desc);
int im_auth_system(im_client_tPtr cli, char *chn_id, char *cnh_pw, char *sys_id);

int im_session_keepalive(im_client_tPtr cli);

char* im_head_get_ext_messageId(IMPacketHeadPtr head);
char* im_head_get_ext_responseCode(IMPacketHeadPtr head);

// packet_io.c
int im_conn_send_packet(im_client_tPtr cli, IMPacketPtr pkt);
int im_conn_recv_packet(im_client_tPtr cli, IMPacketPtr pkt);

// simple_api.c
char* im_head_get_channelAccessToken(IMPacketHeadPtr head);

void im_packet525_request_handler_simple(im_client_tPtr cli, IMPacketPtr req);
void im_packet711_request_one_handler_simple(im_client_tPtr cli, IMPacketPtr req);
void im_packet711_request_all_handler_simple(im_client_tPtr cli, IMPacketPtr req);

void im_packet525sys_request_handler_simple(im_client_tPtr cli, IMPacketPtr req);
void im_packet711sys_request_one_handler_simple(im_client_tPtr cli, IMPacketPtr req);
void im_packet711sys_request_all_handler_simple(im_client_tPtr cli, IMPacketPtr req);

int im_resource_notification_init(im_client_tPtr cli);
int im_resource_notification_append_resource(im_client_tPtr cli, char *resource_id, char *properties_in_json_str);
int im_resource_notification_send(im_client_tPtr cli);
int im_resource_send_notification(im_client_tPtr cli, char *resource_id, char *properties_in_json_str);


int im_sys_resource_notification_init(im_client_tPtr cli);
int im_sys_resource_notification_append_resource(im_client_tPtr cli, char *resource_id, char *properties_in_json_str);
int im_sys_resource_notification_send(im_client_tPtr cli);
int im_sys_resource_send_notification(im_client_tPtr cli, char *resource_id, char *properties_in_json_str);


int im_resource_retrieve_set_resource(IMPacketBodyPtr body, char *resource_id, char *properties_in_json_str);
int im_resource_retrieve_append_resource(IMPacketBodyPtr body, char *resource_id, char *properties_in_json_str);



// netio
void netio_set_read_head_length_4(connection_tPtr ctx);
int net_udp_send_buffer(connection_tPtr ctx);
int net_udp_recv_buffer(connection_tPtr ctx);
int net_udp_available_timeout(connection_tPtr ctx, int seconds);
int net_udp_available(connection_tPtr ctx);

// util
long im_util_gettimeofday_as_sec();

#ifdef __cplusplus
}
#endif


#endif
