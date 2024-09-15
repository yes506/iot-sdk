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


#include "kt_iot_log.h"
#include "net_context.h"


///////////////////////////////////////////

int net_udp_open_socket(connection_tPtr ctx)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);
  ip_context_tPtr ip_ctx = (ip_context_tPtr)&(udp_ctx->ipv4);
  struct sockaddr_in *addr = (struct sockaddr_in *)&(ip_ctx->addr) ;

	udp_ctx->sock = socket(PF_INET, SOCK_DGRAM, 0);
  if ( udp_ctx->sock < 0 ) {
		ERR("fail socket()");
		return -1;
	}

  DBG("udp_ctx->sock=[%d]", udp_ctx->sock);

  ctx->netChannelType = UDP_IPv4;

  return 0;
}

void net_udp_close_socket(connection_tPtr ctx)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);
  
  DBG("udp_ctx->sock=[%d]", udp_ctx->sock);
  if ( udp_ctx->sock > 0 )  {
    close (udp_ctx->sock);
    udp_ctx->sock = 0;
  }
}


int net_udp_set_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);

  strcpy((char*)udp_ctx->remote_ipv4_str, ipv4_str);
  udp_ctx->port = port;

  return 0;
}



int net_udp_send_buffer_to_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

	if ( udp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", udp_ctx->sock);
    return -1;
  }

  DBG("ipv4_str=[%s], port=[%d]", ipv4_str, port);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipv4_str);
	addr.sin_port   = htons(port);
	memset((void*)&(addr.sin_zero), 0x00, 8);     /* zero the rest of the struct */

	size_t bytes_sent = sendto(udp_ctx->sock, buff->data, buff->length, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

  return bytes_sent;
}

int net_udp_send_buffer(connection_tPtr ctx)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);

  return net_udp_send_buffer_to_ipv4(ctx, (char*)udp_ctx->remote_ipv4_str, udp_ctx->port);
}




int net_udp_recv_buffer_from_ipv4(connection_tPtr ctx, char *o_ipv4_str, int *o_port)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

  // receive message.
  size_t want_read = IOBUFF_MAX_SIZE;
  size_t total_length = 0;
  buff->length = 0;

  struct sockaddr_in addr;
  socklen_t clilen;

  clilen = sizeof(struct sockaddr_in);

  total_length = recvfrom(udp_ctx->sock, buff->data, want_read, 0, (struct sockaddr *)&addr, &clilen);
  if ( total_length <= 0)	{
    ERR("fail recvfrom() = [%ld]", total_length);
    return -1;
  }
  
  if ( o_ipv4_str != NULL )    {
    strcpy(o_ipv4_str, (char*)inet_ntoa(addr.sin_addr));
  }

  if ( o_port != NULL )    {
    *o_port = ntohs(addr.sin_port); 
  }

  buff->length = total_length;

  return buff->length;
}

int net_udp_recv_buffer(connection_tPtr ctx)
{
  return net_udp_recv_buffer_from_ipv4(ctx, NULL, NULL);
}



/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error
 * =============================== */
int net_udp_available_timeout(connection_tPtr ctx, int seconds)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);

	if ( udp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", udp_ctx->sock);
    return -1;
  }

  return net_fd_available_for_read_timeout_msec(udp_ctx->sock, seconds*1000);
}

int net_udp_available(connection_tPtr ctx)
{
  udp_channel_tPtr udp_ctx = (udp_channel_tPtr)&(ctx->udp_channel);

	if ( udp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", udp_ctx->sock);
    return -1;
  }

  return net_fd_available_for_read_timeout_msec(udp_ctx->sock, 1);
}

