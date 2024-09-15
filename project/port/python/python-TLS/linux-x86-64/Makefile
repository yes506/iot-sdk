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
IM_PROJECT_HOME = ../../../..
IM_SDK_HOME = $(IM_PROJECT_HOME)/src
include $(IM_SDK_HOME)/Makefile.common

CFLAGS += \
	-fPIC \

OFLAGS += \

LIBS += \

ifeq ($(HAVE_OPENSSL),1)
	CFLAGS += -DHAVE_OPENSSL
	OFLAGS += -lssl -lcrypto
#	OFLAGS += -lssl -lcrypto
#	OFLAGS += -l:libssl.so.1.1 -l:libcrypto.so.1.1
endif

ifeq ($(HAVE_THREAD),1)
	CFLAGS += -DHAVE_THREAD
	OFLAGS += -lpthread
endif



############################################
## usage: $ make PY3=1
############################################

ifeq ($(PY3),1)

	CFLAGS += -D__PYTHON_3__

	PYTHON_INC=-I../../python_src/Python-3.7.0-Linux-x86-64/Include \
			-I../../python_src/Python-3.7.0-Linux-x86-64

	PYTHON_LIB=\
		-L../../python_src/Python-3.7.0-Linux-x86-64 \
		-lpython3
	
	PYTHON_PRODUCT = IotmakersStdDevicePy3.so

else ifeq ($(PY2),1)

	CFLAGS += -D__PYTHON_2__

	PYTHON_INC=-I../../python_src/Python-2.7.18-Linux-x86-64/Include \
			-I../../python_src/Python-2.7.18-Linux-x86-64

	PYTHON_LIB=\
		-L../../python_src/Python-2.7.18-Linux-x86-64 \
		-lpython2

	
	PYTHON_PRODUCT = IotmakersStdDevicePy2.so

endif 


CFLAGS += $(PYTHON_INC)

LIBS += $(PYTHON_LIB)




#####################################################
# Dynamic Library (so)
#####################################################
PYTHON_WRAP_SRC = $(wildcard $(IM_SDK_HOME)/python-pni/*.c)
PYTHON_WRAP_OBJ = $(addprefix objs/,$(notdir $(PYTHON_WRAP_SRC:.c=.o)))
VPATH += $(IM_SDK_HOME)/python-pni


#####################################################
# Build Rule
#####################################################
all: \
	$(PYTHON_PRODUCT)
#	libiotmakers-std-device.so \


$(PYTHON_PRODUCT): $(IM_SDK_OBJS) $(PYTHON_WRAP_OBJ)
	@mkdir -p ./libs
	${CC} -shared -o ./libs/$@ $(PYTHON_WRAP_OBJ)  $(IM_SDK_OBJS) ${OFLAGS} ${LIBS} 


#####################################################
# Compile Rule
#####################################################
objs/%.o: %.c
	@mkdir -p ${@D}
	${CC} -c -o $@ $< ${CFLAGS}

clean:
	$(RM) ./objs ./bin/* ./libs/*

touch:
	$(RM) ./objs/iotmakers_pni.o
