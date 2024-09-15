
build_in_dir() {
	echo "PUSHD $1"
	pushd $1
	
	make clean
	./build-py2.sh
	./deploy.sh

	make clean
	./build-py3.sh
	./deploy.sh

	make clean

	popd
}


build_in_dir "./python-TCP/linux-x86-64"
build_in_dir "./python-TLS/linux-x86-64"

