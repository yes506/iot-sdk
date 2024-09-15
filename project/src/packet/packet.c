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



#include <string.h>

/*
#if !defined(WIN32)
#include <arpa/inet.h>
#endif
*/
#include "packet/packet.h"

/* =====================================
packet - init
====================================== */
int im_packet_init(IMPacketPtr pkt) 
{
	memset(pkt, 0x00, sizeof(IMPacket));
	return 0;
}

int im_packet_release(IMPacketPtr pkt) 
{
	im_head_release(&pkt->head);
	im_body_release(&pkt->body);
	return 0;
}


/* =====================================
packet - (de)serialize
====================================== */
int im_packet_get_serialized_to_buff(IMPacketPtr pkt, char *o_buff, unsigned int o_buff_len)
{
	IMPacketHeadPtr head = &pkt->head;
	IMPacketBodyPtr body = &pkt->body;

	int head_len = 0;
	int body_len = 0;

	head_len = im_head_get_serialized_to_buff(head, 
		&o_buff[0], o_buff_len);
	if ( head_len < 0 )	{
		//ERROR_LOG("fail im_head_get_serialized_to_buff()");
		return -1;
	}

	if ( body->root != NULL )	{
		body_len = im_body_get_serialized_to_buff(body, 
			&o_buff[head_len], o_buff_len-head_len);
	}

	return head_len + body_len;
}

int im_packet_get_deserialized_from_buff(IMPacketPtr pkt, char *i_buff, unsigned int i_buff_len)
{
	IMPacketHeadPtr head = &pkt->head;
	IMPacketBodyPtr body = &pkt->body;

	int head_len = 0;
	int body_len = 0;

	unsigned long pktLen = 0;

	i_buff[i_buff_len] = '\0';

	head_len = im_head_get_deserialized_from_buff(head, &i_buff[0]);
	if ( head_len < 0 )	{
		return -1;
	}

    body_len = im_body_get_deserialized_from_buff_len(body, &i_buff[head_len], i_buff_len-head_len);
	if ( body_len < 0 )	{
		return -2;
	}

	return (head_len + body_len);
}

