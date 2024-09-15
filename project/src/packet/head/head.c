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
#include <stdlib.h>
#include <string.h>


#include "kt_iot_log.h"
#include "deps/platform.h"
#include "util/util.h"
#include "packet/head/head.h"


/**
  멀티세션 복제전송시에 trx_id를 공유한다.
*/
static unsigned long long g_trx_id_4_share = 0;

int im_head_set_trx_id_4_share()
{
  g_trx_id_4_share = (unsigned long long)im_util_get_unique_number();
  return 0;
}

int im_head_reset_trx_id_4_share()
{
  g_trx_id_4_share = (unsigned long long)0;
  return 0;
}

static unsigned long long get_new_tranx_id() 
{
	if ( g_trx_id_4_share != (unsigned long long)0 )  {
#if 0
  printf("!!!!!!!!!!!!!!!!![%ld]!!!!!!!!!!!!!!!\n", g_trx_id_4_share);
#endif    
		return g_trx_id_4_share;
	}

	return (unsigned long long)im_util_get_unique_number();
}



void im_head_init(IMPacketHeadPtr head)
{
	IMHeadRawPtr hraw = &(head->headRaw);

	memset(head, 0x00, sizeof(IMPacketHead));

	hraw->messageVersionMajor		= IOTMAKERS_MESSAGE_VER_MAJOR;	// 프로토콜 메인버전
	hraw->messageVersionMinor		= IOTMAKERS_MESSAGE_VER_MINOR;	// 프로토콜 하위버전

	hraw->messageType		= MessageType_NONE;
	hraw->exchangeType		= ExchangeType_NONE;

    memset(hraw->channelAccessToken, 0x00, 16);

	hraw->headerContentType		= HeaderContentType_JSON;

	hraw->extHeaderLen		= 0;

	IMHeadExtPtr ext = &(head->ext);
	im_headExt_init(ext);

	head->ext_cnt			= 2;		// default: MessageVersion, MessageID

	return;
}


void im_head_release(IMPacketHeadPtr head)
{
	IMHeadExtPtr ext = &(head->ext);
	im_headExt_release(ext);
}


void im_head_init_response(IMPacketHeadPtr head)
{
	im_head_init(head);
	im_head_set_ExchangeType(head, ExchangeType_Response);
}

void im_head_init_report(IMPacketHeadPtr head)
{
	im_head_init(head);
}



void im_head_set_MessageType(IMPacketHeadPtr head, MessageType messageType)
{
	head->headRaw.messageType = messageType;
}

MessageType im_head_get_MessageType(IMPacketHeadPtr head)
{
	return head->headRaw.messageType;
}


void im_head_set_ExchangeType(IMPacketHeadPtr head, ExchangeType exchangeType)
{
	head->headRaw.exchangeType = exchangeType;
}

void im_head_set_channelAccessToken(IMPacketHeadPtr head, char *authCode)
{
	memcpy(head->headRaw.channelAccessToken, authCode, 16);
}

char* im_head_get_channelAccessToken(IMPacketHeadPtr head)
{
	return head->headRaw.channelAccessToken;
}


void im_head_set_extHeaderLen(IMPacketHeadPtr head, unsigned short extHeaderLen)
{
	head->headRaw.extHeaderLen = htons(extHeaderLen);
}

unsigned short im_head_get_extHeaderLen(IMPacketHeadPtr head)
{
	unsigned short extHeaderLen = (unsigned short)ntohs(head->headRaw.extHeaderLen);

	return extHeaderLen;
}


int im_head_put_ext_str_val(IMPacketHeadPtr head, char *key, char *val)
{
	IMHeadExtPtr ext = &(head->ext);

	if ( im_headExt_put_kvp_str(ext, key, val) < 0 )	{
		if ( head->ext_cnt == 0 ){
			im_headExt_release(ext);
		}
		return -1;
	}
	head->ext_cnt++;
	return 0;
}


char* im_head_get_ext_messageId(IMPacketHeadPtr head)
{
	IMHeadExtPtr ext = &(head->ext);
	return im_headExt_get_kvp_str(ext, "messageId");
}

void im_head_set_ext_messageId(IMPacketHeadPtr head, char *messageIdStr)
{
	IMHeadExtPtr ext = &(head->ext);
	im_headExt_put_kvp_str(ext, "messageId", messageIdStr);
}


char* im_head_get_ext_responseCode(IMPacketHeadPtr head)
{
	IMHeadExtPtr ext = &(head->ext);
	return im_headExt_get_kvp_str(ext, "responseCode");
}
void im_head_set_ext_responseCode(IMPacketHeadPtr head, char *responseCode)
{
	IMHeadExtPtr ext = &(head->ext);
	im_headExt_put_kvp_str(ext, "responseCode", responseCode);
}




/* =====================================
head -> extension
====================================== */
int im_pktHeadExt_get_count(IMPacketHeadPtr head)
{
	return head->ext_cnt;
}

int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val)
{
	IMHeadExtPtr ext = &(head->ext);

	if ( im_headExt_put_kvp_str(ext, key, val) < 0 )	{
		return -1;
	}

	head->ext_cnt++;
	return 0;
}

char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key)
{
	IMHeadExtPtr ext = &(head->ext);
	if ( head->ext_cnt == 0 ){
		return NULL;
	}
	return (char*)im_headExt_get_kvp_str(ext, key);
}



/* =====================================
head -> extension -> (de)serialize
====================================== */
int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) 
{
	IMHeadExtPtr ext = &(head->ext);

	if ( head->ext_cnt <= 0 ) {
		return 0;
	}

	int extHeadlen = im_packet_head_ext_get_serialized_strlen(ext);
	if ( extHeadlen <= 0 )	{
		ERR("extHeadlen = %d", extHeadlen);
		return 0;
	}

	if ( extHeadlen > o_buff_len-1 )	{
		ERR("o_buff_len is less than extHeadlen %d", extHeadlen);
		return 0;
	}
	
	strcpy(o_buff, im_packet_head_ext_get_serialized_str(ext));

	return extHeadlen;
}

int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) 
{
	IMHeadRawPtr hraw = &(head->headRaw);

	IMHeadExtPtr ext = &(head->ext);

	head->ext_cnt = 1;

	int rc = im_headExt_init_with_string(ext, buf);

	//DBG("deserialized extHeader = %s", im_packet_head_ext_get_serialized_str(ext));

	return rc;
}


/* =====================================
 head (de)serialize
====================================== */
int im_head_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len)
{
	if ( o_buff_len < IOTMAKERS_DEF_HEAD_LEN )	{
		ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
		return -1;
	}

	head->len = IOTMAKERS_DEF_HEAD_LEN;

	if ( head->ext_cnt > 0 )	
	{
		int hdextlen = im_pktHeadExt_get_serialized_to_buff(head, 
			&o_buff[IOTMAKERS_DEF_HEAD_LEN], o_buff_len-IOTMAKERS_DEF_HEAD_LEN);
		if ( hdextlen <= 0 ){
			ERR("fail im_pktHeadExt_get_serialized_to_buff()");
			return -1;
		}

		im_head_set_extHeaderLen(head, hdextlen);
		head->len += hdextlen;
	}

	memcpy(&o_buff[0], (char*)&(head->headRaw), IOTMAKERS_DEF_HEAD_LEN);

	return head->len;
}


int im_head_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff)
{
	int hdlen = 0;
	int ext_len = 0;

#if 0
	// FIXME !!!
	// 좀 이상하지만 우선 이렇게 초기화하자 2021-03-09
	im_head_init(head);
	im_head_release(head);
#endif

	hdlen = IOTMAKERS_DEF_HEAD_LEN;
	memcpy((char*)&(head->headRaw), &i_buff[0], IOTMAKERS_DEF_HEAD_LEN);

	ext_len = im_head_get_extHeaderLen(head);
	if ( ext_len > 0 ) {
		int rc = im_pktHeadExt_get_deserialize_from_buff(head, &i_buff[IOTMAKERS_DEF_HEAD_LEN]);
		if ( rc < 0 ){
			ERR("fail im_pktHeadExt_get_deserialize_from_buff()");
			return -1;
		}
	}

	head->len = IOTMAKERS_DEF_HEAD_LEN + ext_len;

	return head->len;
}

//
/////////////////////////////////////////////////////////////////

