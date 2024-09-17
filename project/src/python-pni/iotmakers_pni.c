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

#include <Python.h>


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



static PyObject* ImInit(PyObject* self, PyObject* args) 
{ 
	int debug_level;
	PyArg_ParseTuple(args, "i", &debug_level);

    int rc = -1;

  /********************************************************
  * session pool stuff
  ********************************************************/
  int sess_hndl = sesspool_alloc();
  if ( sess_hndl < 0 )  {
    return Py_BuildValue("i", rc);
  }

  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return Py_BuildValue("i", rc);
  }

  /********************************************************
  * 로그레벨 설정
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(debug_level);

  rc = im_client_init(pclient);
  if ( rc < 0 )  {
    return Py_BuildValue("i", rc);
  }

	return Py_BuildValue("i", sess_hndl);

}




static PyObject* ImRelease(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);


	// ver 1.1.8 부터 적용됨 -_-
	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
		return Py_BuildValue("i", -1);
	}
	im_client_release(pclient);
	// ver 1.1.8 부터 적용됨 -_-

	
	sesspool_free(hndl);
 
	return Py_BuildValue("i", 0);
}


static PyObject* ImConnectTo(PyObject* self, PyObject* args) 
{
	int rc = -1;
	int hndl = -1;
	char *ec_ip;
	int ec_port;
	PyArg_ParseTuple(args, "isi", &hndl, &ec_ip, &ec_port);

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }



  rc = im_connect_to(pclient, ec_ip, ec_port);


	return Py_BuildValue("i", rc);
}




static PyObject* ImDisconnect(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);


  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", 0);
  }

	im_disconnect(pclient);

	return Py_BuildValue("i", 0);
}


static PyObject* ImTurnCircuitBreakerOff(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

	int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_turn_circuit_breaker_off(pclient);
	return Py_BuildValue("i", rc);
}


static PyObject* ImTurnResponseWaitOff(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

	int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_turn_response_wait_off(pclient);
	return Py_BuildValue("i", rc);
}



static PyObject* ImAuthDevice(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *im_dev_id;
	char *im_dev_pw;
	char *im_dev_gw;
	PyArg_ParseTuple(args, "isss", &hndl, &im_dev_id, &im_dev_pw, &im_dev_gw);

	int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_auth_device(pclient, im_dev_id, im_dev_pw, im_dev_gw);

	return Py_BuildValue("i", rc);
}


static PyObject* ImAuthSystem(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *im_chn_id;
	char *im_chn_pw;
	char *im_sys_id;
	PyArg_ParseTuple(args, "isss", &hndl, &im_chn_id, &im_chn_pw, &im_sys_id);

	int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_auth_system(pclient, im_chn_id, im_chn_pw, im_sys_id);

	return Py_BuildValue("i", rc);
}



static PyObject* ImPoll(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

	int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_poll(pclient);
	return Py_BuildValue("i", rc);
}


static PyObject* ImMSleep(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	int msec;
	PyArg_ParseTuple(args, "ii", &hndl, &msec);


  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", -1);
  }

  im_msleep(msec);
	return Py_BuildValue("i", 0);
}


static PyObject* ImSetTlsCaFile(PyObject* self, PyObject* args) 
{
	int rc = -1;
	int hndl = -1;
	char *ca_file;
	PyArg_ParseTuple(args, "is", &hndl, &ca_file);

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_client_set_tls_cafile(pclient, ca_file);
  return Py_BuildValue("i", rc);
}


static PyObject* ImSetTlsCaPath(PyObject* self, PyObject* args) 
{
	int rc = -1;
	int hndl = -1;
	char *ca_path;
	PyArg_ParseTuple(args, "is", &hndl, &ca_path);

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

  rc = im_client_set_tls_capath(pclient, ca_path);
  return Py_BuildValue("i", rc);
}


/////////////////////////////////////////
// Device API
static PyObject* ImResourceSendNotification(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *resource_id;
	char *properties_in_json;
	PyArg_ParseTuple(args, "iss", &hndl, &resource_id, &properties_in_json);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_resource_send_notification(pclient, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_resource_send_notification()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}


static PyObject* ImResourceNotificationInit(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_resource_notification_init(pclient);

	if ( rc < 0  )	{
		ERR("fail im_resource_notification_init()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}

static PyObject* ImResourceNotificationAppendResource(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *resource_id;
	char *properties_in_json;
	PyArg_ParseTuple(args, "iss", &hndl, &resource_id, &properties_in_json);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_resource_notification_append_resource(pclient, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_resource_notification_append_resource()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}

static PyObject* ImResourceNotificationSend(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_resource_notification_send(pclient);

	if ( rc < 0  )	{
		ERR("fail im_resource_notification_send()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}
//
/////////////////////////////////////////

/////////////////////////////////////////
// System API
static PyObject* ImSysResourceSendNotification(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *resource_id;
	char *properties_in_json;
	PyArg_ParseTuple(args, "iss", &hndl, &resource_id, &properties_in_json);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_sys_resource_send_notification(pclient, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_sys_resource_send_notification()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}


static PyObject* ImSysResourceNotificationInit(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_sys_resource_notification_init(pclient);

	if ( rc < 0  )	{
		ERR("fail im_sys_resource_notification_init()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}

static PyObject* ImSysResourceNotificationAppendResource(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	char *resource_id;
	char *properties_in_json;
	PyArg_ParseTuple(args, "iss", &hndl, &resource_id, &properties_in_json);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_sys_resource_notification_append_resource(pclient, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_sys_resource_notification_append_resource()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}

static PyObject* ImSysResourceNotificationSend(PyObject* self, PyObject* args) 
{
	int hndl = -1;
	PyArg_ParseTuple(args, "i", &hndl);

  int rc = -1;

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
	return Py_BuildValue("i", rc);
  }

	rc = im_sys_resource_notification_send(pclient);

	if ( rc < 0  )	{
		ERR("fail im_sys_resource_notification_send()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}
//
/////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
//

static PyObject *pyOnResourceSetRequestHandler = NULL;
static PyObject *pyOnResourceRetrieveOneRequestHandler = NULL;
static PyObject *pyOnResourceRetrieveAllRequestHandler = NULL;

static int OnResourceSetRequestHandler(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	int retvalue = 2000;

	if (pyOnResourceSetRequestHandler == NULL)     {
        ERR("no pyOnResourceSetRequestHandler defined");
		return 4005;
	}

    PyObject *arglist = Py_BuildValue("(Ksss)", pktBody, dev_id, resource_id, properties_in_jstr);
    PyObject *result = PyEval_CallObject(pyOnResourceSetRequestHandler, arglist);


#ifdef  __PYTHON_3__
    if (result && PyLong_Check(result)) {
        retvalue = (int)PyLong_AsLong(result);
    }
#else
    if (result && PyInt_Check(result)) {
        retvalue = PyInt_AsLong(result);
    }
#endif

    Py_XDECREF(result);
    Py_DECREF(arglist);

	return retvalue;
}

static int OnResourceRetrieveOneRequestHandler(void *pktBody, char *dev_id, char *resource_id)
{
	int retvalue = 2000;

	if (pyOnResourceRetrieveOneRequestHandler == NULL)     {
        ERR("no pyOnResourceRetrieveOneRequestHandler defined");
		return 4005;
    }

    PyObject *arglist = Py_BuildValue("(Kss)", pktBody, dev_id, resource_id);
    PyObject *result = PyEval_CallObject(pyOnResourceRetrieveOneRequestHandler, arglist);

#ifdef  __PYTHON_3__
    if (result && PyLong_Check(result)) {
        retvalue = (int)PyLong_AsLong(result);
    }
#else
    if (result && PyInt_Check(result)) {
        retvalue = PyInt_AsLong(result);
    }
#endif

    Py_XDECREF(result);
    Py_DECREF(arglist);

	return retvalue;
}

static int OnResourceRetrieveAllRequestHandler(void *pktBody, char *dev_id)
{
	int retvalue = 2000;

	if (pyOnResourceRetrieveAllRequestHandler==NULL)     {
        ERR("no py_strdata_handler_func defined");
		return 4005;
    }

    PyObject *arglist = Py_BuildValue("(Ks)", pktBody, dev_id);
    PyObject *result = PyEval_CallObject(pyOnResourceRetrieveAllRequestHandler, arglist);

#ifdef  __PYTHON_3__
    if (result && PyLong_Check(result)) {
        retvalue = (int)PyLong_AsLong(result);
    }
#else
    if (result && PyInt_Check(result)) {
        retvalue = PyInt_AsLong(result);
    }
#endif

    Py_XDECREF(result);
    Py_DECREF(arglist);

	return retvalue;
}
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Device API
static PyObject* ImSetOnResourceSetRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceSetRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceSetRequestHandler = pyFunctionObj;
		pclient->OnResourceSetRequestHandler = OnResourceSetRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceSetRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}

static PyObject* ImSetOnResourceRetrieveOneRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceRetrieveOneRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceRetrieveOneRequestHandler = pyFunctionObj;
		pclient->OnResourceRetrieveOneRequestHandler = OnResourceRetrieveOneRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceRetrieveOneRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}
static PyObject* ImSetOnResourceRetrieveAllRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceRetrieveAllRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceRetrieveAllRequestHandler = pyFunctionObj;
		pclient->OnResourceRetrieveAllRequestHandler = OnResourceRetrieveAllRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnResourceRetrieveAllRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}
//
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// System API
static PyObject* ImSetOnSysResourceSetRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceSetRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceSetRequestHandler = pyFunctionObj;
		pclient->OnSysResourceSetRequestHandler = OnResourceSetRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceSetRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}

static PyObject* ImSetOnSysResourceRetrieveOneRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceRetrieveOneRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceRetrieveOneRequestHandler = pyFunctionObj;
		pclient->OnSysResourceRetrieveOneRequestHandler = OnResourceRetrieveOneRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceRetrieveOneRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}
static PyObject* ImSetOnSysResourceRetrieveAllRequestHandler(PyObject* self, PyObject* args)
{
	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceRetrieveAllRequestHandler start!!! ===========================")
	int hndl = -1;
    PyObject *pyFunctionObj;
	PyArg_ParseTuple(args, "iO", &hndl, &pyFunctionObj);
   
	int rc = -1;

	im_client_tPtr pclient = sesspool_sess(hndl);
	if ( pclient == NULL )  {
	    ERR("pclient == NULL");
		return Py_BuildValue("i", rc);
	}

    // make sure second argument is a function
    if (!PyCallable_Check(pyFunctionObj)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
		ERR("Need a callable object!");
		return Py_BuildValue("i", rc);
    }
    else {
        pyOnResourceRetrieveAllRequestHandler = pyFunctionObj;
		pclient->OnSysResourceRetrieveAllRequestHandler = OnResourceRetrieveAllRequestHandler;
		rc = 0;
    }

	INF("=========================== project/src/python-pni/iotmakers_pni.c ImSetOnSysResourceRetrieveAllRequestHandler end!!! ===========================")

	return Py_BuildValue("i", rc);
}
//
///////////////////////////////////////////////////////////////////






//EXPORT int __stdcall ImResourceRetrieveSetResource(void *pktBody, char *resource_id, char *properties_in_json)
static PyObject* ImResourceRetrieveSetResource(PyObject* self, PyObject* args) 
{
	void *pktBody;
	char *resource_id;
	char *properties_in_json;

	PyArg_ParseTuple(args, "Kss", &pktBody, &resource_id, &properties_in_json);

	int rc = -1;

	rc = im_resource_retrieve_set_resource(pktBody, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_set_resource()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}

static PyObject* ImResourceRetrieveAppendResource(PyObject* self, PyObject* args) 
{
	void *pktBody;
	char *resource_id;
	char *properties_in_json;

	PyArg_ParseTuple(args, "Kss", &pktBody, &resource_id, &properties_in_json);

	int rc = -1;

	rc = im_resource_retrieve_append_resource(pktBody, resource_id, properties_in_json);

	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_append_resource()\n");
		return Py_BuildValue("i", rc);
	}

	return Py_BuildValue("i", rc);
}


///////////////////////////////////////////////////
#ifdef  __PYTHON_3__

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef IoTMakersDevicePy3Methods[] = { 
    { "ImInit", ImInit, METH_VARARGS, NULL },
    { "ImRelease", ImRelease, METH_VARARGS, NULL },
    { "ImConnectTo", ImConnectTo, METH_VARARGS, NULL },
    { "ImDisconnect", ImDisconnect, METH_VARARGS, NULL },
    { "ImTurnCircuitBreakerOff", ImTurnCircuitBreakerOff, METH_VARARGS, NULL },
    { "ImTurnResponseWaitOff", ImTurnResponseWaitOff, METH_VARARGS, NULL },
    { "ImAuthDevice", ImAuthDevice, METH_VARARGS, NULL },
    { "ImAuthSystem", ImAuthSystem, METH_VARARGS, NULL },
    { "ImPoll", ImPoll, METH_VARARGS, NULL },
    { "ImMSleep", ImMSleep, METH_VARARGS, NULL },
    { "ImSetTlsCaFile", ImSetTlsCaFile, METH_VARARGS, NULL },
    { "ImSetTlsCaPath", ImSetTlsCaPath, METH_VARARGS, NULL },

	/////////////////////////////////////////
	// Device API
    { "ImResourceSendNotification", ImResourceSendNotification, METH_VARARGS, NULL },
    { "ImResourceNotificationInit", ImResourceNotificationInit, METH_VARARGS, NULL },
    { "ImResourceNotificationAppendResource", ImResourceNotificationAppendResource, METH_VARARGS, NULL },		
    { "ImResourceNotificationSend", ImResourceNotificationSend, METH_VARARGS, NULL },

	{ "ImSetOnResourceSetRequestHandler", ImSetOnResourceSetRequestHandler, METH_VARARGS, NULL },
    { "ImSetOnResourceRetrieveOneRequestHandler", ImSetOnResourceRetrieveOneRequestHandler, METH_VARARGS, NULL },
    { "ImSetOnResourceRetrieveAllRequestHandler", ImSetOnResourceRetrieveAllRequestHandler, METH_VARARGS, NULL },

	/////////////////////////////////////////
	// System API
    { "ImSysResourceSendNotification", ImSysResourceSendNotification, METH_VARARGS, NULL },
    { "ImSysResourceNotificationInit", ImSysResourceNotificationInit, METH_VARARGS, NULL },
    { "ImSysResourceNotificationAppendResource", ImSysResourceNotificationAppendResource, METH_VARARGS, NULL },		
    { "ImSysResourceNotificationSend", ImSysResourceNotificationSend, METH_VARARGS, NULL },

	{ "ImSetOnSysResourceSetRequestHandler", ImSetOnSysResourceSetRequestHandler, METH_VARARGS, NULL },
    { "ImSetOnSysResourceRetrieveOneRequestHandler", ImSetOnSysResourceRetrieveOneRequestHandler, METH_VARARGS, NULL },
    { "ImSetOnSysResourceRetrieveAllRequestHandler", ImSetOnSysResourceRetrieveAllRequestHandler, METH_VARARGS, NULL },
	//
	////////////////////////////////////////////////////////////////////////

    { "ImResourceRetrieveSetResource", ImResourceRetrieveSetResource, METH_VARARGS, NULL },
    { "ImResourceRetrieveAppendResource", ImResourceRetrieveAppendResource, METH_VARARGS, NULL },

    { NULL, NULL, 0, NULL }
};


// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef IoTMakersDevicePy3_definition = { 
    PyModuleDef_HEAD_INIT,
    "IotmakersStdDevicePy3",
    "IoTMakers Device Client for Python3",
    -1, 
    IoTMakersDevicePy3Methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_IotmakersStdDevicePy3(void) {
    Py_Initialize();
    return PyModule_Create(&IoTMakersDevicePy3_definition);
}

#else  //////// Python 2

static PyMethodDef IoTMakersPyMethods[] = {
    { "ImInit", ImInit, METH_VARARGS },
    { "ImRelease", ImRelease, METH_VARARGS },
    { "ImConnectTo", ImConnectTo, METH_VARARGS },
    { "ImDisconnect", ImDisconnect, METH_VARARGS },
    { "ImTurnCircuitBreakerOff", ImTurnCircuitBreakerOff, METH_VARARGS },
    { "ImTurnResponseWaitOff", ImTurnResponseWaitOff, METH_VARARGS },
    { "ImAuthDevice", ImAuthDevice, METH_VARARGS },
    { "ImAuthSystem", ImAuthSystem, METH_VARARGS },
    { "ImPoll", ImPoll, METH_VARARGS },
    { "ImMSleep", ImMSleep, METH_VARARGS },
    { "ImSetTlsCaFile", ImSetTlsCaFile, METH_VARARGS },
    { "ImSetTlsCaPath", ImSetTlsCaPath, METH_VARARGS },

	/////////////////////////////////////////
	// Device API
    { "ImResourceSendNotification", ImResourceSendNotification, METH_VARARGS },
    { "ImResourceNotificationInit", ImResourceNotificationInit, METH_VARARGS },
    { "ImResourceNotificationAppendResource", ImResourceNotificationAppendResource, METH_VARARGS },		
    { "ImResourceNotificationSend", ImResourceNotificationSend, METH_VARARGS },

	{ "ImSetOnResourceSetRequestHandler", ImSetOnResourceSetRequestHandler, METH_VARARGS },
    { "ImSetOnResourceRetrieveOneRequestHandler", ImSetOnResourceRetrieveOneRequestHandler, METH_VARARGS },
    { "ImSetOnResourceRetrieveAllRequestHandler", ImSetOnResourceRetrieveAllRequestHandler, METH_VARARGS },

	/////////////////////////////////////////
	// System API
    { "ImSysResourceSendNotification", ImSysResourceSendNotification, METH_VARARGS },
    { "ImSysResourceNotificationInit", ImSysResourceNotificationInit, METH_VARARGS },
    { "ImSysResourceNotificationAppendResource", ImSysResourceNotificationAppendResource, METH_VARARGS },		
    { "ImSysResourceNotificationSend", ImSysResourceNotificationSend, METH_VARARGS },

	{ "ImSetOnSysResourceSetRequestHandler", ImSetOnSysResourceSetRequestHandler, METH_VARARGS },
    { "ImSetOnSysResourceRetrieveOneRequestHandler", ImSetOnSysResourceRetrieveOneRequestHandler, METH_VARARGS },
    { "ImSetOnSysResourceRetrieveAllRequestHandler", ImSetOnSysResourceRetrieveAllRequestHandler, METH_VARARGS },

	//
	////////////////////////////////////////////////////////////////////////



    { "ImResourceRetrieveSetResource", ImResourceRetrieveSetResource, METH_VARARGS },
    { "ImResourceRetrieveAppendResource", ImResourceRetrieveAppendResource, METH_VARARGS },

    { NULL, NULL, 0 }
};

/*
gcc -shared -I../iotmakers_sdk_c_ver_3.0.12/src -I/usr/include/python2.7/ -lpython2.7 -o IoTMakersPy.so IoTMakersPy.c -fPIC

EXPORT int __stdcall ImResourceSendNotification(int hndl, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceNotificationInit(int hndl);
EXPORT int __stdcall ImResourceNotificationAppendResource(int hndl, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceNotificationSend(int hndl);

EXPORT void __stdcall ImSetOnResourceSetRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id, char *resource_id, char *properties_in_jstr));
EXPORT void __stdcall ImSetOnResourceRetrieveOneRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id, char *resource_id));
EXPORT void __stdcall ImSetOnResourceRetrieveAllRequestHandler(int hndl, char*(*func)(void *pktBody, char *dev_id));

EXPORT int __stdcall ImResourceRetrieveSetResource(void *pktBody, char *resource_id, char *properties_in_json);
EXPORT int __stdcall ImResourceRetrieveAppendResource(void *pktBody, char *resource_id, char *properties_in_json);

*/

void initIotmakersStdDevicePy2()
{
    PyObject *m;
    m = Py_InitModule("IotmakersStdDevicePy2", IoTMakersPyMethods);
}

#endif
