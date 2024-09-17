

build_in_dir() {
	echo "PUSHD $1"
	pushd $1
	./build-py3.sh clean
	./build-py3.sh
	./deploy.sh
	./build.sh clean
	popd
}


# build_in_dir "c/c-HTTP/linux-std"
# build_in_dir "c/c-HTTP/linux-x86-32"
# build_in_dir "c/c-HTTP/linux-x86-64"
# build_in_dir "c/c-HTTP/linux-x86-ubc222-yocto"

# build_in_dir "c/c-MQTT/linux-std"
# build_in_dir "c/c-MQTT/linux-x86-32"
# build_in_dir "c/c-MQTT/linux-x86-64"
# build_in_dir "c/c-MQTT/linux-x86-ubc222-yocto"

# build_in_dir "c/c-TCP/linux-std"
# build_in_dir "c/c-TCP/linux-x86-32"
# build_in_dir "c/c-TCP/linux-x86-64"
# build_in_dir "c/c-TCP/linux-x86-ubc222-yocto"

# build_in_dir "c/c-TLS/linux-std"
# build_in_dir "c/c-TLS/linux-x86-32"
# build_in_dir "c/c-TLS/linux-x86-64"
# build_in_dir "c/c-TLS/linux-x86-ubc222-yocto"




# build_in_dir "java/java-TCP/android-arm-32"
# build_in_dir "java/java-TCP/android-arm-64"


# build_in_dir "java/java-TCP/linux-x86-32"
# build_in_dir "java/java-TCP/linux-x86-64"


# build_in_dir "java/java-HTTP/linux-x86-32"
# build_in_dir "java/java-HTTP/linux-x86-64"


# build_in_dir "java/java-MQTT/linux-x86-32"
# build_in_dir "java/java-MQTT/linux-x86-64"


# build_in_dir "java/java-TLS/linux-x86-32"
# build_in_dir "java/java-TLS/linux-x86-64"


build_in_dir "python/python-TCP/linux-x86-64"
