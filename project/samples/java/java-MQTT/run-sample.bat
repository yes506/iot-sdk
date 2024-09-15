# 32bit 
set JAVA_HOME=z:\SanDISK\home\local\Windows\java-se-7u75-ri-x86

# 64bit 
set JAVA_HOME=z:\SanDISK\home\local\Windows\openjdk-1.8.0.265-1.b01.windows.x86_64


set "PATH=%JAVA_HOME%\bin"

javac  -cp ./libiotmakers-std-device-jni.jar -d ./  ../Hello.java

java   -classpath "./libiotmakers-std-device-jni.jar;./"   Hello   %EC_IP% %EC_PORT% %DEV_ID% %DEV_PW% %DEV_GW%
