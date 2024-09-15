
export LD_LIBRARY_PATH=./

gcc  -o hello ../../hello.c  -I./  -L./ -liotmakers-std-device -m32

./hello  ${EC_IP} ${EC_PORT} ${DEV_ID} ${DEV_PW} ${DEV_GW}

