
### JDK 32bit
export JAVA_HOME=~/local/jdk1.7.0_17

### JDK 64bit
export JAVA_HOME=~/local/jdk1.7.0_75_b13-x64



export PATH=$JAVA_HOME/bin:$PATH

javac  -cp ./libiotmakers-std-device-jni.jar -d ./ ../HelloThread.java


#export APPSVR_MEMORY_FLAG=-Xmx1024m -Xms256m -Xss2048k

java  -cp ./libiotmakers-std-device-jni.jar:./  HelloThread  ${EC_IP} ${EC_PORT} ${DEV_ID} ${DEV_PW} ${DEV_GW}

#java  -Xmx1024m -cp ./libiotmakers-std-device-jni.jar:./  HelloThread  ${EC_IP} ${EC_PORT} ${DEV_ID} ${DEV_PW} ${DEV_GW}
