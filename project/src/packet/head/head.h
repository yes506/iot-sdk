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



#ifndef IOTMAKERS_PKTHEAD_H
#define IOTMAKERS_PKTHEAD_H

#include "packet/head/head_ext.h"

#define IOTMAKERS_DEF_HEAD_LEN			23

#define IOTMAKERS_MESSAGE_VER_MAJOR		0x01
#define IOTMAKERS_MESSAGE_VER_MINOR		0x00


typedef enum {
	MessageType_NONE					= 0x00
	, MessageType_AUTH_DEV 				= 0x01	// 장치인증; 장치-->
	, MessageType_AUTH_SYS 				= 0x02	// 시스템인증; 장치-->
	, MessageType_KeepAlive				= 0x03	// 세션유지; 장치-->
	, MessageType_DevResourceNoti		= 0x17	// 리소스통지(수집전송); 장치-->
	, MessageType_DevResourceUpdate		= 0x14	// 리소스설정요청(장치제어); 장치<--
	, MessageType_DevResourceRetrieveOne	= 0x13	// 특정리소스조회요청(최종값); 장치<--
	, MessageType_DevResourceRetrieveAll	= 0x12	// 전체리소스조회요청(최종값); 장치<--
	, MessageType_SysResourceNoti		= 0x56	// 리소스통지(수집전송); 장치-->
	, MessageType_SysResourceUpdate		= 0x53	// 리소스설정요청(장치제어); 장치<--
	, MessageType_SysResourceRetrieveOne	= 0x52	// 특정리소스조회요청(최종값); 장치<--
	, MessageType_SysResourceRetrieveAll	= 0x51	// 전체리소스조회요청(최종값); 장치<--
} MessageType;

typedef enum {
	ExchangeType_NONE			= 0x00
	, ExchangeType_Request 		= 0x01
	, ExchangeType_Response 	= 0x02
	, ExchangeType_Report 		= 0x03
} ExchangeType;

typedef enum {
	HeaderContentType_NONE			= 0x00
	, HeaderContentType_JSON 		= 0x01
} HeaderContentType;



#pragma pack(push, 1)
typedef struct {
	unsigned char			messageVersionMajor;
	unsigned char			messageVersionMinor;

	unsigned char			messageType;
	unsigned char			exchangeType;

	unsigned char			channelAccessToken[16];

	unsigned char			headerContentType;

	unsigned short			extHeaderLen;					// 확장헤더 가변길이
} IMHeadRaw, *IMHeadRawPtr;
#pragma pack(pop)


typedef struct {
	IMHeadRaw	headRaw;
	
	int			ext_cnt;	// 헤더확장필드수
	IMHeadExt	ext;

	int			len;	// 헤더 총 길이
} IMPacketHead, *IMPacketHeadPtr;




#ifdef __cplusplus
extern "C"
{
#endif

// head.c 
void im_head_init(IMPacketHeadPtr head);
void im_head_init_oneway_req(IMPacketHeadPtr head);
void im_head_init_response(IMPacketHeadPtr head);
void im_head_init_report(IMPacketHeadPtr head);
void im_head_release(IMPacketHeadPtr head);

int im_pktHeadExt_get_count(IMPacketHeadPtr head);
int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val);
char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key);
int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) ;
int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) ;
int im_head_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len);
int im_head_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff);

char* im_head_get_channelAccessToken(IMPacketHeadPtr head);


void im_head_set_ext_messageId(IMPacketHeadPtr head, char *messageIdStr);
void im_head_set_ExchangeType(IMPacketHeadPtr head, ExchangeType exchangeType);
void im_head_set_MessageType(IMPacketHeadPtr head, MessageType messageType);
void im_head_set_channelAccessToken(IMPacketHeadPtr head, char *authCode);
void im_head_set_ext_responseCode(IMPacketHeadPtr head, char *responseCode);
MessageType im_head_get_MessageType(IMPacketHeadPtr head);


// head_all_init.c
int im_head000_req_release(IMPacketHeadPtr head);
int im_head224_req_init(IMPacketHeadPtr head );
int im_head231_req_init(IMPacketHeadPtr head, char *auth_code );
int im_head332_req_init(IMPacketHeadPtr head, char *auth_code );
int im_head411_req_init(IMPacketHeadPtr head, char *auth_code );

int im_head224sys_req_init(IMPacketHeadPtr head );
int im_head411sys_req_init(IMPacketHeadPtr head, char *auth_code );





int im_head525_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head525_rep_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711_res_one_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711_res_all_init(IMPacketHeadPtr head, char *auth_code, char *msgid );

int im_head525sys_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head525sys_rep_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711sys_res_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711sys_res_one_init(IMPacketHeadPtr head, char *auth_code, char *msgid );
int im_head711sys_res_all_init(IMPacketHeadPtr head, char *auth_code, char *msgid );


// head_ext.c
int im_headExt_init(IMHeadExtPtr pHExt) ;
int im_headExt_init_with_string(IMHeadExtPtr pHExt, char *json_str) ;
void im_headExt_release(IMHeadExtPtr pHExt) ;
int im_headExt_put_kvp_str(IMHeadExtPtr pHExt, char *key, char *val) ;
char* im_headExt_get_kvp_str(IMHeadExtPtr pHExt, char *key) ;
char* im_headExt_get_kvp_name_with_index(IMHeadExtPtr pHExt, int idx) ;
int im_packet_head_ext_get_serialized_strlen(IMHeadExtPtr pHExt) ;
char* im_packet_head_ext_get_serialized_str(IMHeadExtPtr pHExt) ;
void jsonwrap_print_value_serialized(JSON_Value* json_val);


int im_head_put_ext_str_val(IMPacketHeadPtr head, char *key, char *val);


#ifdef __cplusplus
}
#endif






#endif

