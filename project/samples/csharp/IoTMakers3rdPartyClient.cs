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
using System.Runtime.InteropServices;

public class IoTMakers3rdPartyClient
{
	public const string IMDllName = "libiotmakers-3rdparty-interface.dll";

	public const int IM_RESP_CODE_2000_OK				=		2000;
	public const int IM_RESP_CODE_2004_Changed			=		2004;
	public const int IM_RESP_CODE_4000_BadRequest		=		4000;
	public const int IM_RESP_CODE_4004_NotFound			=		4004;
	public const int IM_RESP_CODE_5000_InternalError	=		5000;

	[DllImport(IMDllName)]
		public static extern int ImInit(int log_level);
	[DllImport(IMDllName)]
		public static extern int ImRelease(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImConnectTo(int hndl, string ec_ip, int ec_port);
	[DllImport(IMDllName)]
		public static extern int ImConnect(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImTurnCircuitBreakerOff(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImTurnResponseWaitOff(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImDisconnect(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImAuthSystem(int hndl, string _chn_id, string _chn_pw, string _sys_id);
	[DllImport(IMDllName)]
		public static extern int ImAuthDevice(int hndl, string _dev_id, string _dev_pw, string _svc_gw);
	[DllImport(IMDllName)]
		public static extern int ImPoll(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImMSleep(int hndl, int msec);

	///////////////////////////////////
	// Device Interface APIs
	[DllImport(IMDllName)]
		public static extern int ImResourceSendNotification(int hndl, string resource_id, string properties_in_json);
	[DllImport(IMDllName)]
		public static extern int ImResourceNotificationInit(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImResourceNotificationAppendResource(int hndl, string resource_id, string properties_in_json);
	[DllImport(IMDllName)]
		public static extern int ImResourceNotificationSend(int hndl);

	[DllImport(IMDllName)]
		public static extern void ImSetOnResourceSetRequestHandler(int hndl, IMOnResourceSetRequestDelegate func);
	[DllImport(IMDllName)]
		public static extern void ImSetOnResourceRetrieveOneRequestHandler(int hndl, IMOnResourceRetrieveOneRequestDelegate func);
	[DllImport(IMDllName)]
		public static extern void ImSetOnResourceRetrieveAllRequestHandler(int hndl, IMOnResourceRetrieveAllRequestDelegate func);

	///////////////////////////////////
	// System Interface APIs
	[DllImport(IMDllName)]
		public static extern int ImSysResourceSendNotification(int hndl, string resource_id, string properties_in_json);
	[DllImport(IMDllName)]
		public static extern int ImSysResourceNotificationInit(int hndl);
	[DllImport(IMDllName)]
		public static extern int ImSysResourceNotificationAppendResource(int hndl, string resource_id, string properties_in_json);
	[DllImport(IMDllName)]
		public static extern int ImSysResourceNotificationSend(int hndl);

	[DllImport(IMDllName)]
		public static extern void ImSetOnSysResourceSetRequestHandler(int hndl, IMOnSysResourceSetRequestDelegate func);
	[DllImport(IMDllName)]
		public static extern void ImSetOnSysResourceRetrieveOneRequestHandler(int hndl, IMOnSysResourceRetrieveOneRequestDelegate func);
	[DllImport(IMDllName)]
		public static extern void ImSetOnSysResourceRetrieveAllRequestHandler(int hndl, IMOnSysResourceRetrieveAllRequestDelegate func);
	//
	///////////////////////////////////////

	[DllImport(IMDllName)]
		public static extern int ImResourceRetrieveSetResource(IntPtr pktBody, string resource_id, string properties_in_json);
	[DllImport(IMDllName)]
		public static extern int ImResourceRetrieveAppendResource(IntPtr pktBody, string resource_id, string properties_in_json);


	///////////////////////////////////////////////////////////////////////
	// Device Control Handler Definition
	///////////////////////////////////////////////////////////////////////
	public delegate int IMOnResourceSetRequestDelegate(IntPtr pktBody, string dev_id, string resource_id, string properties_in_jstr);
	public delegate int IMOnResourceRetrieveOneRequestDelegate(IntPtr pktBody, string dev_id, string resource_id);
	public delegate int IMOnResourceRetrieveAllRequestDelegate(IntPtr pktBody, string dev_id);
	
	///////////////////////////////////////////////////////////////////////
	// System Control Handler Definition
	///////////////////////////////////////////////////////////////////////
	public delegate int IMOnSysResourceSetRequestDelegate(IntPtr pktBody, string sys_id, string resource_id, string properties_in_jstr);
	public delegate int IMOnSysResourceRetrieveOneRequestDelegate(IntPtr pktBody, string sys_id, string resource_id);
	public delegate int IMOnSysResourceRetrieveAllRequestDelegate(IntPtr pktBody, string sys_id);



	// This Client Handler
	private int hndl = -1;

    public IoTMakers3rdPartyClient()
    {
        this.hndl = -1;
    }

    public int Init(int logLevel)
    {
		this.hndl = ImInit(logLevel);
		return this.hndl;
    }

	public int Release()
	{
		return ImRelease(this.hndl);
	}

	public int ConnectTo(string serverIp, int serverPort)
	{
		return ImConnectTo(this.hndl, serverIp, serverPort);
	}

	public int Disconnect()
	{
		return ImDisconnect(this.hndl);
	}

	public int TurnCircuitBreakerOff()
	{
		return ImTurnCircuitBreakerOff(this.hndl);
	}

	public int TurnResponseWaitOff()
	{
		return ImTurnResponseWaitOff(this.hndl);
	}

	public int AuthDevice(string devId, string devPw, string devGw)
	{
		return ImAuthDevice(this.hndl, devId, devPw, devGw);
	}

	public int AuthSystem(string chnId, string chnPw, string sysId)
	{
		return ImAuthSystem(this.hndl, chnId, chnPw, sysId);
	}

	
	public int Poll()
	{
		return ImPoll(this.hndl);
	}

	public int MSleep(int msec)
	{
		return ImMSleep(this.hndl, msec);
	}

	///////////////////////////////////////////////////
	// Device API
	public int ResourceSendNotification(string resource_id, string properties_in_json)
	{
		return ImResourceSendNotification(this.hndl, resource_id, properties_in_json);
	}
	
	public int ResourceNotificationInit()
	{
		return ImResourceNotificationInit(this.hndl);
	}

	public int ResourceNotificationAppendResource(string resource_id, string properties_in_json)
	{
		return ImResourceNotificationAppendResource(this.hndl, resource_id, properties_in_json);
	}

	public int ResourceNotificationSend()
	{
		return ImResourceNotificationSend(this.hndl);
	}

	public void SetControlCallBackHandler(IMOnResourceSetRequestDelegate OnResourceSetRequestHandler, 
											IMOnResourceRetrieveOneRequestDelegate OnResourceRetrieveOneRequestHandler,
											IMOnResourceRetrieveAllRequestDelegate OnResourceRetrieveAllRequestHandler)
	{   
		ImSetOnResourceSetRequestHandler(this.hndl, OnResourceSetRequestHandler);
		ImSetOnResourceRetrieveOneRequestHandler(this.hndl, OnResourceRetrieveOneRequestHandler);
		ImSetOnResourceRetrieveAllRequestHandler(this.hndl, OnResourceRetrieveAllRequestHandler);
	}


	///////////////////////////////////////////////////
	// System API
	public int SysResourceSendNotification(string resource_id, string properties_in_json)
	{
		return ImSysResourceSendNotification(this.hndl, resource_id, properties_in_json);
	}
	
	public int SysResourceNotificationInit()
	{
		return ImSysResourceNotificationInit(this.hndl);
	}

	public int SysResourceNotificationAppendResource(string resource_id, string properties_in_json)
	{
		return ImSysResourceNotificationAppendResource(this.hndl, resource_id, properties_in_json);
	}

	public int SysResourceNotificationSend()
	{
		return ImSysResourceNotificationSend(this.hndl);
	}

	public void SysSetControlCallBackHandler(IMOnSysResourceSetRequestDelegate OnResourceSetRequestHandler, 
												IMOnSysResourceRetrieveOneRequestDelegate OnResourceRetrieveOneRequestHandler,
												IMOnSysResourceRetrieveAllRequestDelegate OnResourceRetrieveAllRequestHandler)
	{
		ImSetOnSysResourceSetRequestHandler(this.hndl, OnResourceSetRequestHandler);
		ImSetOnSysResourceRetrieveOneRequestHandler(this.hndl, OnResourceRetrieveOneRequestHandler);
		ImSetOnSysResourceRetrieveAllRequestHandler(this.hndl, OnResourceRetrieveAllRequestHandler);
	}
	//
	///////////////////////////////////////////////////////////////////////////////////

	public int ResourceRetrieveSetResource(IntPtr pktBody, string resource_id, string properties_in_json)
	{
		return ImResourceRetrieveSetResource(pktBody, resource_id, properties_in_json);
	}

	public int ResourceRetrieveAppendResource(IntPtr pktBody, string resource_id, string properties_in_json)
	{
		return ImResourceRetrieveAppendResource(pktBody, resource_id, properties_in_json);
	}                                                                                                                                                                                                                                                                                                     
}
