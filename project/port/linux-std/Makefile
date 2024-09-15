#####################################################
# Copyright (c) 2018 KT Corp. All rights reserved.
#####################################################

#####################################################
# Tool Chain
#####################################################
CC	= cc
AR	= ar rvs
RM	= /bin/rm -rf


#####################################################
# IoTMakers Device SDK Source Home
#####################################################
IM_PROJECT_HOME = ../..
IM_SDK_HOME = $(IM_PROJECT_HOME)/src
include $(IM_SDK_HOME)/Makefile.common

CFLAGS += \
	-fPIC \

OFLAGS += \

LIBS += \

ifeq ($(HAVE_OPENSSL),1)
	CFLAGS += -DHAVE_OPENSSL
	OFLAGS += -lssl -lcrypto
endif

ifeq ($(HAVE_MBEDTLS),1)
	MBEDTLS_DEV = /home/user/project/opensource/mbedtls/mbedtls-development/build
	CFLAGS += -DHAVE_MBEDTLS -I$(MBEDTLS_DEV)/include
	OFLAGS += \
		$(MBEDTLS_DEV)/library/libmbedx509.a \
		$(MBEDTLS_DEV)/library/libmbedtls.a \
		$(MBEDTLS_DEV)/library/libmbedcrypto.a
endif

ifeq ($(HAVE_THREAD),1)
	CFLAGS += -DHAVE_THREAD
	OFLAGS += -lpthread
endif



#####################################################
# Dynamic Library (so)
#####################################################
DLL_IMPL_SRC = $(wildcard $(IM_SDK_HOME)/dynamic-library/*.c)
DLL_IMPL_OBJ = $(addprefix objs/,$(notdir $(DLL_IMPL_SRC:.c=.o)))
VPATH += $(IM_SDK_HOME)/dynamic-library


#####################################################
# Sample Apps
#####################################################
SAMPLE_APP_SRC = $(IM_PROJECT_HOME)/samples/simple_app/simple_app_main.c
SAMPLE_SIMPLE_APP_OBJ = $(addprefix objs/,$(notdir $(SAMPLE_APP_SRC:.c=.o)))
VPATH += $(IM_PROJECT_HOME)/samples/simple_app

SYSTEM_CLI_SRC = $(IM_PROJECT_HOME)/samples/simple_app/system_client_main.c
SYSTEM_CLI_APP_OBJ = $(addprefix objs/,$(notdir $(SYSTEM_CLI_SRC:.c=.o)))
VPATH += $(IM_PROJECT_HOME)/samples/simple_app



#####################################################
# Build Rule
#####################################################

OUT_DYNAMIC_LIB = libiotmakers-3rdparty-interface.so

ifeq ($(HAVE_MBEDTLS),1)
	OUT_DYNAMIC_LIB = 
endif

all: \
	libiotmakers-3rdparty-interface.a \
	$(OUT_DYNAMIC_LIB) \
	simple_app \
	system_cli \
#	biz_app \
#	test_app \


libiotmakers-3rdparty-interface.a: $(IM_SDK_OBJS)
	@mkdir -p ./libs
	$(AR) ./libs/$@ $(IM_SDK_OBJS)

libiotmakers-3rdparty-interface.so: $(IM_SDK_OBJS) $(DLL_IMPL_OBJ)
	${CC} -shared -o ./libs/$@ $(DLL_IMPL_OBJ)  $(IM_SDK_OBJS) ${OFLAGS} ${LIBS} 

simple_app: $(IM_SDK_OBJS) $(SAMPLE_SIMPLE_APP_OBJ)
	@mkdir -p ./bin
	${CC} -o ./bin/$@ $(SAMPLE_SIMPLE_APP_OBJ) $(IM_SDK_OBJS) ${OFLAGS} ${LIBS}

system_cli: $(IM_SDK_OBJS) $(SYSTEM_CLI_APP_OBJ)
	@mkdir -p ./bin
	${CC} -o ./bin/$@ $(SYSTEM_CLI_APP_OBJ) $(IM_SDK_OBJS) ${OFLAGS} ${LIBS}


#####################################################
# Compile Rule
#####################################################
objs/%.o: %.c
	@mkdir -p ${@D}
	${CC} -c -o $@ $< ${CFLAGS}

clean:
	$(RM) ./objs ./bin/* ./libs/*
