set MINGW_PATH=z:\SanDISK\home\local\Windows\mingw64\

set "PATH=%MINGW_PATH%\bin"

gcc.exe .\IoTMakers3rdPartySystemClientSample.c  -o hello.exe  -I.  -L. -liotmakers-3rdparty-interface

rem .\hello   %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

.\hello  dev-gw-iotmakers.cloud.kt.com  30024 _testbyw111 12312 KT_3RD_BYW

