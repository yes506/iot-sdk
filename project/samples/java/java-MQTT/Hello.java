import com.kt.iotmakers.device.IoTMakersDeviceClient;

public class Hello
{
	///////////////////////////////////////////////////
	// The native methods in "libiotmakers-std-device.jar"
	//
	// public native int ImInit(int debug_level);                                                                                    
	// public native int ImRelease(int hndl);                                                                                        
	// public native int ImConnectTo(int hndl, String  ec_ip, int ec_port);                                                           
	// public native int ImConnect(int hndl);                                                                                        
	// public native int ImDisconnect(int hndl);                                                                                     
	// public native int ImTurnCircuitBreakerOff(int hndl);                                                                          
	// public native int ImTurnResponseWaitOff(int hndl);                                                                          
	// public native int ImAuthDevice(int hndl, String im_dev_id, String im_dev_pw, String im_dev_gw);                                 
	// public native int ImPoll(int hndl);                                                                                           
	// public native int ImMSleep(int hndl, int msec);
	// public native int ImSendDataOnDev(int hndl, String json_str, String resource_name, String dev_id);                                             
	// public native int ImSendData(int hndl, String json_str, String resource_name);                                                                
	// public native int ImSetReportDataOnDev(long rep_body, String json_str, String resource_name, String dev_id);
	///////////////////////////////////////////////////

	public static void main(String[] args) {

		int ret = 0;

		///////////////////////////////////////////////////
		// IoTMakers Device init
		///////////////////////////////////////////////////
		IoTMakersDeviceClient cli = new IoTMakersDeviceClient();

		if ( args.length < 5 )
		{
			System.out.println("Usage : Hello {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}");
			return;
		}

		String ecIp   = args[0];
		int    ecPort = Integer.parseInt(args[1]);
		String dev_id  = args[2];
		String devPw  = args[3];
		String devGw  = args[4];


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
	
			String g_report_str = "";

            @Override
			public String onControlStringData(String dev_id, String resource_name, String prop_name, String prop_value){
				System.out.println("=================================================================");
				System.out.println("onControlStringData prop_name="+ prop_name +", prop_value="+ prop_value);
				System.out.println("=================================================================");

				g_report_str = String.format("{\"%s\":\"%s\"}", prop_name, prop_value);

				return prop_value;
            }

            @Override
			public int onControlIntegerData(String dev_id, String resource_name, String prop_name, int prop_value){
				System.out.println("=================================================================");
				System.out.println("onControlIntegerData prop_name="+ prop_name +", prop_value="+ prop_value);
				System.out.println("=================================================================");

				g_report_str = String.format("{\"%s\":%d}", prop_name, prop_value);

				return prop_value;
            }

            @Override
			public double onControFloatData(String dev_id, String resource_name, String prop_name, double prop_value){
				System.out.println("=================================================================");
				System.out.println("onControFloatData prop_name="+ prop_name +", prop_value="+ prop_value);
				System.out.println("=================================================================");

				g_report_str = String.format("{\"%s\":%f}", prop_name, prop_value);

				return prop_value;
            }

            @Override
			public double onControlNumberData(String dev_id, String resource_name, String prop_name, double prop_value){
				System.out.println("=================================================================");
				System.out.println("onControlNumberData prop_name="+ prop_name +", prop_value="+ prop_value);
				System.out.println("=================================================================");

				g_report_str = String.format("{\"%s\":%f}", prop_name, prop_value);

				return prop_value;
            }

			@Override
			public boolean onControlBoolData(String dev_id, String resource_name, String prop_name, boolean prop_value){
				System.out.println("=================================================================");
				System.out.println("onControlBoolData prop_name="+ prop_name +", prop_value="+ prop_value);
				System.out.println("=================================================================");

				g_report_str = String.format("{\"%s\":%s}", prop_name, (prop_value==true)?"true":"false");

				return prop_value;
            }

			@Override
			public int onEndOfControl(IoTMakersDeviceClient cli, String dev_id, String resource_name, long rep_body){
				System.out.println("=================================================================");
				System.out.println("onEndOfControl dev_id="+ dev_id +", resource_name="+ resource_name);
				System.out.println("onEndOfControl g_report_str="+ g_report_str);
				System.out.println("=================================================================");

				// Report in JSON String
				if ( g_report_str.equals("") != true )
				{
					cli.ImSetReportDataOnDev(rep_body, g_report_str, resource_name, dev_id);
					g_report_str = "";
				} 

				// Result Code of End Of Control
				return (100); // code: 100=OK, 200=FAIL
            }
		
		});

		///////////////////////////////////////////////////
		// connect to Server for Open Platform
		//     int ImConnect(int hndl);      
		///////////////////////////////////////////////////
		if ( cli.ImConnectTo(hndl, ecIp, ecPort) < 0 )	{
			return;
		}

		///////////////////////////////////////////////////
		// Auth Device
		///////////////////////////////////////////////////
		if ( cli.ImAuthDevice(hndl, dev_id, devPw, devGw) < 0 )	{
			return;
		}

		while (true)
		{
			///////////////////////////////////////////////////
			// Send Sensing data in JSON
			///////////////////////////////////////////////////
			String resource_name = "test";
			//String jdata = "{\"LED\":\"OFF\"}";

			String jdata = "{\"Temp\":25.5, \"status\":\"hello\"}";
			/*
			String jdata = "{\"Temp\":25.5, "
							"\"Hum\":50, "
							"\"status\":\"hello\", "
							"\"TRUE\":true, "
							"\"FALSE\":false, "
							"\"DATE\":\"2020-10-10 10:10:10.100\", "

							"\"ARR_INT\":[0,1,2,3,4], "
							"\"ARR_FLT\":[0.5,1.5,2.5,3.5,4.5], "
							"\"ARR_STR\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
							"\"ARR_DATE\":[\"2020-10-11 10:10:10.100\", \"2020-10-12 10:10:10.100\"], "
							"\"ARR_BOOL\":[true, true, false]"
							"}";
			*/

			//////////////////////////////////////
			// send collection data
			ret = cli.ImSendData(hndl, jdata, resource_name);
	
			//////////////////////////////////////
			// sub_device, group_tag_name
			/**
			String subDevId = null;
			ret = cli.ImSendDataOnDev(hndl, jdata, resource_name, subDevId);
			*/	
			if (  ret < 0 )			{
				System.out.println("FAIL sent : " + jdata);
				break;
			}

			cli.ImMSleep(hndl, 3*1000);

			///////////////////////////////////////////////////
			// NetIO polling
			///////////////////////////////////////////////////
			if ( cli.ImPoll(hndl) < 0 ) {
				break;
			}

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

}
