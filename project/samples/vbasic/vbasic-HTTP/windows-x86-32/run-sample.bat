set path=c:\Windows\Microsoft.NET\Framework\v4.0.30319\;

set path=c:\Windows\Microsoft.NET\Framework\v3.5\;

del ..\..\hello.exe

msbuild ..\..\hello.vbproj -t:Build

..\..\hello.exe  %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

