#include <jni.h>

#ifndef __IOTMAKERS_JNI__

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImInit(JNIEnv *, jobject, jint debug_level);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImRelease(JNIEnv *, jobject, int hndl);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnectTo(JNIEnv *, jobject, int hndl, jstring  ec_ip, int ec_port);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImDisconnect(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnCircuitBreakerOff(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnResponseWaitOff(JNIEnv *, jobject, int hndl);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthDevice(JNIEnv *, jobject, int hndl, jstring im_dev_id, jstring im_dev_pw, jstring im_dev_gw);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthSystem(JNIEnv *, jobject, int hndl, jstring im_sys_id, jstring im_sys_pw, jstring im_dev_gw);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImPoll(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImMSleep(JNIEnv *, jobject, int hndl, int msec);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImIsTls(JNIEnv *, jobject);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetTlsCaFile(JNIEnv *, jobject, int hndl, jstring cafile);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetTlsCaPath(JNIEnv *, jobject, int hndl, jstring capath);


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceSendNotification(JNIEnv *, jobject, int hndl, jstring resource_id, jstring properties_in_json);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationInit(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationAppendResource(JNIEnv *, jobject, int hndl, jstring resource_id, jstring properties_in_json);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceNotificationSend(JNIEnv *, jobject, int hndl);


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceSendNotification(JNIEnv *, jobject, int hndl, jstring resource_id, jstring properties_in_json);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationInit(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationAppendResource(JNIEnv *, jobject, int hndl, jstring resource_id, jstring properties_in_json);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSysResourceNotificationSend(JNIEnv *, jobject, int hndl);


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceRetrieveSetResource(JNIEnv *, jobject, jlong rep_body, jstring resource_id, jstring properties_in_json);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImResourceRetrieveAppendResource(JNIEnv *, jobject, jlong rep_body, jstring resource_id, jstring properties_in_json);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetControlCallBack(JNIEnv *, jobject, int hndl);


#ifdef __cplusplus
}
#endif

#endif