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


#ifndef _NET_CONTEXT_H
#define _NET_CONTEXT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif FREERTOS
//#include <sys/socket.h>			// Needed for socket() etc.
#include <lwip/sockets.h>
//#include <netinet/in.h>			// Needed for sockaddr_in

#include <netdb.h>
// For hostent

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <netdb.h>
#include <errno.h>
#endif


#ifdef HAVE_OPENSSL
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#endif


#ifdef HAVE_MBEDTLS

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_time            time
#define mbedtls_time_t          time_t
#define mbedtls_fprintf         fprintf
#define mbedtls_printf          printf
#define mbedtls_exit            exit
#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include <string.h>

#endif /* HAVE_MBEDTLS */



#ifdef HAVE_MQTT
#include "org.eclipse.paho.mqtt.c/src/MQTTClient.h"
#include "org.eclipse.paho.mqtt.c/src/Log.h"
#endif


#include "deps/platform.h"


#define IOBUFF_MAX_SIZE   1024*1000*1 // 1MB,   1024*10; 2022-02-10

typedef struct buffer_t {
  uint8_t data[IOBUFF_MAX_SIZE];
  size_t length;
} buffer_t, *buffer_tPtr;



typedef struct ip_context_t {
  struct sockaddr_storage addr;
} ip_context_t, *ip_context_tPtr;



typedef struct udp_channel_t {
  ip_context_t ipv4;
  uint16_t port;
  int sock;
  char *remote_ipv4_str[32];
} udp_channel_t, *udp_channel_tPtr;


typedef struct tcp_channel_t {
  ip_context_t ipv4;
  uint16_t port;
  int sock;
} tcp_channel_t, *tcp_channel_tPtr;



typedef struct termio_channel_t {
  int fd;
} termio_channel_t, *termio_channel_tPtr;



#ifdef HAVE_OPENSSL
typedef struct tls_channel_t {
  SSL_CTX   *g_ctx;
  SSL       *g_ssl;
  const char  *g_ca_file;
  const char  *g_ca_path;
  int g_connected;
} tls_channel_t, *tls_channel_tPtr;
#endif




#ifdef HAVE_MBEDTLS

#define __MBEDTLS_DEBUG_LEVEL 1

typedef struct tls_channel_t {
    mbedtls_net_context		server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context		ssl;
    mbedtls_ssl_config		conf;
    mbedtls_x509_crt		cacert;
  
  const char  *g_ca_file;
  const char  *g_ca_path;
  int g_connected;
} tls_channel_t, *tls_channel_tPtr;
#endif



#ifdef HAVE_MQTT
typedef struct mqtt_channel_t {
    MQTTClient client;

	char mqttTopic4Pub[80];
	char mqttTopic4Sub[80];

} mqtt_channel_t, *mqtt_channel_tPtr;
#endif


#ifdef HAVE_HTTP

#define IM_HTTP_HEAD_AUTH \
    "Message-Exchange-Pattern: one-way-ack\r\n" \
    "Message-Type: request\r\n" \
    "Header-Type: basic\r\n" \
    "Encryption-Usage: no\r\n" \
    "Protocol-Version: 01.00\r\n" \
    "Compression-Usage: no\r\n" \
    "Encoding-Type: json\r\n" \
    "Method-Type: /extr/athn/devCommChAthn" \


#define IM_HTTP_HEAD_COLL_FORM \
    "Message-Exchange-Pattern: one-way-ack\r\n" \
    "Message-Type: request\r\n" \
    "Header-Type: basic\r\n" \
    "Encryption-Usage: no\r\n" \
    "Protocol-Version: 01.00\r\n" \
    "Compression-Usage: no\r\n" \
    "Encoding-Type: json\r\n" \
    "Method-Type: /extr/colec/itgData/recv\r\n" \
    "Channel-Auth-Token: %s" \


#define IM_PKT_AUTH_REQ "athnRqtNo"

typedef struct HTTPClient {
    char remote_ip[60];
    int  remote_port;

    char IM_HTTP_HEAD_COLL[300];

} HTTPClient, *HTTPClientPtr;

typedef struct http_channel_t {
    HTTPClient client;

} http_channel_t, *http_channel_tPtr;
#endif


typedef enum {
	UNDEF = 0,
	TCP_IPv4 = 1,
	UDP_IPv4 = 2,
	TERMIO = 3,
	TLS = 4,
	MQTT_TCP_IPv4 = 5,
  HTTP_TCP_IPv4 = 6
} NetChannelType;


typedef struct connection_t {
  udp_channel_t udp_channel;
  tcp_channel_t tcp_channel;
  termio_channel_t termio_channel;

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  tls_channel_t tls_channel;
#endif

#ifdef HAVE_MQTT
  mqtt_channel_t mqtt_channel;
#endif

#ifdef HAVE_HTTP
  http_channel_t http_channel;
#endif

  buffer_t send_buff; 
  buffer_t recv_buff; 

  /* 2019-04-01 */
  char SOH_CHAR ;
  int enable_SOH ;
  int head_len_bytes ;

  NetChannelType  netChannelType;

} connection_t, *connection_tPtr;

#endif /* _NET_CONTEXT_H */




#ifdef __cplusplus
extern "C"
{
#endif

// fd_util.c
int net_fd_available_for_read_timeout_msec(int fd, int msec);

// netio.c
int netio_send_buffer(connection_tPtr ctx);
int netio_recv_buffer(connection_tPtr ctx);
int netio_available_timeout(connection_tPtr ctx, int seconds);
int netio_available(connection_tPtr ctx);

// tcp_io.c
int net_tcp_open_socket(connection_tPtr ctx);
int net_tcp_open_socket_n_bind_with_ip(connection_tPtr ctx, char *ipaddr);

int net_tcp_connect_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port);
void net_tcp_disconnect(connection_tPtr ctx);
void net_tcp_close_socket(connection_tPtr ctx);
int net_tcp_send_buffer(connection_tPtr ctx);
int net_tcp_recv_buffer(connection_tPtr ctx);
int net_tcp_available_timeout(connection_tPtr ctx, int seconds);
int net_tcp_available(connection_tPtr ctx);


// udp_io.c
int net_udp_open_socket(connection_tPtr ctx);
void net_udp_close_socket(connection_tPtr ctx);
int net_udp_set_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port);
int net_udp_send_buffer_to_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port);
int net_udp_send_buffer(connection_tPtr ctx);
int net_udp_recv_buffer_from_ipv4(connection_tPtr ctx, char *o_ipv4_str, int *o_port);
int net_udp_recv_buffer(connection_tPtr ctx);
int net_udp_available_timeout(connection_tPtr ctx, int seconds);
int net_udp_available(connection_tPtr ctx);

// tls_io.c
int net_tls_open_socket(connection_tPtr ctx);
int net_tls_open_socket_n_bind_with_ip(connection_tPtr ctx, char *ipaddr);

int net_tls_set_trusred_ca_file(connection_tPtr ctx, char *ca_file);
int net_tls_set_trusred_ca_path(connection_tPtr ctx, char *ca_path);

int net_tls_connect_ipv4(connection_tPtr ctx, char *ipv4_str, uint16_t port) ;
int net_tls_disconnect(connection_tPtr ctx);

int net_tls_send_buffer(connection_tPtr ctx);
int net_tls_recv_buffer(connection_tPtr ctx);
int net_tls_available_timeout(connection_tPtr ctx, int seconds);
int net_tls_available(connection_tPtr ctx);

// mqtt_io.c
int net_mqtt_connect(connection_tPtr ctx, char* host_addr_str, uint16_t port, int keepAliveInSec);
int net_mqtt_disconnect(connection_tPtr ctx);
int net_mqtt_set_io_topic(connection_tPtr ctx, char *topic_key);
int net_mqtt_send_buffer(connection_tPtr ctx);
int net_mqtt_recv_buffer(connection_tPtr ctx);
int net_mqtt_available_timeout(connection_tPtr ctx, int seconds);
int net_mqtt_available(connection_tPtr ctx);


// http_io.c
int net_http_connect(connection_tPtr ctx, char* host_addr_str, uint16_t port);
int net_http_disconnect(connection_tPtr ctx);
int net_http_set_auth_tok(connection_tPtr ctx, char *auth_tok);
int net_http_send_buffer(connection_tPtr ctx);
int net_http_recv_buffer(connection_tPtr ctx);
int net_http_available_timeout(connection_tPtr ctx, int seconds);
int net_http_available(connection_tPtr ctx);



// term.c
int net_term_open(connection_tPtr ctx, char *portname);
void net_term_close(connection_tPtr ctx);
void net_term_set_speed_115200(connection_tPtr ctx);
void net_term_set_speed_9600(connection_tPtr ctx);
int net_term_send_buffer(connection_tPtr ctx);
int net_term_recv_buffer(connection_tPtr ctx);
int net_term_available_timeout(connection_tPtr ctx, int seconds);
int net_term_available(connection_tPtr ctx);


#ifdef __cplusplus
}
#endif






/**

struct sockaddr {
   unsigned short    sa_family;    // address family, AF_xxx
   char              sa_data[14];  // 14 bytes of protocol address
};

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};

*/
