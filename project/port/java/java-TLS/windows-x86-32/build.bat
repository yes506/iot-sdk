set MINGW_PATH="z:\SanDISK\home\local\Windows\mingw32\"

set "PATH=%MINGW_PATH%\bin"

rem MS WIndows 32 bit
rem build.bat OR build.bat clean

mkdir objs
mkdir jni-libs
mkdir classes
mingw32-make -f Makefile.MinGW32 HAVE_OPENSSL=1 HAVE_THREAD=1 %1

