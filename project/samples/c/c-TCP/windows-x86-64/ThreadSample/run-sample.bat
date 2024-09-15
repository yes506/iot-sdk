set MINGW_PATH=z:\SanDISK\home\local\Windows\mingw64\

set "PATH=%MINGW_PATH%\bin"

gcc.exe .\IoTMakers3rdPartySystemClientSampleThread.c  -o hello.exe  -I.  -L. -liotmakers-3rdparty-interface

.\hello
