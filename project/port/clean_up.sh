
find ./ -name *.o    -exec rm {} \;
find ./ -name *.a    -exec rm {} \;

find ./ -name *jni.dll  -exec rm {} \;
find ./ -name *device.dll  -exec rm {} \;

find ./ -name *jni.so  -exec rm {} \;
find ./ -name *device.so  -exec rm {} \;

find ./ -name *.class  -exec rm {} \;
#find ./ -name *.jar  -exec rm {} \;

find ../samples -name core  -exec rm {} \;
find ../samples -name *.log  -exec rm {} \;
find ../samples -name *.class  -exec rm {} \;
find ../samples -name hello.exe  -exec rm {} \;
find ../samples -name hello  -exec rm {} \;

