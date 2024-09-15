set path=c:\Windows\Microsoft.NET\Framework\v4.0.30319\;

del ..\..\IoTMakers3rdPartyDeviceClientSample.exe

msbuild ..\..\SampleDeviceClient.csproj -t:Build

rem ..\..\IoTMakers3rdPartyDeviceClientSample.exe  %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

..\..\IoTMakers3rdPartyDeviceClientSample.exe   dev-gw-iotmakers.cloud.kt.com  30024 testbyw111 12312 KT_3RD_BYW

