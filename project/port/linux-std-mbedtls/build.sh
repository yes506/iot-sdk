# ./build.sh clean
# or
# ./build.sh


#######################################
# TSL using OpenSSL
#make -f Makefile HAVE_OPENSSL=1 $1

#######################################
# TSL using MbedTLS
make -f Makefile HAVE_MBEDTLS=1 $1

