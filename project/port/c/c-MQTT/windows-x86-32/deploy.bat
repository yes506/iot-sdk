set IM_PROJECT_HOME=..\..\..\..

copy libs\*.dll           %IM_PROJECT_HOME%\samples\c\c-MQTT\windows-x86-32\
copy libwinpthread-1.dll  %IM_PROJECT_HOME%\samples\c\c-MQTT\windows-x86-32\

copy %IM_PROJECT_HOME%\src\dynamic-library\iotmakers_dll_impl.h  %IM_PROJECT_HOME%\samples\c\c-MQTT\windows-x86-32\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\csharp\csharp-MQTT\windows-x86-32\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\vbasic\vbasic-MQTT\windows-x86-32\
