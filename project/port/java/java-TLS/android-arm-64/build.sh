

export NDK=~/local/android-sdk-linux64/ndk/21.0.6113669

export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

# Only choose one of these, depending on your device...
export TARGET=aarch64-linux-android
#export TARGET=armv7a-linux-androideabi

export API=21

rm -rf ./objs/*

make \
    CC=$TOOLCHAIN/bin/$TARGET$API-clang \
    AR=$TOOLCHAIN/bin/$TARGET-ar \
    RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib \
    HAVE_OPENSSL=1 HAVE_THREAD=1 

