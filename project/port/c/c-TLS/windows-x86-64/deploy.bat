set IM_PROJECT_HOME=..\..\..\..

copy libs\*.dll           %IM_PROJECT_HOME%\samples\c\c-TLS\windows-x86-64\
copy libwinpthread-1.dll  %IM_PROJECT_HOME%\samples\c\c-TLS\windows-x86-64\
copy libeay32.dll	  %IM_PROJECT_HOME%\samples\c\c-TLS\windows-x86-64\
copy ssleay32.dll	  %IM_PROJECT_HOME%\samples\c\c-TLS\windows-x86-64\

copy %IM_PROJECT_HOME%\src\dynamic-library\iotmakers_dll_impl.h  %IM_PROJECT_HOME%\samples\c\c-TLS\windows-x86-64\


copy libs\*.dll           %IM_PROJECT_HOME%\samples\csharp\csharp-TLS\windows-x86-64\
copy libwinpthread-1.dll  %IM_PROJECT_HOME%\samples\csharp\csharp-TLS\windows-x86-64\
copy libeay32.dll	  %IM_PROJECT_HOME%\samples\csharp\csharp-TLS\windows-x86-64\
copy ssleay32.dll	  %IM_PROJECT_HOME%\samples\csharp\csharp-TLS\windows-x86-64\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\vbasic\vbasic-TLS\windows-x86-64\
copy libwinpthread-1.dll  %IM_PROJECT_HOME%\samples\csharp\vbasic-TLS\windows-x86-64\
copy libeay32.dll	  %IM_PROJECT_HOME%\samples\csharp\vbasic-TLS\windows-x86-64\
copy ssleay32.dll	  %IM_PROJECT_HOME%\samples\csharp\vbasic-TLS\windows-x86-64\
