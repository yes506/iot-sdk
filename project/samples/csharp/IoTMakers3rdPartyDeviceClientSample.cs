/** 
 *  Copyright (c) 2021 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

using System;
using System.Threading;

public class IoTMakers3rdPartyDeviceClientSample {

	static IoTMakers3rdPartyClient client = new IoTMakers3rdPartyClient();

	////////////////////////////////////////////////////
	/* 콜백 응답코드 예시
	#define IM_RESP_CODE_2000_OK			2000
	#define IM_RESP_CODE_2004_Changed		2004
	#define IM_RESP_CODE_4000_BadRequest	4000
	#define IM_RESP_CODE_4004_NotFound		4004
	#define IM_RESP_CODE_5000_InternalError	5000
	*/
	////////////////////////////////////////////////////

	static int OnResourceSetRequest (IntPtr pktBody, string dev_id, string resource_id, string properties_in_jstr)	
	{
        Console.WriteLine("================================================================");
		Console.WriteLine("OnResourceSetRequest;dev_id={0}, resource_id={1}, properties_in_jstr={2} ", dev_id, resource_id, properties_in_jstr);
        Console.WriteLine("================================================================");

		return 2004;	//IM_RESP_CODE_2004_Changed
	}

	static int OnResourceRetrieveOneRequest (IntPtr pktBody, string dev_id, string resource_id)	
	{
        Console.WriteLine("================================================================");
		Console.WriteLine("OnResourceRetrieveOneRequest;dev_id={0}, resource_id={1}", dev_id, resource_id);
        Console.WriteLine("================================================================");

		string properties = "{"
					  + "\"ectestString\": \"hello\", "
					  + "\"ectestDouble\": 99.55, "
					  + "\"ectestInteger\": 100, "
					  + "\"ectestBoolean\": true"
					+ "}";

		int rc = client.ResourceRetrieveSetResource(pktBody, resource_id, properties);
		if ( rc < 0 )    {
			Console.WriteLine("fail im_resource_notification_append()");
			return 5000;	//IM_RESP_CODE_5000_InternalError;
		}

		return 2000;	//IM_RESP_CODE_2000_OK;
	}

	static int OnResourceRetrieveAllRequest (IntPtr pktBody, string dev_id)	
	{
        Console.WriteLine("================================================================");
		Console.WriteLine("OnResourceRetrieveAllRequest;dev_id={0}", dev_id);
        Console.WriteLine("================================================================");

		// 1. First One
		string properties = "{"
					  + "\"ectestString\": \"hello\", "
					  + "\"ectestDouble\": 99.55, "
					  + "\"ectestInteger\": 100, "
					  + "\"ectestBoolean\": true"
					+ "}";

		string resource_id = "testAllTypeUri";

		int rc = client.ResourceRetrieveSetResource(pktBody, resource_id, properties);
		if ( rc < 0 )    {
			Console.WriteLine("fail ImResourceRetrieveSetResource()");
			return 5000;	//IM_RESP_CODE_5000_InternalError;
		}

		// 2. Second One
		string properties2 = "{"
					  + "\"ectestInteger\": 100, "
					  + "\"ectestBoolean\": true"
					+ "}";

		string resource_id2 = "testAllTypeUri";
		rc = client.ResourceRetrieveAppendResource(pktBody, resource_id2, properties2);
		if ( rc < 0 )    {
			Console.WriteLine("fail ImResourceRetrieveAppendResource()");
			return 5000;	//IM_RESP_CODE_5000_InternalError;
		}

		return 2000;	//IM_RESP_CODE_2000_OK;
	}

	static int example_resource_send_notification()
	{
		int rc = -1;

		// 1. 전송 패킷 준비
		rc = client.SysResourceNotificationInit();
		if ( rc < 0 )    {
			Console.WriteLine("fail imSysResourceNotificationInit()");
			return -1;
		  }

		string properties = "{"
						  + "\"ectestString\": \"hello\", "
						  + "\"ectestDouble\": 99.55, "
						  + "\"ectestInteger\": 100, "
						  + "\"ectestBoolean\": true"
						+ "}";

		string resource_id = "testAllTypeUri";

		// 2. 패킷에 리소스 추가
		rc = client.SysResourceNotificationAppendResource(resource_id, properties);
		if ( rc < 0 )    {
			Console.WriteLine("fail SysResourceNotificationAppendResource()");
			return -1;
		  }

		// 3. 패킷 전송
		rc = client.SysResourceNotificationSend();
		if ( rc < 0 )    {
			Console.WriteLine("fail SysResourceNotificationSend()");
			return -1;
		  }

		return rc;
	}	
	//
	///////////////////////////////////////////////////////////////////////


	//##########################################
	// 수신 처리용 쓰레드
	//##########################################
	static void _thread_poll_sample()
	{
		int rc = 0;
		while (true){
			rc = client.Poll();
			if ( rc < 0 ) {
				Console.WriteLine("FAIL ImPoll()...");
				break;
			};

			client.MSleep(1*100);
		}
	}
 
	//##########################################
	//# 리소스 Notofication 전송 쓰레드
	//##########################################
	static void _thread_notification_sample()
	{
		int rc = 0;
		while (true){
			//# 리소스 전송 예제
			rc = example_resource_send_notification();
			if (rc < 0 ){
				Console.WriteLine("FAIL example_resource_send_notification()...");
				break;
			}
			
			Console.WriteLine("OK; device resource notification sent.");

			client.MSleep(1*1000);
		}
	}



	static void Main(string[] args)
    {
		if ( args.Length < 5 )
		{
			Console.WriteLine("Usage : Hello {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}");
			return;
		}

		string ecIp   = args[0];
		int    ecPort = Int32.Parse(args[1]);
		string dev_id  = args[2];
		string dev_pw  = args[3];
		string svc_gw  = args[4];
	
		///////////////////////////////////////////////////
		// IoTMakers Device Init
		///////////////////////////////////////////////////
		int log_level = 2;	// 1=ERR, 2=INFO, 3=DEBUG
		if ( client.Init(log_level) < 0 )		{
	        Console.WriteLine("fail Init()");
			return;
		}


		///////////////////////////////////////////////////
		// Register System Control Handler
		///////////////////////////////////////////////////
		IoTMakers3rdPartyClient.IMOnResourceSetRequestDelegate			OnResourceSetRequestHndl = new IoTMakers3rdPartyClient.IMOnResourceSetRequestDelegate(OnResourceSetRequest);
		IoTMakers3rdPartyClient.IMOnResourceRetrieveOneRequestDelegate	OnResourceRetrieveOneRequestHndl = new IoTMakers3rdPartyClient.IMOnResourceRetrieveOneRequestDelegate(OnResourceRetrieveOneRequest);
		IoTMakers3rdPartyClient.IMOnResourceRetrieveAllRequestDelegate	OnResourceRetrieveAllRequestHndl = new IoTMakers3rdPartyClient.IMOnResourceRetrieveAllRequestDelegate(OnResourceRetrieveAllRequest);

		client.SetControlCallBackHandler(OnResourceSetRequestHndl, OnResourceRetrieveOneRequestHndl, OnResourceRetrieveAllRequestHndl);
	
	
		///////////////////////////////////////////////////
		// 다중쓰레드 고속전송 옵션
		///////////////////////////////////////////////////
		client.TurnCircuitBreakerOff();
		//client.TurnResponseWaitOff();

	
		///////////////////////////////////////////////////
		// Connect to Server
		///////////////////////////////////////////////////
		if ( client.ConnectTo(ecIp, ecPort) < 0 )		{
	        Console.WriteLine("fail ConnectTo()");
			return;
		}


		///////////////////////////////////////////////////
		// Auth System
		///////////////////////////////////////////////////
		if ( client.AuthDevice(dev_id, dev_pw, svc_gw) < 0 )	{
			return;
		}


		// 수신 처리용 쓰레드 생성
		Console.WriteLine("_thread_notification_sample()...");
		Thread t_send = new Thread(new ThreadStart(_thread_notification_sample));
		t_send.Start();

		// 리소스 Notofication 전송 쓰레드 생성
		Console.WriteLine("_thread_poll_sample()...");
		Thread t_poll = new Thread(new ThreadStart(_thread_poll_sample));
		t_poll.Start();

		while (true)
		{
			client.MSleep(1*1000);
		}

		///////////////////////////////////////////////////
		// Disconnect
		///////////////////////////////////////////////////
		client.Disconnect();

		///////////////////////////////////////////////////
		// Release
		///////////////////////////////////////////////////
		client.Release();
	}
}


