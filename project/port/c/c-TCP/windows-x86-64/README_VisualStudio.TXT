

dumpbin.exe /exports libiotmakers-3rdparty-interface.dll > libiotmakers-3rdparty-interface.def

lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib 

lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib /machine:x86



/*

사용시.. 아래와 같은 에러를 왕창 얻을 수 있다..

Linker Tools Warning LNK4017

.\test\opencv_core2413d.def(1481) : warning LNK4017: 대상 플랫폼에는 29C000 문이 지원되지 않습니다. 무시됩니다.

keyword statement not supported for the target platform; ignored

​
이유가 무엇인가 하니,


우리가 lib를 만들 때 필요한 def 파일은 위에 있는 함수 name 만 필요하기 때문이다!!

★ 1. def 파일로 변환한다.

.\dumpbin.exe /exports (dll이름).dll > (저장할 파일 이름).txt

ex) dumpbin.exe /exports libiotmakers-3rdparty-interface.dll > libiotmakers-3rdparty-interface.txt

​

echo LIBRARY (라이브러리 이름) > (저장할 def 파일 이름).def

ex) echo LIBRARY libiotmakers-3rdparty-interface > libiotmakers-3rdparty-interface.def

​echo EXPORTS >> (저장한 def 파일 이름).def

ex) echo EXPORTS >> libiotmakers-3rdparty-interface.def
​

for /f "skip=19 tokens=4" %A in ((저장했던 파일 이름).txt) do echo %A >> (저장한 def 파일 이름).def

ex) for /f "skip=19 tokens=4" %A in libiotmakers-3rdparty-interface.txt do echo %A >> libiotmakers-3rdparty-interface.def

​

★ 2. lib 파일을 만든다.

..\lib.exe /def:(저장한 def 파일 이름).def /out:(라이브러리 이름).lib /machine:(계열)

ex) ..\lib.exe /def:opencv.def /out:opencv_core2413d.lib /machine:x86
lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib /machine:amd64
lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib /machine:x86

이렇게 하면 이상없이 lib 파일이 제대로 만들어진다.!!




dumpbin.exe /EXPORTS /OUT:libiotmakers-3rdparty-interface.def libiotmakers-3rdparty-interface.dll

lib.exe /def:libiotmakers-3rdparty-interface.def /machine:amd64

lib.exe /def:libiotmakers-3rdparty-interface.def /machine:x86



dumpbin.exe /exports libiotmakers-3rdparty-interface.dll > libiotmakers-3rdparty-interface.def

lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib 

lib.exe /def:libiotmakers-3rdparty-interface.def /out:libiotmakers-3rdparty-interface.lib /machine:x86




cl test.c 

cl test.c /link libiotmakers-3rdparty-interface.a 


cl test.c /link libiotmakers-3rdparty-interface.dll

cl test.c /link libiotmakers-3rdparty-interface.lib


심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
오류	LNK2019	"int __cdecl ImRelease(int)" (?ImRelease@@YAHH@Z)main 함수에서 참조되는 확인할 수 없는 외부 기호	ConsoleApplication1	C:\Users\demo\source\repos\ConsoleApplication1\ConsoleApplication1.obj	1	


cl test.c /flags /link /subsystem:console


*/

