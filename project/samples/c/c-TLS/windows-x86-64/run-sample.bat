set MINGW_PATH=z:\SanDISK\home\local\Windows\mingw64\

set "PATH=%MINGW_PATH%\bin"

rem gcc.exe ..\..\hello.c  -o hello.exe  -I.  -L. -liotmakers-std-device -lssl -lcrypto

gcc.exe ..\..\hello.c  -o hello.exe  -I.  -L. -liotmakers-3rdparty-interface -lssl-1_1-x64 -lcrypto-1_1-x64


rem .\hello   %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

.\hello  dev-gw-iotmakers.cloud.kt.com  30105  testbyw111 12312 KT_3RD_BYW
