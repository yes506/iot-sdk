set IM_PROJECT_HOME=..\..\..\..

copy libs\*.dll		%IM_PROJECT_HOME%\samples\c\c-HTTP\windows-x86-64\
copy .\*.dll		%IM_PROJECT_HOME%\samples\c\c-HTTP\windows-x86-64\

copy %IM_PROJECT_HOME%\src\dynamic-library\iotmakers_dll_impl.h  %IM_PROJECT_HOME%\samples\c\c-HTTP\windows-x86-64\

copy .\*.dll		%IM_PROJECT_HOME%\samples\csharp\csharp-HTTP\windows-x86-64\

copy .\*.dll		%IM_PROJECT_HOME%\samples\vbasic\vbasic-HTTP\windows-x86-64\
