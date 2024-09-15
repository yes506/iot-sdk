build_in_dir() {
	echo "PUSHD $1"
	pushd $1
	./build.sh clean
	./build.sh
	./deploy.sh
	./build.sh clean
	popd
}


build_in_dir "c/c-HTTP/rasberrypi-32"
build_in_dir "c/c-MQTT/rasberrypi-32"
build_in_dir "c/c-TCP/rasberrypi-32"
build_in_dir "c/c-TLS/rasberrypi-32"

build_in_dir "java/java-HTTP/rasberrypi-32"
build_in_dir "java/java-MQTT/rasberrypi-32"
build_in_dir "java/java-TCP/rasberrypi-32"
build_in_dir "java/java-TLS/rasberrypi-32"

