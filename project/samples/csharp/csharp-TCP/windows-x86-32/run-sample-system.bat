set path=c:\Windows\Microsoft.NET\Framework\v4.0.30319\;

del ..\..\IoTMakers3rdPartySystemClientSample.exe

msbuild ..\..\SampleSystemClient.csproj -t:Build

rem ..\..\IoTMakers3rdPartySystemClientSample.exe  %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

rem ..\..\IoTMakers3rdPartySystemClientSample.exe   dev-gw-iotmakers.cloud.kt.com 30024 test12345 test54321 testbyw11

..\..\IoTMakers3rdPartySystemClientSample.exe   dev-gw-iotmakers.cloud.kt.com 30110 M_ROBOT_CHOSUN_THYSSEN 2c01180d2d6d4a7aace1634e26674fe0 CSP_Elevator
