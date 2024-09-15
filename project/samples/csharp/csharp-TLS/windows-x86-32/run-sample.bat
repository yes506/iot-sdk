set path=c:\Windows\Microsoft.NET\Framework\v4.0.30319\;

del ..\..\hello.exe

msbuild ..\..\hello.csproj -t:Build

..\..\hello.exe  %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%