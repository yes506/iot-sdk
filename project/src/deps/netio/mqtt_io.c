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

#ifdef HAVE_MQTT

#include "net_context.h"
#include "kt_iot_log.h"


#define MQTT_CLIENT_ID  "IM_MQTT_CLIENT"


static buffer_t __data_recv;


typedef void (*IMCbRecvMsgHandler)(char *data, int data_len);

static IMCbRecvMsgHandler _cb_onMsgarrvd_hndl = NULL;;

void im_mqtt_set_cb_recv_handler(void* cb)
{
	_cb_onMsgarrvd_hndl = (IMCbRecvMsgHandler)cb;
}

static void im_mqtt_onConnlost(void *context, char *cause)
{
	INF("Connection lost; cause=[%s]", cause);
}
static int im_mqtt_onMsgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	DBG("Message arrived, topic=[%s]", topicName);

	if ( _cb_onMsgarrvd_hndl != NULL )	{
		_cb_onMsgarrvd_hndl(message->payload, message->payloadlen);
	}

    memcpy(__data_recv.data, message->payload, message->payloadlen);
    __data_recv.length = message->payloadlen;

	MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
static void im_mqtt_onDelivered(void *context, MQTTClient_deliveryToken dt)
{
	DBG("Message with token value [%d] delivery confirmed", dt);
}



int net_mqtt_connect(connection_tPtr ctx, char* host_addr_str, uint16_t port, int keepAliveInSec)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);

	char  mqttAddr[160];
	int rc = -1;

    int timeout_sec = 5;


    ctx->netChannelType = MQTT_TCP_IPv4;
    __data_recv.length = 0;


    mqtt_ctx->mqttTopic4Pub[0] = '\0';
    mqtt_ctx->mqttTopic4Sub[0] = '\0';



    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	if ( timeout_sec > 0 )	{
		conn_opts.connectTimeout = timeout_sec;
	}

	snprintf(mqttAddr, sizeof(mqttAddr), "tcp://%s:%d", host_addr_str, port);

    rc = MQTTClient_create(pclient, mqttAddr, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if ( rc < 0) {
		ERR("fail MQTTClient_create(), rc=[%d]", rc);
		return -1;
	}

	conn_opts.keepAliveInterval = keepAliveInSec;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(*pclient, NULL, im_mqtt_onConnlost, im_mqtt_onMsgarrvd, im_mqtt_onDelivered);

    DBG("MQTTClient_connect(); addr=[%s], cid=[%s]", mqttAddr, MQTT_CLIENT_ID);
	rc = MQTTClient_connect(*pclient, &conn_opts);
    if (rc < 0 ) {
		ERR("fail MQTTClient_connect(), rc=[%d]", rc);
		return -1;
	}

	return 0;
}


int net_mqtt_disconnect(connection_tPtr ctx)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);

	int timeout_msec = 5000;
    MQTTClient_disconnect(*pclient, timeout_msec);
    MQTTClient_destroy(pclient);

	return 0;

}


static void net_mqtt_set_mqttTopic4Pub(connection_tPtr ctx, char *topic)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
	
    sprintf(mqtt_ctx->mqttTopic4Pub, "in/%s", topic);
}

static void net_mqtt_set_mqttTopic4Sub(connection_tPtr ctx, char *topic)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
	sprintf(mqtt_ctx->mqttTopic4Sub, "out/%s", topic);
}


static void net_mqtt_unsubscribe(connection_tPtr ctx)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);

    if ( mqtt_ctx->mqttTopic4Sub[0] == '\0' )
    {
        return;
    }

    char *topic = mqtt_ctx->mqttTopic4Sub;

	INF("Unsubscribe topic=[%s]", topic);
	int rc = MQTTClient_unsubscribe(*pclient, topic);
    if (rc < 0 ) {
		ERR("fail MQTTClient_unsubscribe(), rc=[%d]", rc);
		return -1;
	}

    mqtt_ctx->mqttTopic4Sub[0] = '\0';
    
    return 0;
}


static int net_mqtt_subscribe_topic(connection_tPtr ctx, char *topic_key)
{	
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);

	int rc;
	static int QOS = 1;

    char *topic4sub = mqtt_ctx->mqttTopic4Sub;

    net_mqtt_unsubscribe(ctx);

    net_mqtt_set_mqttTopic4Sub(ctx, topic_key);

	INF("Subscribe topic=[%s]", topic4sub);

	rc = MQTTClient_subscribe(*pclient, topic4sub, QOS);
    if (rc < 0 ) {
		ERR("fail MQTTClient_subscribe(), rc=[%d]", rc);
		return -1;
	}

	return 0;
}


int net_mqtt_set_io_topic(connection_tPtr ctx, char *topic_key)
{	
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);


    net_mqtt_set_mqttTopic4Pub(ctx, topic_key);

    int rc = net_mqtt_subscribe_topic(ctx, topic_key);

    INF("mqttTopic4Pub=[%s]", mqtt_ctx->mqttTopic4Pub);
    INF("mqttTopic4Sub=[%s]", mqtt_ctx->mqttTopic4Sub);

	return rc;
}



static int net_mqtt_publish(connection_tPtr ctx, unsigned char *data, int data_len)
{	
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);
    MQTTClient *pclient = &(mqtt_ctx->client);

    int rc;
	static int timeout_msec = 2000L;
	static int QOS = 1;

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

	pubmsg.payload = data;
	pubmsg.payloadlen = data_len;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;


    char *topic4pub = mqtt_ctx->mqttTopic4Pub;

    DBG("Publish for topic [%s]", topic4pub);
	rc = MQTTClient_publishMessage(*pclient, topic4pub, &pubmsg, &token);
	if ( rc < 0 ){
		ERR("fail MQTTClient_publishMessage(), rc=[%d]", rc);
		return -1;
	}

	return data_len;
}

int net_mqtt_send_buffer(connection_tPtr ctx)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);

    buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

	if ( mqtt_ctx->mqttTopic4Pub[0] == '\0' )  {
		ERR("mqtt not ready; mqttTopic4Pub=[%s]", mqtt_ctx->mqttTopic4Pub);
        return -1;
    }

    int bytes_sent = net_mqtt_publish(ctx, buff->data, buff->length);

    return bytes_sent;
}




int net_mqtt_recv_buffer(connection_tPtr ctx)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);

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

int net_mqtt_available_timeout_msec(connection_tPtr ctx, int msec)
{
    mqtt_channel_tPtr mqtt_ctx = (mqtt_channel_tPtr)&(ctx->mqtt_channel);

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

int net_mqtt_available_timeout(connection_tPtr ctx, int seconds)
{
    return net_mqtt_available_timeout_msec(ctx, seconds*1000);
}


int net_mqtt_available(connection_tPtr ctx)
{
    return net_mqtt_available_timeout_msec(ctx, 1);
}


// HAVE_MQTT
#endif

