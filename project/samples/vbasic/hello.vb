'/** 
' *  Copyright (c) 2018 KT Corp. All rights reserved.
' *
' *  This is a proprietary software of KT corp, and you may not use this file except in
' *  compliance with license agreement with KT corp. Any redistribution or use of this
' *  software, with or without modification shall be strictly prohibited without prior written
' *  approval of KT corp, and the copyright notice above does not evidence any actual or
' *  intended publication of such software.
' *
' */

Imports System
Imports System.IO
Imports System.Runtime.InteropServices
Imports System.Threading

Module Module1

	'///////////////////////////////////////////////////////////////////////
	'// IoTMakers Device API : HowTo Declare
 	Private Declare Function ImInit			 Lib "libiotmakers-std-device.dll" (ByVal log_level As Integer) As Integer
 	Private Declare Function ImRelease		 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer) As Integer
 	Private Declare Function ImConnectTo	 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal ec_ip As String, ByVal ec_port As Integer) As Integer
 	Private Declare Function ImConnect		 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer) As Integer
 	Private Declare Function ImDisconnect	 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer) As Integer
 	Private Declare Function ImTurnCircuitBreakerOff	Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer) As Integer
 	Private Declare Function ImAuth_device	 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal im_dev_id As String, ByVal im_dev_id As String, ByVal im_dev_gw As String) As Integer
 	Private Declare Function ImPoll			 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer) As Integer
 	Private Declare Function ImMSleep		 Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal hndl As Integer) As Integer
 	Private Declare Function ImSendData      Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal json_str As String, ByVal resource_name As String) As Integer
 	Private Declare Function ImSendDataOnDev   Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal json_str As String, ByVal resource_name As String, ByVal dev_id As String) As Integer

	Private Declare Sub ImSet_strdata_handler Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal func As IMStrDataDelegate)
	Private Declare Sub ImSet_numdata_handler Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal func As IMNumDataDelegate)

	Private Declare Sub ImSet_intdata_handler Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal func As IMIntDataDelegate)
	Private Declare Sub ImSet_floatdata_handler Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal func As IMFloatDataDelegate)
	Private Declare Sub ImSet_booldata_handler Lib "libiotmakers-std-device.dll" (ByVal hndl As Integer, ByVal func As IMBoolDataDelegate)
	

	'///////////////////////////////////////////////////////////////////////
	'// IoTMakers Device Control Handler
	'///////////////////////////////////////////////////////////////////////
	Delegate Sub IMStrDataDelegate(ByRef cli As IntPtr, ByVal dev_id As String, ByVal resource_name As String, ByVal prop_name As String, ByVal prop_value As String) As String
	Delegate Sub IMNumDataDelegate(ByRef cli As IntPtr, ByVal dev_id As String, ByVal resource_name As String, ByVal prop_name As String, ByVal prop_value As Double) As Double
	Delegate Sub IMIntDataDelegate(ByRef cli As IntPtr, ByVal dev_id As String, ByVal resource_name As String, ByVal prop_name As String, ByVal prop_value As Integer) As Integer
	Delegate Sub IMFloatDataDelegate(ByRef cli As IntPtr, ByVal dev_id As String, ByVal resource_name As String, ByVal prop_name As String, ByVal prop_value As Single) As String
	Delegate Sub IMBoolDataDelegate(ByRef cli As IntPtr, ByVal dev_id As String, ByVal resource_name As String, ByVal prop_name As String, ByVal prop_value As Integer) As Integer


	Sub im_strdata_handler (ByRef cli As IntPtr, ByVal dev_id As String, ByVal prop_name As String, ByVal prop_value As String)
        Console.WriteLine("================================================================");
        Console.WriteLine("im_strdata_handler;dev_id={0}, prop_name={1}, prop_value={2} ", dev_id, prop_name, prop_value)
        Console.WriteLine("================================================================");
		Return prop_value;
	End Sub
	
	Sub im_numdata_handler (ByRef cli As IntPtr, ByVal dev_id As String, ByVal prop_name As String, ByVal prop_value As Double)
        Console.WriteLine("================================================================");
        Console.WriteLine("im_strdata_handler;dev_id={0}, prop_name={1}, prop_value={2} ", dev_id, prop_name, prop_value)
        Console.WriteLine("================================================================");
		Return prop_value;
	End Sub
	
	Sub im_intdata_handler (ByRef cli As IntPtr, ByVal dev_id As String, ByVal prop_name As String, ByVal prop_value As Integer)
        Console.WriteLine("================================================================");
        Console.WriteLine("im_strdata_handler;dev_id={0}, prop_name={1}, prop_value={2} ", dev_id, prop_name, prop_value)
        Console.WriteLine("================================================================");
		Return prop_value;
	End Sub
	
	Sub im_floatdata_handler (ByRef cli As IntPtr, ByVal dev_id As String, ByVal prop_name As String, ByVal prop_value As Single)
        Console.WriteLine("================================================================");
        Console.WriteLine("im_strdata_handler;dev_id={0}, prop_name={1}, prop_value={2} ", dev_id, prop_name, prop_value)
        Console.WriteLine("================================================================");
		Return prop_value;
	End Sub
	
	Sub im_booldata_handler (ByRef cli As IntPtr, ByVal dev_id As String, ByVal prop_name As String, ByVal prop_value As Integer)
        Console.WriteLine("================================================================");
        Console.WriteLine("im_strdata_handler;dev_id={0}, prop_name={1}, prop_value={2} ", dev_id, prop_name, prop_value)
        Console.WriteLine("================================================================");
		Return prop_value;
	End Sub
	'//
	'///////////////////////////////////////////////////////////////////////

   
	Sub Main()
		Dim hndl As Integer

		Dim ret As Integer

		Dim clArgs() As String = Environment.GetCommandLineArgs()

		If clArgs.Length < 5 Then
			Console.WriteLine("Usage : Hello {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}")
			return
		End If


		'//////////////////////////////////////////////////
		'// IoTMakers Device API Sample

		Dim ecIp As String = clArgs(1)
		Dim ecPort As Integer = Convert.toInt32(clArgs(2))
		Dim dev_id As String = clArgs(3)
		Dim devPw As String = clArgs(4)
		Dim devGw As String = clArgs(5)
   

		'///////////////////////////////////////////////////
		'// IoTMakers Device Init
		'///////////////////////////////////////////////////
		Dim log_level As Integer = 3	' 1, 2, 3
		hndl = ImInit(log_level)
		If hndl < 0 Then  
			return
		End If


		'///////////////////////////////////////////////////
		'// Register Device Control Handler
		'///////////////////////////////////////////////////
		Dim cb_strdata_handler As IMStrDataDelegate = AddressOf im_strdata_handler
		Dim cb_numdata_handler As IMNumDataDelegate = AddressOf im_numdata_handler
		Dim cb_intdata_handler As IMIntDataDelegate = AddressOf im_intdata_handler
		Dim cb_floatdata_handler As IMFloatDataDelegate = AddressOf im_floatdata_handler
		Dim cb_booldata_handler As IMBoolDataDelegate = AddressOf im_booldata_handler
		
		ImSet_strdata_handler(hndl, cb_strdata_handler)
		ImSet_numdata_handler(hndl, cb_numdata_handler)
		ImSet_intdata_handler(hndl, cb_intdata_handler)
		ImSet_floatdata_handler(hndl, cb_floatdata_handler)
		ImSet_booldata_handler(hndl, cb_booldata_handler)

		'///////////////////////////////////////////////////
		'// Connect to Server
		'///////////////////////////////////////////////////
		If  ImConnect(hndl) < 0 Then
			return
		End If
   

		'///////////////////////////////////////////////////
		'// Auth Device
		'///////////////////////////////////////////////////
		If ImAuth_device(hndl, dev_id, devPw, devGw) < 0 Then
			return
		End If
	

		While true
			'///////////////////////////////////////////////////
			'// Send Sensing data in JSON
			'///////////////////////////////////////////////////
			Dim resource_name As String = "test"
			Dim jdata As String = "{""Temp"":25.5, ""status"":""hello""}"

			'Dim jdata As String = "{
			'"Temp":25.5, "
			'"Hum":50, "
			'"status":"hello", "
			'"TRUE":true, "
			'"FALSE":false, "
			'"DATE":"2020-10-10 10:10:10.100", "
			'"ARR_INT":[0,1,2,3,4], "
			'"ARR_FLT":[0.5,1.5,2.5,3.5,4.5], "
			'"ARR_STR":["aaa", "bbb", "ccc", "ddd"], "
			'"ARR_DATE":["2020-10-11 10:10:10.100", "2020-10-12 10:10:10.100"], "
			'"ARR_BOOL":[true, true, false]"
			'}"


			'//////////////////////////////////////
			'// 센싱데이터 전송 예
			ret = ImSendData(hndl, jdata, resource_name);

			'//////////////////////////////////////
			'// 하위장치, 그룹태그명 지정예제
			'Dim subDevId As String = Nothing;
			'ret = ImSendDataOnDev(hndl, jdata, resource_name, subDevId);

			If ret < 0 Then
		        Console.WriteLine("FAIL send data");
				Exit While
			End If


			ImMSleep(hndl, 1*1000)


			'///////////////////////////////////////////////////
			'// NetIO polling
			'///////////////////////////////////////////////////
			If  ImPoll(hndl) < 0 Then
				Exit While
			End If

		End While


   		'///////////////////////////////////////////////////
		'// Disconnect
		'///////////////////////////////////////////////////
		ImDisconnect(hndl)

   		'///////////////////////////////////////////////////
		'// Release
		'///////////////////////////////////////////////////
		ImRelease(hndl)

   
   End Sub

End Module



