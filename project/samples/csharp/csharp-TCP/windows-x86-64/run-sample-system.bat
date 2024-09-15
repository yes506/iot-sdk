set path=c:\Windows\Microsoft.NET\Framework64\v4.0.30319\;

del ..\..\IoTMakers3rdPartySystemClientSample.exe

msbuild ..\..\SampleSystemClient.csproj -t:Build

rem ..\..\IoTMakers3rdPartySystemClientSample.exe  %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

..\..\IoTMakers3rdPartySystemClientSample.exe   dev-gw-iotmakers.cloud.kt.com 30024 test12345 test54321 testbyw11

