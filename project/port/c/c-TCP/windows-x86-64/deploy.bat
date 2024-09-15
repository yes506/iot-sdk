set IM_PROJECT_HOME=..\..\..\..


rem %%% x64 Native Tools Command Prompt for VS 2019
rem 
rem dumpbin.exe /exports libs\libiotmakers-3rdparty-interface.dll > libs\libiotmakers-3rdparty-interface.txt
rem echo LIBRARY libiotmakers-3rdparty-interface > libs\libiotmakers-3rdparty-interface.def
rem echo EXPORTS >> libs\libiotmakers-3rdparty-interface.def
rem 
rem echo ImAuthDevice                                  >> libs\libiotmakers-3rdparty-interface.def
rem echo ImAuthSystem                                  >> libs\libiotmakers-3rdparty-interface.def
rem echo ImConnect                                     >> libs\libiotmakers-3rdparty-interface.def
rem echo ImConnectTo                                   >> libs\libiotmakers-3rdparty-interface.def
rem echo ImDisconnect                                  >> libs\libiotmakers-3rdparty-interface.def
rem echo ImInit                                        >> libs\libiotmakers-3rdparty-interface.def
rem echo ImMSleep                                      >> libs\libiotmakers-3rdparty-interface.def
rem echo ImPoll                                        >> libs\libiotmakers-3rdparty-interface.def
rem echo ImRelease                                     >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceNotificationAppendResource          >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceNotificationInit                    >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceNotificationSend                    >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceRetrieveAppendResource              >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceRetrieveSetResource                 >> libs\libiotmakers-3rdparty-interface.def
rem echo ImResourceSendNotification                    >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnResourceRetrieveAllRequestHandler      >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnResourceRetrieveOneRequestHandler      >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnResourceSetRequestHandler              >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnSysResourceRetrieveAllRequestHandler   >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnSysResourceRetrieveOneRequestHandler   >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetOnSysResourceSetRequestHandler           >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetTlsCaFile                                >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSetTlsCaPath                                >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSysResourceNotificationAppendResource       >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSysResourceNotificationInit                 >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSysResourceNotificationSend                 >> libs\libiotmakers-3rdparty-interface.def
rem echo ImSysResourceSendNotification                 >> libs\libiotmakers-3rdparty-interface.def
rem echo ImTurnCircuitBreakerOff                       >> libs\libiotmakers-3rdparty-interface.def
rem echo ImTurnResponseWaitOff                         >> libs\libiotmakers-3rdparty-interface.def
rem ​
rem lib.exe /def:libs\libiotmakers-3rdparty-interface.def /out:libs\libiotmakers-3rdparty-interface.lib /machine:amd64
rem 

copy libs\*.lib           %IM_PROJECT_HOME%\samples\c\c-TCP\windows-x86-64\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\c\c-TCP\windows-x86-64\
copy libwinpthread-1.dll  %IM_PROJECT_HOME%\samples\c\c-TCP\windows-x86-64\

copy %IM_PROJECT_HOME%\src\dynamic-library\iotmakers_dll_impl.h  %IM_PROJECT_HOME%\samples\c\c-TCP\windows-x86-64\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\csharp\csharp-TCP\windows-x86-64\

copy libs\*.dll           %IM_PROJECT_HOME%\samples\vbasic\vbasic-TCP\windows-x86-64\




