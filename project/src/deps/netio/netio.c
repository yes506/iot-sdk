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




//////////////////////////

int netio_send_buffer(connection_tPtr ctx)
{
  size_t bytes_sent = -1;
  
  switch (ctx->netChannelType)  {
#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  case TLS:
    bytes_sent = net_tls_send_buffer(ctx);
    break;  
#else
  case TCP_IPv4:
    bytes_sent = net_tcp_send_buffer(ctx);
    break;  
  case UDP_IPv4:
    bytes_sent = net_udp_send_buffer(ctx);
    break;  
#endif

#ifdef HAVE_MQTT
  case MQTT_TCP_IPv4:
    bytes_sent = net_mqtt_send_buffer(ctx);
    break;  
#endif

#ifdef HAVE_HTTP
  case HTTP_TCP_IPv4:
    bytes_sent = net_http_send_buffer(ctx);
    break;  
#endif

  }
  return bytes_sent;
}


int netio_recv_buffer(connection_tPtr ctx)
{
  size_t bytes_read = -1;

  switch (ctx->netChannelType)  {
#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  case TLS:
    bytes_read = net_tls_recv_buffer(ctx);
    break;  
#else
  case TCP_IPv4:
    bytes_read = net_tcp_recv_buffer(ctx);
    break;  
  case UDP_IPv4:
    bytes_read = net_udp_recv_buffer(ctx);
    break;  
#endif

#ifdef HAVE_MQTT
  case MQTT_TCP_IPv4:
    bytes_read = net_mqtt_recv_buffer(ctx);
    break;  
#endif

#ifdef HAVE_HTTP
  case HTTP_TCP_IPv4:
    bytes_read = net_http_recv_buffer(ctx);
    break;  
#endif

  }

  return bytes_read;
}


/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error; peer may closed
 * =============================== */
int netio_available_timeout(connection_tPtr ctx, int seconds)
{
  int rc = -1;

  switch (ctx->netChannelType)  {
#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  case TLS:
    rc = net_tls_available_timeout(ctx, seconds);
    break;  
#else
  case TCP_IPv4:
    rc = net_tcp_available_timeout(ctx, seconds);
    break;  
  case UDP_IPv4:
    rc = net_udp_available_timeout(ctx, seconds);
    break;  
#endif

#ifdef HAVE_MQTT
  case MQTT_TCP_IPv4:
    rc = net_mqtt_available_timeout(ctx, seconds);
    break;  
#endif

#ifdef HAVE_HTTP
  case HTTP_TCP_IPv4:
    rc = net_http_available_timeout(ctx, seconds);
    break;  
#endif

  }

  return rc;
}

int netio_available(connection_tPtr ctx)
{
  INF("====================== project/src/deps/netio/netio.c netio_available start !!! ======================");

  int rc = -1;

  switch (ctx->netChannelType)  {
#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  case TLS:
    rc = net_tls_available(ctx);
    break;  
#else
  case TCP_IPv4:
  INF("====================== ctx->netChannelType == TCP_IPv$ ======================");
    rc = net_tcp_available(ctx);
    break;  
  case UDP_IPv4:
  INF("====================== ctx->netChannelType == UDP_IPv$ ======================");
    rc = net_udp_available(ctx);
    break;  
#endif

#ifdef HAVE_MQTT
  case MQTT_TCP_IPv4:
    rc = net_mqtt_available(ctx);
    break;  
#endif

#ifdef HAVE_HTTP
  case HTTP_TCP_IPv4:
    rc = net_http_available(ctx);
    break;  
#endif

  }

  INF("====================== project/src/deps/netio/netio.c netio_available end !!! ======================");

  return rc;
}


/*
  set to read first 4 bytes for every packet 
*/
void netio_set_read_head_length_4(connection_tPtr ctx)
{
  ctx->head_len_bytes = 4 ;
}

/*
  set to read and veriry SOH(Start Of Head) charactor
*/
void netio_set_start_head_char(connection_tPtr ctx, char SOH)
{
  /* 2019-04-01 */
  ctx->enable_SOH = 1 ;
  ctx->SOH_CHAR = SOH ;
}
void netio_reset_start_head_char(connection_tPtr ctx)
{
  /* 2019-04-01 */
  ctx->enable_SOH = 0 ;
  ctx->SOH_CHAR = 0 ;
}
