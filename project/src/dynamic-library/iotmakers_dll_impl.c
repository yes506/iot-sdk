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

/* iotmakers_dll_impl.c */

#include <stdio.h>
#include <stdlib.h>
#include "client/client.h"
#include "util/util.h"
#include "kt_iot_log.h"

#include "iotmakers_dll_impl.h"

///////////////////////////////////////////////////
//  SESSION POOL STUFF

#define IM_SESS_MAX   128

static im_client_tPtr sess_pool[IM_SESS_MAX];

static int sesspool_alloc()
{
  int sess_idx;
  
  for (sess_idx = 0; sess_idx < IM_SESS_MAX; sess_idx++)  {
    if ( sess_pool[sess_idx] == NULL )    {
      sess_pool[sess_idx] = malloc(sizeof(im_client_t));
      return sess_idx;
    }
  }

  ERR ("sess_pool is full, max = %d", IM_SESS_MAX);
  return -1;
}

static void sesspool_free(int hndl)
{
  if ( hndl < 0 || hndl >= IM_SESS_MAX ) {
    ERR ("hndl is out of %d", IM_SESS_MAX);
    return;
  }

  im_client_tPtr sess = sess_pool[hndl];
  if ( sess != NULL )  {
    free(sess);
    sess_pool[hndl] = NULL;
  }
}

static im_client_tPtr sesspool_sess(int hndl)
{
  if ( hndl < 0 || hndl >= IM_SESS_MAX ) {
    ERR ("hndl is out of %d", IM_SESS_MAX);
    return NULL;
  }

  if ( sess_pool[hndl] == NULL ) {
    ERR("no sess found in the sess_pool");
    return NULL;
  }

  return  sess_pool[hndl];
}

//  SESSION POOL STUFF
///////////////////////////////////////////////////

int __stdcall ImInit(int debug_level)
{
	int rc = 0;
 
  /********************************************************
  * session pool stuff
  ********************************************************/
  int sess_hndl = sesspool_alloc();
  if ( sess_hndl < 0 )  {
    return -1;
  }

  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }


  /********************************************************
  * 로그레벨 설정
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(debug_level);

  rc = im_client_init(pclient);
  if ( rc < 0 )  {
    return -1;
  }

  return sess_hndl;
}

int __stdcall ImRelease(int sess_hndl)
{
  sesspool_free(sess_hndl);
  return 0;
}

int __stdcall ImConnectTo(int sess_hndl, char* ec_ip, int ec_port)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect_to(pclient, ec_ip, ec_port);
  return rc;
}



int __stdcall ImTurnCircuitBreakerOff(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_circuit_breaker_off(pclient);
  return rc;
}


int __stdcall ImTurnResponseWaitOff(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_response_wait_off(pclient);
  return rc;
}




int __stdcall ImConnect(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }
#if 0
  int rc = im_connect(pclient);
	return rc;
#else
	return -1;
#endif
}

int __stdcall ImDisconnect(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

	im_disconnect(pclient);
  return 0;
}

//////////////////////////////////
int __stdcall ImAuthDevice(int sess_hndl, char *im_dev_id,	char *im_dev_pw, char *im_dev_gw)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_auth_device(pclient, im_dev_id, im_dev_pw, im_dev_gw);
	return rc;
}

int __stdcall ImAuthSystem(int sess_hndl, char *im_chn_id,	char *im_chn_pw, char *im_sys_id)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_auth_system(pclient, im_chn_id, im_chn_pw, im_sys_id);
	return rc;
}





int __stdcall ImPoll(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_poll(pclient);
	return rc;
}

int __stdcall ImMSleep(int sess_hndl, int msec)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  im_msleep(msec);
  return 0;
}


int __stdcall ImSetTlsCaFile(int sess_hndl, char *ca_file)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_client_set_tls_cafile(pclient, ca_file);
  return rc;
}


int __stdcall ImSetTlsCaPath(int sess_hndl, char *ca_path)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_client_set_tls_capath(pclient, ca_path);
  return rc;
}



/////////////////////////////////////////
// Device API
int __stdcall ImResourceSendNotification(int sess_hndl, char *resource_id, char *properties_in_json)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_resource_send_notification(pclient, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_send_notification()\n");
		rc = -1;
	}
	return rc;
}

int __stdcall ImResourceNotificationInit(int sess_hndl)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_resource_notification_init(pclient);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_init()\n");
		rc = -1;
	}
	return rc;
}

int __stdcall ImResourceNotificationAppendResource(int sess_hndl, char *resource_id, char *properties_in_json)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_resource_notification_append_resource(pclient, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_append_resource()\n");
		rc = -1;
	}
	return rc;
}
int __stdcall ImResourceNotificationSend(int sess_hndl)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_resource_notification_send(pclient);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_send()\n");
		rc = -1;
	}
	return rc;
}


EXPORT void __stdcall ImSetOnResourceSetRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnResourceSetRequestHandler = func;
}

EXPORT void __stdcall ImSetOnResourceRetrieveOneRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *dev_id, char *resource_id))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnResourceRetrieveOneRequestHandler = func;
}

EXPORT void __stdcall ImSetOnResourceRetrieveAllRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *dev_id))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnResourceRetrieveAllRequestHandler = func;
}
//
////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////
// System API
int __stdcall ImSysResourceSendNotification(int sess_hndl, char *resource_id, char *properties_in_json)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_sys_resource_send_notification(pclient, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_send_notification()\n");
		rc = -1;
	}
	return rc;
}

int __stdcall ImSysResourceNotificationInit(int sess_hndl)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_sys_resource_notification_init(pclient);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_init()\n");
		rc = -1;
	}
	return rc;
}

int __stdcall ImSysResourceNotificationAppendResource(int sess_hndl, char *resource_id, char *properties_in_json)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_sys_resource_notification_append_resource(pclient, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_append_resource()\n");
		rc = -1;
	}
	return rc;
}
int __stdcall ImSysResourceNotificationSend(int sess_hndl)
{
	im_client_tPtr pclient = sesspool_sess(sess_hndl);
	if ( pclient == NULL )  {
	  return -1;
	}
	
	int rc = im_sys_resource_notification_send(pclient);
	if ( rc < 0  )	{
		ERR("fail im_resource_notification_send()\n");
		rc = -1;
	}
	return rc;
}


EXPORT void __stdcall ImSetOnSysResourceSetRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *sys_id, char *resource_id, char *properties_in_jstr))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnSysResourceSetRequestHandler = func;
}

EXPORT void __stdcall ImSetOnSysResourceRetrieveOneRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *sys_id, char *resource_id))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnSysResourceRetrieveOneRequestHandler = func;
}

EXPORT void __stdcall ImSetOnSysResourceRetrieveAllRequestHandler(int sess_hndl, int(*func)(void *pktBody, char *sys_id))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  pclient->OnSysResourceRetrieveAllRequestHandler = func;
}
//
////////////////////////////////////////////////////////////////////////



EXPORT int __stdcall ImResourceRetrieveSetResource(void *pktBody, char *resource_id, char *properties_in_json)
{
	int rc = im_resource_retrieve_set_resource(pktBody, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_set_resource()\n");
		rc = -1;
	}

	return rc;
}

EXPORT int __stdcall ImResourceRetrieveAppendResource(void *pktBody, char *resource_id, char *properties_in_json)
{
	int rc = im_resource_retrieve_append_resource(pktBody, resource_id, properties_in_json);
	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_append_resource()\n");
		rc = -1;
	}

	return rc;
}

