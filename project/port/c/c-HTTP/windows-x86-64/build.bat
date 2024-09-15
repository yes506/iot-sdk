set MINGW_PATH=z:\SanDISK\home\local\Windows\mingw64\

set "PATH=%MINGW_PATH%\bin"

rem MS WIndows 64 bit

rem build.bat OR build.bat clean 


mkdir objs
mkdir libs
mkdir bin

mingw32-make -f Makefile.MinGW64 %1

