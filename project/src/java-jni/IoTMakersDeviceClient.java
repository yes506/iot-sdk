package com.kt.iotmakers.device;

import java.io.*;

import com.kt.iotmakers.device.NativeUtils;

public class IoTMakersDeviceClient {

	public interface ControlCallBack {
		public abstract int OnResourceSetRequestHandler(long rep_body, String dev_id, String resource_id, String properties_in_jstr);
		public abstract int OnResourceRetrieveOneRequestHandler(long rep_body, String dev_id, String resource_id);
		public abstract int OnResourceRetrieveAllRequestHandler(long rep_body, String dev_id);

		/**
		public abstract int OnSysResourceSetRequestHandler(long rep_body, String sys_id, String resource_id, String properties_in_jstr);
		public abstract int OnSysResourceRetrieveOneRequestHandler(long rep_body, String sys_id, String resource_id);
		public abstract int OnSysResourceRetrieveAllRequestHandler(long rep_body, String sys_id);
		*/
    }

	private ControlCallBack callback;


    public IoTMakersDeviceClient() {
        this.callback = null;
    }
 
    public void setCallBack(int hndl, ControlCallBack callback) {
        this.callback = callback;
		
		ImSetControlCallBack(hndl);
    }


	public int OnResourceSetRequestHandler(long rep_body, String dev_id, String resource_id, String properties_in_jstr){
		if ( callback != null )		{
			return this.callback.OnResourceSetRequestHandler(rep_body, dev_id, resource_id, properties_in_jstr);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}
	public int OnResourceRetrieveOneRequestHandler(long rep_body, String dev_id, String resource_id){
		if ( callback != null )		{
			return this.callback.OnResourceRetrieveOneRequestHandler(rep_body, dev_id, resource_id);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}
	public int OnResourceRetrieveAllRequestHandler(long rep_body, String dev_id){
		if ( callback != null )		{
			return this.callback.OnResourceRetrieveAllRequestHandler(rep_body, dev_id);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}

	/**
	public int OnSysResourceSetRequestHandler(long rep_body, String dev_id, String resource_id, String properties_in_jstr){
		if ( callback != null )		{
			return this.callback.OnSysResourceSetRequestHandler(rep_body, dev_id, resource_id, properties_in_jstr);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}
	public int OnSysResourceRetrieveOneRequestHandler(long rep_body, String dev_id, String resource_id){
		if ( callback != null )		{
			return this.callback.OnSysResourceRetrieveOneRequestHandler(rep_body, dev_id, resource_id);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}
	public int OnSysResourceRetrieveAllRequestHandler(long rep_body, String dev_id){
		if ( callback != null )		{
			return this.callback.OnSysResourceRetrieveAllRequestHandler(rep_body, dev_id);
		} else {
			System.out.println("IoTMakersDeviceClient callback is null");
		}

		return 0;
	}
	*/

	static {
		
		// export LD_LIBRARY_PATH=./
		//java -Djava.library.path=./jni-libs -classpath ./jni-libs/libiotmakers-std-device.jar:./jni-libs:./classes  Hello
		//System.out.println("1 iotmakers_3rdparty_sdk_1.1.10;loadLibrary; iotmakers-std-device-jni");
		//System.loadLibrary("iotmakers-std-device-jni");
		String os_name = System.getProperty("os.name");	// "Windows ...", "Linux"
		os_name = (os_name.contains("Windows"))?"Windows":(os_name.contains("Linux"))?"Linux":"UnknownOS";

		String os_arch = System.getProperty("os.arch"); // "amd64", "x86"
		os_arch = (os_arch.contains("amd64"))?"amd64":(os_arch.contains("x86"))?"x86":(os_arch.contains("i386"))?"x86":(os_arch.contains("arm"))?"arm":"UnknownArchitecture";

		String libname = (os_name.contains("Windows"))?"libiotmakers-std-device-jni.dll":"libiotmakers-std-device-jni.so";

		/* 2021-01-19
			libiotmakers-std-device-jni.jar/jni-libs/Windows/amd64/libiotmakers-std-device-jni.dll
			libiotmakers-std-device-jni.jar/jni-libs/Windows/x86/libiotmakers-std-device-jni.dll
			libiotmakers-std-device-jni.jar/jni-libs/Linux/amd64/libiotmakers-std-device-jni.so
			libiotmakers-std-device-jni.jar/jni-libs/Linux/x86/libiotmakers-std-device-jni.so
		*/

        try {
            NativeUtils.loadLibraryFromJar("/jni-libs/" + os_name + "/" + os_arch + "/" + libname);   
        } catch (IOException e) {
            // This is probably not the best way to handle exception :-)    
            //e.printStackTrace();
			System.out.println("Fail loadLibraryFromJar;" + libname);

			//////////////////////////////////////////////////////////////////////////////
			// TODO : Android에서 NativeUtils.loadLibraryFromJar() 미작동
			// 일단 지금(2022-07-04)은 AndroidProject\app\src\main\jniLibs\{arch}\* 에서 로딩
			//////////////////////////////////////////////////////////////////////////////
			System.out.println("Trying loadLibrary;iotmakers_3rdparty_sdk;iotmakers-std-device-jni");
			System.loadLibrary("iotmakers-std-device-jni");
			
			IoTMakersDeviceClient tc = new IoTMakersDeviceClient();
			if (tc.ImIsTls() == (1))
			{
				System.out.println("Trying loadLibrary;ssl");
				System.loadLibrary("ssl");
				System.out.println("Trying loadLibrary;crypto");
				System.loadLibrary("crypto");
			}
        }
	}
	

	public native void sayHello(String hello);
	public native int ImInit(int debug_level);                                                                                    
	public native int ImRelease(int hndl);                                                                                        
	public native int ImConnectTo(int hndl, String  ec_ip, int ec_port);                                                           
	public native int ImDisconnect(int hndl);                                                                                     
	public native int ImTurnCircuitBreakerOff(int hndl);                                                                          
	public native int ImTurnResponseWaitOff(int hndl);                                                                          
	public native int ImAuthDevice(int hndl, String im_dev_id, String im_dev_pw, String im_dev_gw);                                 
	public native int ImAuthSystem(int hndl, String im_sys_id, String im_sys_pw, String im_sys_gw);                                 
	public native int ImPoll(int hndl);                                                                                           
	public native int ImMSleep(int hndl, int msec);

	public native int ImIsTls();     
	public native int ImSetTlsCaFile(int hndl, String cafile);
	public native int ImSetTlsCaPath(int hndl, String capath);


	public native int ImResourceSendNotification(int hndl, String resource_id, String properties_in_json);
	public native int ImResourceNotificationInit(int hndl);
	public native int ImResourceNotificationAppendResource(int hndl, String resource_id, String properties_in_json);
	public native int ImResourceNotificationSend(int hndl);

	public native int ImSysResourceSendNotification(int hndl, String resource_id, String properties_in_json);
	public native int ImSysResourceNotificationInit(int hndl);
	public native int ImSysResourceNotificationAppendResource(int hndl, String resource_id, String properties_in_json);
	public native int ImSysResourceNotificationSend(int hndl);

	public native int ImResourceRetrieveSetResource(long rep_body, String resource_id, String properties_in_json);                                                                                   
	public native int ImResourceRetrieveAppendResource(long rep_body, String resource_id, String properties_in_json);                                                                                   

	private native int ImSetControlCallBack(int hndl);                                                                                    

}


