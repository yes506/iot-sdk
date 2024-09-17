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


static int get_host_ipv4_by_name(char* hostname, char* ip_str)
{
  struct hostent *host_entry;
  char  *host_ip;

  /**
       The gethostbyname*(), gethostbyaddr*(), herror(), and hstrerror() functions  are  obsolete.   Applications
       should use getaddrinfo(3), getnameinfo(3), and gai_strerror(3) instead.
  */
    host_entry = gethostbyname( hostname);
    if ( !host_entry)    {
        ERR("fail gethostbyname()");
        return -1;
    }

    host_ip = inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[0]);
    snprintf(ip_str, 32, "%s", host_ip);
    return 0;
}

static int sock_set_read_timeout(int sock, int sec)
{
	int ret = -1;

#if defined(FREERTOS)
	int  opt = sec;
	ret = lwip_setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(int));			
#else
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	ret = setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
#endif

if ( ret < 0)
		WRN("fail setsockopt SO_RCVTIMEO, sock=[%d]", sock);

	return 0;
}

static int sock_set_write_timeout(int sock, int sec)
{
	int ret = -1;

#if defined(FREERTOS)
	int  opt = sec;
	ret = lwip_setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, &opt, sizeof(int));
#else
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	ret = setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
#endif

	if ( ret < 0)
		WRN("fail setsockopt SO_SNDTIMEO, g_sockfd=[%d]", sock);
	
	return 0;
}


//////////////////////////

int net_tcp_open_socket(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);

  tcp_ctx->sock = socket(PF_INET, SOCK_STREAM, 0);
    if ( tcp_ctx->sock < 0 ) {
		ERR("fail socket()");
		return -1;
	}

  ctx->netChannelType = TCP_IPv4;

  return 0;
}

/**
TCP/IP connection on a specific interface

https://stackoverflow.com/questions/14697963/tcp-ip-connection-on-a-specific-interface
*/
int net_tcp_open_socket_n_bind_with_ip(connection_tPtr ctx, char *ipaddr)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);

  if ( net_tcp_open_socket(ctx) < 0 )	{
		ERR("fail net_tcp_open_socket()");
		return -1;
	}

  struct sockaddr_in localaddr;
  memset(&localaddr, 0x00, sizeof(struct sockaddr_in));
  localaddr.sin_family = AF_INET;
  localaddr.sin_addr.s_addr = inet_addr(ipaddr);

  int rc = bind(tcp_ctx->sock, (struct sockaddr*)&localaddr, sizeof(localaddr));

  DBG("socket[%d] binded with ip_addr [%s], rc = [%d]", tcp_ctx->sock, ipaddr, rc);

  if ( rc < 0 )  {
    return -1;
  }

	return 0;
}


/*
	Get ip from domain name
 */

static int net_tcp_hostname_to_ipv4(char* host_addr_str , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( host_addr_str ) ) == NULL) 	{
		ERR("fail gethostbyname(), errno=[%d], %s\n", errno, strerror(errno));
		return -1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	
	return -1;
}


#if defined(FREERTOS)
unsigned long addr_make_ip_ss(unsigned long ia, unsigned long ib, unsigned long ic, unsigned long id)
{
	unsigned long retulIP;
		
	retulIP = (ia << 24)+(ib << 16)+(ic << 8)+(id);
			
	return retulIP;
}

int parserip_ss(unsigned char *iData, unsigned char *oData, unsigned int pcnt)
{
	unsigned char tmp_buf[3];
	char cvtbuff[3];
	int ap_s = 0;
	int ucdata_f = 0;
	int b_cnt = 0;
	int i;
	for (i = 0; i< 30; i++)
	{
		if (iData[i] == '.')
		{
			if (i - ap_s == 1)
			{
				tmp_buf[0] = iData[ap_s];
			}
			else if (i - ap_s == 2)
			{
				tmp_buf[0] = iData[ap_s];
				tmp_buf[1] = iData[ap_s + 1];
			}
			else if (i - ap_s == 3)
			{
				tmp_buf[0] = iData[ap_s];
				tmp_buf[1] = iData[ap_s + 1];
				tmp_buf[2] = iData[ap_s + 2];
			}
			memset(cvtbuff, 0, sizeof(cvtbuff));
			memcpy(cvtbuff, tmp_buf, i - ap_s);
			oData[ucdata_f++] = atoi(cvtbuff);
			ap_s = i + 1;
			memset(tmp_buf, 0, sizeof(tmp_buf));
			b_cnt++;
		}   // if(iData[i] == '.')

		if (b_cnt == pcnt - 1)
		{
			memset(cvtbuff, 0, sizeof(cvtbuff));
			memcpy(cvtbuff, &iData[i+1], 3);
			oData[ucdata_f] = atoi(cvtbuff);
			return i;
		}
		

	} // for(int i = 0; i< 71;i++)
	return i;
}
#endif

int net_tcp_connect_ipv4(connection_tPtr ctx, char* host_addr_str, uint16_t port)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  ip_context_tPtr ip_ctx = (ip_context_tPtr)&(tcp_ctx->ipv4);
  struct sockaddr_in *addr = (struct sockaddr_in *)&(ip_ctx->addr) ;

	int ret = -1;

  if ( tcp_ctx->sock <= 0 ) {
		ERR("no socket available");
		return -1;
	}

  sock_set_read_timeout(tcp_ctx->sock, 3);
  sock_set_write_timeout(tcp_ctx->sock, 3);

  memset((char *)addr, 0x00, sizeof(struct sockaddr_in));
 
  tcp_ctx->port = port;

  INF("sock=[%d] is connecting to [%s:%d]", tcp_ctx->sock, host_addr_str, port);    

  char ipv4_str[32];
  if ( net_tcp_hostname_to_ipv4(host_addr_str, ipv4_str) < 0 )  
	  {
      return -1;
  }

  if ( strcmp(host_addr_str, ipv4_str) != 0 )  {
      INF("connect to ip [%s:%d]", ipv4_str, port);    
  }


#if defined(FREERTOS)
	unsigned long ulIPAddresskt;
	unsigned char oData[5];
	memset(oData,0,sizeof(oData));  
	parserip_ss(host_addr_str, oData, 4);
	ulIPAddresskt = addr_make_ip_ss(oData[0],oData[1],oData[2],oData[3]);
	printf("ulIPAddresskt : %d.%d.%d.%d  \r\n", (ulIPAddresskt >> 24)&0xFF,(ulIPAddresskt >> 16)&0xFF,(ulIPAddresskt >> 8)&0xFF,(ulIPAddresskt >> 0)&0xFF );

  addr->sin_family = AF_INET;
  addr->sin_len = sizeof(addr);
  addr->sin_port = lwip_htons(tcp_ctx->port);
  addr->sin_addr.s_addr = htonl(ulIPAddresskt);

#else
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(ipv4_str);
  addr->sin_port = htons(tcp_ctx->port);
#endif


  ret = connect(tcp_ctx->sock, (const struct sockaddr *)addr, sizeof(struct sockaddr_in)); 
  if( ret < 0) {
    ERR("fail connect(), errno=[%d], %s\n", errno, strerror(errno));
    return -1;
  }

  return 0;
}

void net_tcp_disconnect(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  
  if ( tcp_ctx->sock > 0 )  {
	close (tcp_ctx->sock);
    INF("close sock=[%d]", tcp_ctx->sock);    
    tcp_ctx->sock = 0;
  }
}

void net_tcp_close_socket(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  
  if ( tcp_ctx->sock > 0 )  {
	close (tcp_ctx->sock);
    INF("close sock=[%d]", tcp_ctx->sock);    
    tcp_ctx->sock = 0;
  }
}


int net_tcp_send_buffer(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

	if ( tcp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", tcp_ctx->sock);
    return -1;
  }

  size_t bytes_sent = 0;
  do {
    size_t send_len = send(tcp_ctx->sock, buff->data + bytes_sent,
                            buff->length - bytes_sent, 0);

	if (send_len < 0) {
      WRN("send() returned errno %d", errno);
      return -1;
    }
    bytes_sent += send_len;
  } while (bytes_sent < buff->length);

  DBG("Sent %ld bytes", bytes_sent);

#if 0  
  LOGHex("send_buff", buff->data, bytes_sent);
#endif

  return bytes_sent;
}






int net_tcp_recv_buffer_one(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

  // receive message.
  size_t want_read = IOBUFF_MAX_SIZE;
  buff->length = 0;

	int count = recv(tcp_ctx->sock, buff->data + buff->length, want_read, 0);
  
  if (count < 0) {
    ERR("recv error! %d", errno);
    return -1;
  } else if (count == IOBUFF_MAX_SIZE) {
    ERR("recv buff overflow; max=%d", IOBUFF_MAX_SIZE);
    return -1;
  } else if (count == 0) {
    WRN("peer may closed TCP session");

    close(tcp_ctx->sock);
    tcp_ctx->sock = -1;

    return -1;
  }

  DBG("recv(): %d bytes.", count);
  buff->length += (size_t)count;
  want_read -= (size_t)count;

  DBG("Recv %ld bytes", buff->length);

#if 0
  LOGHex("recv_buff", buff->data, buff->length);
#endif

  return buff->length;
}






int net_tcp_recv_with_length(connection_tPtr ctx, char *recv_buff, int recv_len )
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);

  // receive message.
  size_t want_read_now = recv_len;
  size_t total_length = 0;



  do {
    
    int count = recv(tcp_ctx->sock, recv_buff + total_length, want_read_now, 0);
    if (count < 0) {

#ifdef WIN32
        if(WSAGetLastError() == WSAEWOULDBLOCK) {
#else
        if(errno == EINTR) { 
#endif         
            continue; 
        } else {
            WRN("Recv Error: %s\n", strerror(errno)); 
            return -1; 
        }

    } else if (count == 0) {
      WRN("peer may closed TCP session");

      close(tcp_ctx->sock);
      tcp_ctx->sock = -1;
      return -1;
    }

    total_length += (size_t)count;
    want_read_now -= (size_t)count;

    DBG("recv(): %d bytes.", count);

  } while (total_length < recv_len);

  return total_length;
}






int net_tcp_recv_packet_with_header(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

  int len = net_tcp_recv_with_length(ctx, &buff->data[0], ctx->head_len_bytes);
  if ( len !=  ctx->head_len_bytes )  {
    ERR ("FAIL read head bytes");
  }
  buff->length = len;

  unsigned int packetLen = 0;

  if ( len == 4 )  {
    unsigned int len_tmp;
    memcpy((char*)&len_tmp, &buff->data[0], 4);
    packetLen = ntohl(len_tmp);
  }
  else if ( len == 2 )  {
    unsigned short len_tmp;
    memcpy((char*)&len_tmp, &buff->data[0], 2);
    packetLen = ntohs(len_tmp);
  }
  else if ( len == 1 )  {
    packetLen = (unsigned int)buff->data[0];
  }
  else {
    ERR ("FAIL pars head length");
    return -1;
  }

  DBG("packet length = [%d]", packetLen);

  if (packetLen >= IOBUFF_MAX_SIZE) {
    ERR("recv buff overflow; max=%d", IOBUFF_MAX_SIZE);
    return -1;
  }

  len = net_tcp_recv_with_length(ctx, &buff->data[len], packetLen);
  if ( len != packetLen )  {
    ERR ("FAIL read packet");
  }

  buff->length += len;

  DBG("Recv %ld bytes", buff->length);

  return buff->length;
}





int net_tcp_recv_buffer(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

  if (   ctx->head_len_bytes > 0  ) {
    return net_tcp_recv_packet_with_header(ctx);
  }
  else if ( ctx->enable_SOH == 1 )  {
    ERR("Not implemented");
    return -1;
  } 

	return net_tcp_recv_buffer_one(ctx);
}


/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error
 * =============================== */
int net_tcp_available_timeout(connection_tPtr ctx, int seconds)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);

	if ( tcp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", tcp_ctx->sock);
    return -1;
  }

  return net_fd_available_for_read_timeout_msec(tcp_ctx->sock, seconds*1000);
}

int net_tcp_available(connection_tPtr ctx)
{
  tcp_channel_tPtr tcp_ctx = (tcp_channel_tPtr)&(ctx->tcp_channel);

  INF("================= project/src/deps/netio/tcp_io.c net_tcp_available() start !!! =================");

  INF("tcp_ctx->sock value : [%d]", tcp_ctx->sock);

	if ( tcp_ctx->sock < 0 )  {
		ERR("invalid socket [%d] ", tcp_ctx->sock);
    return -1;
  }

  INF("================= project/src/deps/netio/tcp_io.c net_tcp_available() end !!! =================");

  return net_fd_available_for_read_timeout_msec(tcp_ctx->sock, 1);
}

