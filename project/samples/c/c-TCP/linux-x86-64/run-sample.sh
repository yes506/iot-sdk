
export LD_LIBRARY_PATH=./

gcc  -o hello ../../hello.c  -I./  -L./ -liotmakers-std-device

#./hello  ${EC_IP} ${EC_PORT} ${DEV_ID} ${DEV_PW} ${DEV_GW}

./hello  dev-gw-iotmakers.cloud.kt.com 30024 testbyw111 12312 KT_3RD_BYW
