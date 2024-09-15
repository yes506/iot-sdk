#####################################################
# Copyright (c) 2018 KT Corp. All rights reserved.
#####################################################

JAVA_HOME = /home/user/local/jdk1.7.0_75_b13-x64
JAVAC	= $(JAVA_HOME)/bin/javac
JAVA	= $(JAVA_HOME)/bin/java
JAR	= $(JAVA_HOME)/bin/jar

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


### Android liblog
OFLAGS += \
	-llog \

LIBS += \

ifeq ($(HAVE_OPENSSL),1)
	CFLAGS += -DHAVE_OPENSSL
	OFLAGS += -lssl -lcrypto
endif

ifeq ($(HAVE_THREAD),1)
	CFLAGS += -DHAVE_THREAD
#	OFLAGS += -lpthread
endif

#####################################################
# JNI + JAR Library (Java Native Interface)
#####################################################
JNI_IMPL_SRC = $(wildcard $(IM_SDK_HOME)/java-jni/*.c)

JNI_IMPL_OBJ = $(addprefix objs/,$(notdir $(JNI_IMPL_SRC:.c=.o)))

VPATH += $(IM_SDK_HOME)/java-jni

CFLAGS += \
	-I$(JAVA_HOME)/include \
	-I$(JAVA_HOME)/include/linux

JAR_SRC = $(wildcard $(IM_SDK_HOME)/java-jni/*.java)

JAR_CLZ = $(addprefix classes/, $(notdir $(JAR_SRC:.java=.class)))

JFLAGS = -g -Xlint

JDPATH = classes

JCPATH = jni-libs


###########
# Android/arm64-v8a/libiotmakers-std-device-jni.so
###########
JNIPATH = jni-libs/Android/arm64-v8a

#####################################################
# Build Rule
#####################################################
all: \
	libiotmakers-std-device-jni.so \
	libiotmakers-std-device-jni.jar \

libiotmakers-std-device-jni.so: $(IM_SDK_OBJS) $(JNI_IMPL_OBJ)
	@mkdir -p $(JNIPATH)
	${CC} -shared -o $(JNIPATH)/$@  $(JNI_IMPL_OBJ)  $(IM_SDK_OBJS) ${OFLAGS} ${LIBS} 

libiotmakers-std-device-jni.jar:
	$(JAR) uvf ../$@  $(JNIPATH)

#####################################################
# Compile Rule
#####################################################
objs/%.o: %.c
	@mkdir -p ${@D}
	${CC} -c -o $@ $< ${CFLAGS}


classes/%.class: %.java
	@mkdir -p ${@D}
	$(JAVAC) $(JFLAGS) -cp $(JCPATH) -d $(JDPATH) $<


#####################################################
# Utils
#####################################################
clean:
	$(RM) ./objs ./bin/* ./jni-libs/* ./classes/*
