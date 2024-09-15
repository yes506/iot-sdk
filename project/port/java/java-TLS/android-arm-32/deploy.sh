IM_PROJECT_HOME=../../../..

cp jni-libs/Android/armeabi-v7a/*.so ${IM_PROJECT_HOME}/samples/java/java-TLS/android/IoTMakersDeviceSampleApp/app/src/main/jniLibs/armeabi-v7a/
cp ../Prebuilt-OpenSSL-Android/Prebuilt/armv7-shared/lib/*.so ${IM_PROJECT_HOME}/samples/java/java-TLS/android/IoTMakersDeviceSampleApp/app/src/main/jniLibs/armeabi-v7a/

cp ../*.jar ${IM_PROJECT_HOME}/samples/java/java-TLS/android/IoTMakersDeviceSampleApp/app/libs/
