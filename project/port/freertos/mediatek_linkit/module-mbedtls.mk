#module path
IM_SDK_HOME = middleware/third_party/iotmakers_device_sdk_4.3.1/project

#source file
C_FILES += $(IM_SDK_HOME)/src/util/util.c

C_FILES += $(IM_SDK_HOME)/src/deps/parson/parson.c

C_FILES += $(IM_SDK_HOME)/src/deps/netio/fd_util.c
C_FILES += $(IM_SDK_HOME)/src/deps/netio/netio.c
C_FILES += $(IM_SDK_HOME)/src/deps/netio/tcp_io.c
C_FILES += $(IM_SDK_HOME)/src/deps/netio/tls_io.c

C_FILES += $(IM_SDK_HOME)/src/packet/head/head_all_init.c
C_FILES += $(IM_SDK_HOME)/src/packet/head/head.c
C_FILES += $(IM_SDK_HOME)/src/packet/head/head_ext.c
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_121_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_121_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_224_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_224_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_231_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_231_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_332_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_332_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_411_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_411_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_525_rep.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_525_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_525_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_711_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_711_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_731_req.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_731_res.c 
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_all_init.c
C_FILES += $(IM_SDK_HOME)/src/packet/body/body.c         
C_FILES += $(IM_SDK_HOME)/src/packet/body/body_util.c    
C_FILES += $(IM_SDK_HOME)/src/packet/body/json_stub.c    
C_FILES += $(IM_SDK_HOME)/src/packet/body/json_wrap.c    
C_FILES += $(IM_SDK_HOME)/src/packet/packet_all_init.c   
C_FILES += $(IM_SDK_HOME)/src/packet/packet.c            

C_FILES += $(IM_SDK_HOME)/src/client/packet_io.c            
C_FILES += $(IM_SDK_HOME)/src/client/simple_api.c            
C_FILES += $(IM_SDK_HOME)/src/client/client.c            


#include path
CFLAGS += -DFREERTOS -DHAVE_MBEDTLS
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/iotmakers_device_sdk_4.3.1/project/src
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/iotmakers_device_sdk_4.3.1/project/src/deps

# mbedtls read this
CFLAGS += -DSUPPORT_MBEDTLS -DMBEDTLS_MTK
