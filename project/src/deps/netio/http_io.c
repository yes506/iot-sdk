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

#ifdef HAVE_HTTP

#include "http-client-c/src/http-client-c.h"

#include "net_context.h"
#include "kt_iot_log.h"

static buffer_t __data_recv;

static void im_http_onMsgarrvd(char *body_part, int body_len)
{
    if ( body_len == 0 )    {
        return;
    }

    memcpy(__data_recv.data, body_part, body_len);
    __data_recv.length = body_len;

    __data_recv.data[body_len] = '\0';

	DBG("__data_recv.data=[%s]", __data_recv.data);

    return ;
}


int net_http_connect(connection_tPtr ctx, char* host_addr_str, uint16_t port)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);
    HTTPClient *pclient = &(http_ctx->client);

    ctx->netChannelType = HTTP_TCP_IPv4;
    __data_recv.length = 0;

    strcpy(pclient->remote_ip, host_addr_str);
    pclient->remote_port = port;

	DBG("pclient->remote=[%s:%d]", pclient->remote_ip, pclient->remote_port);

	return 0;
}


int net_http_disconnect(connection_tPtr ctx)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);
    HTTPClient *pclient = &(http_ctx->client);

	return 0;

}


int net_http_set_auth_tok(connection_tPtr ctx, char *auth_tok)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);
    HTTPClient *pclient = &(http_ctx->client);

    sprintf(pclient->IM_HTTP_HEAD_COLL, IM_HTTP_HEAD_COLL_FORM, auth_tok);

    return 0;
}



static int net_http_send_buffer_auth(connection_tPtr ctx)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);
    HTTPClient *pclient = &(http_ctx->client);
    buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

    char url_auth[120];

    sprintf(url_auth, "http://%s:%d/extr/athn/devCommChAthn", pclient->remote_ip, pclient->remote_port);

    struct http_response* resp = http_post_json_with_callback(url_auth, IM_HTTP_HEAD_AUTH, buff->data, im_http_onMsgarrvd);

    if ( resp == NULL )    {
        ERR("Failed http_post_json_with_callback\n");
        return -1;
    }

    DBG("status_code=[%d]", resp->status_code_int);

    http_response_free(resp);

    return buff->length;
}



static int net_http_send_buffer_collection(connection_tPtr ctx)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);
    HTTPClient *pclient = &(http_ctx->client);
    buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

    char url_auth[120];

    sprintf(url_auth, "http://%s:%d/extr/colec/itgData/recv", pclient->remote_ip, pclient->remote_port);

    struct http_response* resp = http_post_json_with_callback(url_auth, pclient->IM_HTTP_HEAD_COLL, buff->data, im_http_onMsgarrvd);

    if ( resp == NULL )    {
        ERR("Failed http_post_json_with_callback\n");
        return -1;
    }

    DBG("status_code=[%d]", resp->status_code_int);

    http_response_free(resp);

    return buff->length;
}


int net_http_send_buffer(connection_tPtr ctx)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);

    buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

	DBG("");

    int bytes_sent = -1;
    
    if ( strstr(buff->data, IM_PKT_AUTH_REQ) != NULL)    {
        bytes_sent = net_http_send_buffer_auth(ctx);
    } else {
        bytes_sent = net_http_send_buffer_collection(ctx);
    }

    return bytes_sent;
}




int net_http_recv_buffer(connection_tPtr ctx)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);

    buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

    memcpy((void*)buff, (void*)&__data_recv, sizeof(buffer_t));
    __data_recv.length = 0;

#if 0
  LOGHex("recv_buff", buff->data, buff->length);
#endif

  return buff->length;
}




/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error
 * =============================== */

int net_http_available_timeout_msec(connection_tPtr ctx, int msec)
{
    http_channel_tPtr http_ctx = (http_channel_tPtr)&(ctx->http_channel);

    int msec_unit = 1;
    int msec_cnt = 0;

    while ( __data_recv.length == 0 )
    {
        #if defined(WIN32) || defined(WIN64)
            Sleep(msec_unit);
        #else
            usleep(msec_unit*1000);
        #endif

        msec_cnt++;
        if ( msec_cnt >= msec )        {
            break;
        }
    }

    return __data_recv.length;
}

int net_http_available_timeout(connection_tPtr ctx, int seconds)
{
    return net_http_available_timeout_msec(ctx, seconds*1000);
}


int net_http_available(connection_tPtr ctx)
{
    return net_http_available_timeout_msec(ctx, 1);
}


// HAVE_MQTT
#endif

