

export NDK=~/local/android-sdk-linux64/ndk/20.0.5594570
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

# Only choose one of these, depending on your device...
export TARGET=aarch64-linux-android
#export TARGET=armv7a-linux-androideabi
#export TARGET=i686-linux-android
#export TARGET=x86_64-linux-android

export API=21

make \
    CC=$TOOLCHAIN/bin/$TARGET$API-clang \
    AR=$TOOLCHAIN/bin/$TARGET-ar \
    RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib






