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

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "client/client.h"
#include "util/util.h"
#include "kt_iot_log.h"

#include "iotmakers_jni.h"


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


static char* getStringUTFChars(JNIEnv *env, jstring jstr)
{
#ifdef __cplusplus
    char *c_str = (char*)(env)->GetStringUTFChars(jstr, NULL);
#else
    char *c_str = (char*)(*env)->GetStringUTFChars( env, jstr, NULL ) ;
#endif
    return c_str;
}


static void releaseStringUTFChars(JNIEnv *env, char* c_str, jstring jstr)
{
#ifdef __cplusplus
    (env)->ReleaseStringUTFChars(ec_ip, c_str);
#else
    (*env)->ReleaseStringUTFChars(env, jstr, c_str);
#endif
}


#if 1
JavaVM * glpVM = NULL;

jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    jint result = -1 ;
    JNIEnv* env = NULL ;
    jclass cls ;

    if ( (*vm)->GetEnv(vm,  (void**)&env, JNI_VERSION_1_6) != JNI_OK ) {
        INF("GetEnv failed.\n" ) ;
        goto error ;
    }

    result = JNI_VERSION_1_6 ;
    glpVM = vm ;

error:
    return result ;
}
#endif



//////////////////////////////////////////////////////
// run in the im_poll thread 
static JNIEnv *poll_env = NULL;
static jobject *poll_obj = NULL;

static jclass findJavaClass(JNIEnv* env, char *class_path)
{
    jclass clz = (*env)->FindClass(env, class_path);
    //jclass clz = (*poll_env)->FindClass(env, class_path);
    if ( clz == NULL )    {
        ERR("java class IoTMakersDeviceClient Not Found");
        return NULL;
    }
    return clz;
}

static jmethodID findJavaStaticMethod(JNIEnv* env, jclass clz, char* method_name, char* method_sig)
{
    // find a static method
    jmethodID method = (*env)->GetStaticMethodID(env, clz, method_name, method_sig);
    //jmethodID method = (*poll_env)->GetStaticMethodID(env, clz, method_name, method_sig);
    if ( method == NULL )    {
        ERR("java static method callback_method Not Found;[%s]", method_name);
        return NULL;
    }
    return method;
}

static jmethodID findJavaMethod(JNIEnv* env, jclass clz, char* method_name, char* method_sig)
{
    // find a non-static method
    jmethodID method = (*env)->GetMethodID(env, clz, method_name, method_sig);
    //jmethodID method = (*poll_env)->GetMethodID(env, clz, method_name, method_sig);
    if ( method == NULL )    {
        ERR("java method callback_method Not Found;[%s]", method_name);
        return NULL;
    }
    return method;
}

//////////////////////////////////////////////////////
// run in the im_poll thread 

static int __OnResourceSetRequestHandler(void* pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	DBG("dev_id=[%s], resource_id=[%s], properties_in_jstr=%s\n", dev_id, resource_id, properties_in_jstr);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnResourceSetRequestHandler", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if ( method == NULL )    {
        ERR("java method OnResourceSetRequestHandler Not Found");
        return (int)-1;
    }


    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jstring param2 = (*env)->NewStringUTF(env, resource_id); 
    jstring param3 = (*env)->NewStringUTF(env, properties_in_jstr); 
    jint retVal = 0;

    jobject jobj = poll_obj;

    DBG("jobj=[%p]", poll_obj);
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1, param2, param3);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (int)retVal;
}

static int __OnResourceRetrieveOneRequestHandler(void* pktBody, char *dev_id, char *resource_id)
{
	DBG("dev_id=[%s], resource_id=[%s]\n", dev_id, resource_id);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnResourceRetrieveOneRequestHandler", "(JLjava/lang/String;Ljava/lang/String;)I");
    if ( method == NULL )    {
        ERR("java method OnResourceRetrieveOneRequestHandler Not Found");
        return (int)-1;
    }

    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jstring param2 = (*env)->NewStringUTF(env, resource_id); 
    jint retVal = 0;

    jobject jobj = poll_obj;
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1, param2);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);

	return (int)retVal;
}

static int __OnResourceRetrieveAllRequestHandler(void* pktBody, char *dev_id)
{
	DBG("dev_id=[%s]", dev_id);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnResourceRetrieveAllRequestHandler", "(JLjava/lang/String;)I");
    if ( method == NULL )    {
        ERR("java method OnResourceRetrieveAllRequestHandler Not Found");
        return (int)-1;
    }


    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jint retVal = 0;

    jobject jobj = poll_obj;
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1);

	(*env)->DeleteLocalRef(env, param1);

	return (int)retVal;
}

/////////////////////////////
#if 0
static int __OnSysResourceSetRequestHandler(void* pktBody, char *dev_id, char *resource_id, char *properties_in_jstr)
{
	// !!! USER CODE HERE
	DBG("dev_id=[%s], resource_id=[%s], properties_in_jstr=%s\n", dev_id, resource_id, properties_in_jstr);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnSysResourceSetRequestHandler", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if ( method == NULL )    {
        printf("%s(): java method OnSysResourceSetRequestHandler Not Found\n", __FUNCTION__);
        return (int)-1;
    }


    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jstring param2 = (*env)->NewStringUTF(env, resource_id); 
    jstring param3 = (*env)->NewStringUTF(env, properties_in_jstr); 
    jint retVal = 0;

    jobject jobj = poll_obj;
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1, param2, param3);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (int)retVal;
}

static int __OnSysResourceRetrieveOneRequestHandler(void* pktBody, char *dev_id, char *resource_id)
{
	// !!! USER CODE HERE
	DBG("dev_id=[%s], resource_id=[%s]\n", dev_id, resource_id);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnSysResourceRetrieveOneRequestHandler", "(Ljava/lang/String;Ljava/lang/String;)I");
    if ( method == NULL )    {
        printf("%s(): java method OnSysResourceRetrieveOneRequestHandler Not Found\n", __FUNCTION__);
        return (int)-1;
    }


    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jstring param2 = (*env)->NewStringUTF(env, resource_id); 
    jint retVal = 0;

    jobject jobj = poll_obj;
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1, param2);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);

	return (int)retVal;
}

static int __OnSysResourceRetrieveAllRequestHandler(void* pktBody, char *dev_id)
{
	// !!! USER CODE HERE
	DBG("dev_id=[%s]", dev_id);

    JNIEnv *env = poll_env;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (int)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (int)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "OnSysResourceRetrieveAllRequestHandler", "(Ljava/lang/String;)I");
    if ( method == NULL )    {
        printf("%s(): java method OnSysResourceRetrieveAllRequestHandler Not Found\n", __FUNCTION__);
        return (int)-1;
    }


    jlong param0 = (jlong)pktBody; 
    jstring param1 = (*env)->NewStringUTF(env, dev_id); 
    jint retVal = 0;

    jobject jobj = poll_obj;
    retVal = (*env)->CallIntMethod(env, jobj, method, param0, param1);

	(*env)->DeleteLocalRef(env, param1);

	return (int)retVal;
}
#endif
//
////////////////////////////////////////////////////////////




JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImInit(JNIEnv *env, jobject jobj, 
    jint debug_level)
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

  /********************************************************
  *  디바이스 제어수신 처리기 reset
  ********************************************************/
  pclient->OnResourceSetRequestHandler = NULL;
  pclient->OnResourceRetrieveOneRequestHandler = NULL;
  pclient->OnResourceRetrieveAllRequestHandler = NULL;
  pclient->OnSysResourceSetRequestHandler = NULL;
  pclient->OnSysResourceRetrieveOneRequestHandler = NULL;
  pclient->OnSysResourceRetrieveAllRequestHandler = NULL;


  // JNIEnv *env를 저장
  pclient->host_evn = env;
  pclient->host_object = jobj;

#if 0
    DBG("%s: AttachCurrentThread()", __FUNCTION__);
  if ( (*glpVM)->AttachCurrentThread(glpVM, env, NULL) != JNI_OK ) {
        ERR("%s: AttachCurrentThread() failed", __FUNCTION__);
        return -1;
  }
#endif

#if 0
    DBG("%s: JNI_CreateJavaVM()", __FUNCTION__);
    //JNI_CreateJavaVM(&g_pjvm, (void**)&g_penv, &vm_args);
    rc = JNI_CreateJavaVM((JavaVM**)&(pclient->host_jvm), (void**)&(pclient->host_evn), NULL);
    if ( rc == 0 )
    {
        ERR("%s: FAILED JNI_CreateJavaVM()", __FUNCTION__);
        return -1;
    }
#endif 


  return sess_hndl;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImRelease(JNIEnv *env, jobject jobj, 
    int hndl)
{
  sesspool_free(hndl);
 
#if 0
  if ( (*glpVM)->DetachCurrentThread(glpVM) != JNI_OK ) {
    ERR("%s: DetachCurrentThread() failed", __FUNCTION__);
  }
#endif

  return 0;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnectTo(JNIEnv *env, jobject jobj, 
    int hndl, jstring ec_ip, int ec_port)
{
    char *p_ec_ip = getStringUTFChars(env, ec_ip);

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect_to(pclient, p_ec_ip, ec_port);


    releaseStringUTFChars(env, p_ec_ip, ec_ip);

  return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImDisconnect(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

	im_disconnect(pclient);
  return 0;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnCircuitBreakerOff(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_circuit_breaker_off(pclient);
  return rc;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnResponseWaitOff(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_response_wait_off(pclient);
  return rc;
}



JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthDevice(JNIEnv *env, jobject jobj, 
    int hndl, jstring im_dev_id, jstring im_dev_pw, jstring im_dev_gw)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

    char *p_dev_id = getStringUTFChars(env, im_dev_id);
    char *p_dev_pw = getStringUTFChars(env, im_dev_pw);
    char *p_dev_gw = getStringUTFChars(env, im_dev_gw);

  int rc = im_auth_device(pclient, p_dev_id, p_dev_pw, p_dev_gw);

    releaseStringUTFChars(env, p_dev_id, im_dev_id);
    releaseStringUTFChars(env, p_dev_pw, im_dev_pw);
    releaseStringUTFChars(env, p_dev_gw, im_dev_gw);

    return rc;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthSystem(JNIEnv *env, jobject jobj, 
    int hndl, jstring im_sys_id, jstring im_sys_pw, jstring im_sys_gw)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

    char *p_dev_id = getStringUTFChars(env, im_sys_id);
    char *p_dev_pw = getStringUTFChars(env, im_sys_pw);
    char *p_dev_gw = getStringUTFChars(env, im_sys_gw);

  int rc = im_auth_device(pclient, p_dev_id, p_dev_pw, p_dev_gw);

    releaseStringUTFChars(env, p_dev_id, im_sys_id);
    releaseStringUTFChars(env, p_dev_pw, im_sys_pw);
    releaseStringUTFChars(env, p_dev_gw, im_sys_gw);

    return rc;
}




JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImPoll(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

#if 1
	/****
	native thread에서 jni 관련 함수를 쓰고자 할때는 
	보통 JNIEnv 를 얻는 과정에서 AttachCurrentThread를 한다. 물론 종료되기 전에 DetachCurrentThread를 불러줘야 하고..
	이렇게 아무 생각없이 짝만 맞춰주면서 잘 사용을 하고 있었는데..
	native thread에서 timer를 돌릴 일이 생겼고..
	해당 timer callback에서 jni함수를 사용하니 DetachCurrentThread에서 죽어 나가는것이 아닌가...??
	뭐지 하고 한참을 보았더니..
	timer callback 부르는 쓰레드 ID를 찍어보니..jni 쓰레드였던것...?
	애초에 AttachCurrentThread를 하면 안되는 쓰레드 였다는 결론인데..
	왜 native thread에서 생성시킨 timer의 콜백이 jni의 쓰레드에서 불러주는지 알고 싶진 않고..
	결국 AttachCurrentThread를 하는 올바른 방법은

	먼저 JavaVm 으로 GetEnv(JavaVM* vm, void ** env, jint version)를 한다.
	결과가 JNI_OK 라면 이미 있는 거니까  env 넘겨준 변수로 그냥 사용하면 되고

	JNI_OK가 아니면 AttachCurrentThread()해서 사용하면 된다.
	출처: https://jjban.tistory.com/4 [감성적인 프로그래머:티스토리]
	*/

	int getEnvStat = (*glpVM)->GetEnv(glpVM, &(pclient->host_evn), JNI_VERSION_1_6);
	//DBG("%s: GetEnv()=[%d]", __FUNCTION__, getEnvStat);

	if ( getEnvStat != JNI_OK ) {

		DBG("%s: AttachCurrentThread()", __FUNCTION__);
		if ( (*glpVM)->AttachCurrentThread(glpVM, &(pclient->host_evn), NULL) != JNI_OK ) {
			ERR("%s: AttachCurrentThread() failed", __FUNCTION__);
			return -1;
		}

	}
#endif

  /*
  host_object갱신; 2021-03-08
  (클래스오브젝트의 주소가 동적으로 변경되는 경우 method callback시 jvm crash유발)
  */
  //pclient->host_evn = env;
  pclient->host_object = jobj;

  poll_env = (JNIEnv *)pclient->host_evn;
  poll_obj = pclient->host_object;

    DBG("jobj=[%p]", poll_obj);


  int rc = im_poll(pclient);

#if 0
   if (getEnvStat == JNI_EDETACHED)  
      ;//(*glpVM)->DetachCurrentThread(glpVM);
#endif

  return rc;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImMSleep(JNIEnv *env, jobject jobj, 
    int hndl, int msec)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  im_msleep(msec);
  return 0;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImIsTls(JNIEnv *env, jobject jobj)
{
#ifdef HAVE_OPENSSL
    return (1);
#else
    return (0);
#endif
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetTlsCaFile(JNIEnv *env, jobject jobj, int hndl, jstring cafile)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_cafile = getStringUTFChars(env, cafile);

  int rc = im_client_set_tls_cafile(pclient, p_cafile);
  DBG("cafile=%s", p_cafile);

  releaseStringUTFChars(env, p_cafile, cafile);

  if ( rc < 0  )	{
    ERR("fail im_client_set_tls_cafile()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetTlsCaPath(JNIEnv *env, jobject jobj, int hndl, jstring capath)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_capath = getStringUTFChars(env, capath);

  int rc = im_client_set_tls_capath(pclient, p_capath);
  DBG("capath=%s", p_capath);

  releaseStringUTFChars(env, p_capath, capath);

  if ( rc < 0  )	{
    ERR("fail im_client_set_tls_capath()\n");
    rc = -1;
  }

	return rc;
}



////////////////////////////////////////////////////
// send data device
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceSendNotification(JNIEnv *env, jobject jobj, int hndl, jstring resource_id, jstring properties_in_json)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_resource_name = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = getStringUTFChars(env, properties_in_json);

  int rc = im_resource_send_notification(pclient, p_resource_name, p_json_str);
  DBG("properties_in_json=%s", p_json_str);

  releaseStringUTFChars(env, p_json_str, properties_in_json);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_id);
  }
    
  if ( rc < 0  )	{
    ERR("fail im_resource_send_notification()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationInit(JNIEnv *env, jobject jobj, int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
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

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationAppendResource(JNIEnv *env, jobject jobj, int hndl, jstring resource_id, jstring properties_in_json)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_resource_name = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = getStringUTFChars(env, properties_in_json);

  int rc = im_resource_notification_append_resource(pclient, p_resource_name, p_json_str);
  DBG("properties_in_json=%s", p_json_str);

  releaseStringUTFChars(env, p_json_str, properties_in_json);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_id);
  }
    
  if ( rc < 0  )	{
    ERR("fail im_resource_notification_append_resource()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationSend(JNIEnv *env, jobject jobj, int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
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

////////////////////////////////////////////////////
// send data system
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceSendNotification(JNIEnv *env, jobject jobj, int hndl, jstring resource_id, jstring properties_in_json)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_resource_name = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = getStringUTFChars(env, properties_in_json);

  int rc = im_sys_resource_send_notification(pclient, p_resource_name, p_json_str);
  DBG("properties_in_json=%s", p_json_str);

  releaseStringUTFChars(env, p_json_str, properties_in_json);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_id);
  }
    
  if ( rc < 0  )	{
    ERR("fail im_sys_resource_send_notification()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationInit(JNIEnv *env, jobject jobj, int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_sys_resource_notification_init(pclient);
    
  if ( rc < 0  )	{
    ERR("fail im_sys_resource_notification_init()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationAppendResource(JNIEnv *env, jobject jobj, int hndl, jstring resource_id, jstring properties_in_json)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_resource_name = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = getStringUTFChars(env, properties_in_json);

  int rc = im_sys_resource_notification_append_resource(pclient, p_resource_name, p_json_str);
  DBG("properties_in_json=%s", p_json_str);

  releaseStringUTFChars(env, p_json_str, properties_in_json);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_id);
  }
    
  if ( rc < 0  )	{
    ERR("fail im_sys_resource_notification_append_resource()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationSend(JNIEnv *env, jobject jobj, int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_sys_resource_notification_send(pclient);
    
  if ( rc < 0  )	{
    ERR("fail im_sys_resource_notification_send()\n");
    rc = -1;
  }

	return rc;
}

//
/////////////////////////////////////////


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceRetrieveSetResource(JNIEnv *env, jobject jobj, 
		jlong rep_body, jstring resource_id, jstring properties_in_json)
{
  char *p_resource_id = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = (properties_in_json==NULL)?NULL:getStringUTFChars(env, properties_in_json);

	int rc = im_resource_retrieve_set_resource((void*)rep_body, p_resource_id, p_json_str);
	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_set_resource()\n");
		rc = -1;
	}

  if ( p_resource_id != NULL )	{
    releaseStringUTFChars(env, p_resource_id, resource_id);
  }
  if ( p_json_str != NULL )	{
    releaseStringUTFChars(env, p_json_str, properties_in_json);
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceRetrieveAppendResource(JNIEnv *env, jobject jobj, 
		jlong rep_body, jstring resource_id, jstring properties_in_json)
{

  char *p_resource_id = (resource_id==NULL)?NULL:getStringUTFChars(env, resource_id);
  char *p_json_str = (properties_in_json==NULL)?NULL:getStringUTFChars(env, properties_in_json);

	int rc = im_resource_retrieve_append_resource((void*)rep_body, p_resource_id, p_json_str);
	if ( rc < 0  )	{
		ERR("fail im_resource_retrieve_append_resource()\n");
		rc = -1;
	}

  if ( p_resource_id != NULL )	{
    releaseStringUTFChars(env, p_resource_id, resource_id);
  }
  if ( p_json_str != NULL )	{
    releaseStringUTFChars(env, p_json_str, properties_in_json);
  }

	return rc;
}



JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetControlCallBack(JNIEnv *env, jobject jobj, 
int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (디바이스/시스템)
  ********************************************************/
  pclient->OnResourceSetRequestHandler = __OnResourceSetRequestHandler;
  pclient->OnResourceRetrieveOneRequestHandler = __OnResourceRetrieveOneRequestHandler;
  pclient->OnResourceRetrieveAllRequestHandler = __OnResourceRetrieveAllRequestHandler;
#if 0
  pclient->OnSysResourceSetRequestHandler = __OnSysResourceSetRequestHandler;
  pclient->OnSysResourceRetrieveOneRequestHandler = __OnSysResourceRetrieveOneRequestHandler;
  pclient->OnSysResourceRetrieveAllRequestHandler = __OnSysResourceRetrieveAllRequestHandler;
#else
  pclient->OnSysResourceSetRequestHandler = __OnResourceSetRequestHandler;
  pclient->OnSysResourceRetrieveOneRequestHandler = __OnResourceRetrieveOneRequestHandler;
  pclient->OnSysResourceRetrieveAllRequestHandler = __OnResourceRetrieveAllRequestHandler;
#endif

	return 0;
}


JNIEXPORT void JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_sayHello(JNIEnv *env, jobject jobj, 
    jstring hello)
{
    char *p_hello = getStringUTFChars(env, hello);

    printf("%s(): say [%s]\n", __FUNCTION__, p_hello);

    releaseStringUTFChars(env, p_hello, hello);

#if 0

    poll_env = env;
    DBG("poll_env         =[%p]", poll_env);
    DBG("env              =[%p]", env);

    //__OnResourceSetRequestHandler(NULL, "a", "b", "c");
    //__OnResourceRetrieveAllRequestHandler(NULL, "a");

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return;
    }

    DBG("clz              =[%p]", clz);

    jmethodID method = findJavaMethod(env, clz, "OnResourceRetrieveAllRequestHandler", "(JLjava/lang/String;)I");
    if ( method == NULL )    {
        ERR("java method OnResourceRetrieveAllRequestHandler Not Found");
        return;
    }
    DBG("OnResourceRetrieveAllRequestHandler");


    DBG("jobj=[%p]", poll_obj);
    __OnResourceSetRequestHandler(NULL, "a", "a", "a");
#endif
    return;
}


