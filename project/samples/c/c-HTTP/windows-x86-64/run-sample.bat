set MINGW_PATH=z:\SanDISK\home\local\Windows\mingw64\

set "PATH=%MINGW_PATH%\bin"

gcc.exe ..\..\hello.c  -o hello.exe  -I.  -L. -liotmakers-std-device

.\hello   %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%

