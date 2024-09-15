/** 
 *  Copyright (c) 2022 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */
import com.kt.iotmakers.device.IoTMakersDeviceClient;

public class HelloThread
{
	///////////////////////////////////////////////////
	// IoTMakers Device init
	///////////////////////////////////////////////////
	IoTMakersDeviceClient cli = new IoTMakersDeviceClient();

	int thread_run_flag = 0;

	/*
	#   리소스 값 전송 예제
	*/
	public int example_resource_send_notification(int hndl)
	{
		String resource_id = "testAllTypeUri";
		String jdata = "{\"data01\":100, \"text01\":\"hello\"}";

		System.out.println(jdata);

		// 1. 전송 패킷 준비
		int rc = cli.ImResourceNotificationInit(hndl);
		if (rc < 0 ) {
			System.out.println("fail ImResourceNotificationInit()");
			return -1;
		}

		// 2. 패킷에 리소스 추가; append FIRST RESOURCE
		rc = cli.ImResourceNotificationAppendResource(hndl, resource_id, jdata);
		if (rc < 0 ) {
			System.out.println("fail ImResourceNotificationAppendResource()");
			return -1;
		}

		// 3. 패킷 전송
		rc = cli.ImResourceNotificationSend(hndl);
		if (rc < 0 ) {
			System.out.println("fail ImResourceNotificationSend()");
			return -1;
		}

		System.out.println("OK, SENT: " + jdata);
		return 0;
	}



	public class ThreadPollSample implements Runnable{
		
		int hndl = 0;

		public ThreadPollSample(int _hndl){
			hndl = _hndl;
		}
		
		@Override public void run() {
			while (thread_run_flag==(1))
			{
				///////////////////////////////////////////////////
				// NetIO polling
				///////////////////////////////////////////////////

				if ( cli.ImPoll(hndl) < 0 ) {
					thread_run_flag = 0;
					System.out.println("FAIL ImPoll()...");
					break;
				}

				cli.ImMSleep(hndl, 1*1000);
			}
			System.out.println("ThreadPollSample()..., DONE.\n");
		}
	}


	public class ThreadNotificationSample implements Runnable{
		
		int hndl = 0;

		public ThreadNotificationSample(int _hndl){
			hndl = _hndl;
		}
		
		@Override public void run() {
			while (thread_run_flag==(1))
			{
				int rc = example_resource_send_notification(hndl);
				if ( rc < 0 ){
					thread_run_flag = 0;
					break;
				}

				cli.ImMSleep(hndl, 1*1000);
			}
			System.out.println("ThreadNotificationSample()..., DONE.\n");
		}
	}




	public void run_main(String  ec_ip, int  ec_port, String dev_id ,String dev_pw, String svc_gw) {
		int ret = 0;

		int log_level = 3;	// 1=ERROR, 2=INFO, 3=DEBUG

		int hndl = cli.ImInit(log_level);
		if ( hndl < 0 )	{
			System.out.println("FAIL cli.ImInit()");
			return;
		}

		///////////////////////////////////////////////////
		// Register Device Control Handler
		///////////////////////////////////////////////////
		cli.setCallBack(hndl, new IoTMakersDeviceClient.ControlCallBack() {
	
            @Override
			public int OnResourceSetRequestHandler(long rep_body, String dev_id, String resource_id, String properties_in_jstr){
				System.out.println("=================================================================");
				System.out.println("OnResourceSetRequestHandler dev_id="+ dev_id +", resource_id="+ resource_id  +", properties_in_jstr="+ properties_in_jstr);
				System.out.println("=================================================================");
				// YOUR CONTROL CODE HERE

				//IM_RESP_CODE_2004_Changed
				return 2004;
            }

            @Override
			public int OnResourceRetrieveOneRequestHandler(long rep_body, String dev_id, String resource_id){
				System.out.println("=================================================================");
				System.out.println("OnResourceRetrieveOneRequestHandler dev_id="+ dev_id +", resource_id="+ resource_id);
				System.out.println("=================================================================");
				
				// YOUR CONTROL CODE HERE
				String jdata = "{\"data01\":100, \"text01\":\"hello\"}";

				int rc = cli.ImResourceRetrieveSetResource(rep_body, resource_id, jdata);
				if (rc < 0 ) {
					System.out.println("fail ImResourceRetrieveSetResource()");
					return 5000;
				}
				
				//IM_RESP_CODE_2000_OK
				return 2000;
            }

            @Override
			public int OnResourceRetrieveAllRequestHandler(long rep_body, String dev_id){
				System.out.println("=================================================================");
				System.out.println("OnResourceRetrieveAllRequestHandler dev_id="+ dev_id);
				System.out.println("=================================================================");
				
				// YOUR CONTROL CODE HERE
				
				String resource_id = "testAllTypeUri";
				String jdata = "{\"data01\":100, \"text01\":\"hello\"}";
				int rc = cli.ImResourceRetrieveAppendResource(rep_body, resource_id, jdata);
				if (rc < 0 ) {
					System.out.println("fail ImResourceRetrieveAppendResource()");
					return 5000;
				}
				
				String resource_id2 = "testAllTypeUri2";
				String jdata2 = "{\"data01\":200, \"text01\":\"world\"}";
				rc = cli.ImResourceRetrieveAppendResource(rep_body, resource_id2, jdata2);
				if (rc < 0 ) {
					System.out.println("fail ImResourceRetrieveAppendResource()");
					return 5000;
				}

				//IM_RESP_CODE_2000_OK
				return 2000;
            }
		
		});


		///////////////////////////////////////////////////
		// -- 다중쓰레드 고속전송 옵션 --
		cli.ImTurnCircuitBreakerOff(hndl); // 1건/초 제한해제
		cli.ImTurnResponseWaitOff(hndl);   // 서버응답 대기안함
		///////////////////////////////////////////////////


		///////////////////////////////////////////////////
		// connect to Server for Open Platform
		//     int ImConnect(int hndl);      
		///////////////////////////////////////////////////
		if ( cli.ImConnectTo(hndl, ec_ip, ec_port) < 0 )	{
			return;
		}

		///////////////////////////////////////////////////
		// Auth Device
		///////////////////////////////////////////////////
		if ( cli.ImAuthDevice(hndl, dev_id, dev_pw, svc_gw) < 0 )	{
			return;
		}


		thread_run_flag = 1;

		Thread t_send = new Thread(new ThreadNotificationSample(hndl));
		t_send.start();
		
		Thread t_poll = new Thread(new ThreadPollSample(hndl));
		t_poll.start();

		while (thread_run_flag==(1))
		{
			cli.ImMSleep(hndl, 1*1000);
		}


		///////////////////////////////////////////////////
		// disconnect
		///////////////////////////////////////////////////
		System.out.println("disconnect hndl = " + hndl);
		cli.ImDisconnect(hndl);

		///////////////////////////////////////////////////
		// release
		///////////////////////////////////////////////////
		System.out.println("release hndl = " + hndl);
		cli.ImRelease(hndl);
	
	}
	



	public static void main(String[] args) {

		/*
		String ec_ip   = "dev-gw-iotmakers.cloud.kt.com";
		int    ec_port = 30105;
		String dev_id  = "testbyw111";
		String dev_pw  = "12312";
		String svc_gw  = "KT_3RD_BYW";
		*/

		String ec_ip   = "dev-gw-iotmakers.cloud.kt.com";
		int    ec_port = 30026;
		String dev_id  = "testbyw111";
		String dev_pw  = "12312";
		String svc_gw  = "KT_3RD_BYW";

		if ( args.length == 5 )
		{
			ec_ip   = args[0];
			ec_port = Integer.parseInt(args[1]);
			dev_id  = args[2];
			dev_pw  = args[3];
			svc_gw  = args[4];
		}

		HelloThread hello = new HelloThread();
		hello.run_main(ec_ip, ec_port, dev_id, dev_pw, svc_gw);
	}


}
