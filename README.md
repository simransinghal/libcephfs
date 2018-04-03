# libcephfs

This program computes the sha256sum of a file in CephFS

----------------------------------------------------------

Clone the Ceph source code from here:

	https://github.com/ceph/ceph

Check out the "mimic" branch:

	git checkout -b mimic

Ceph contains many git submodules that need to be checked out with:

	git submodule update --init --recursive

Install the necessary dependencies by running:

	./install-deps.sh

Run cmake:

	./do_cmake.sh

cd into the newly created "build" directory:

	cd build
	
Run:

	make vstart

Start up a ceph cluster:

	MON=3 OSD=1 MDS=1 MGR=1 RGW=1 ../src/vstart.sh -n -d

export:

	export LD_LIBRARY_PATH=./lib

Compile sha256sum.c file:
	
	/build$ gcc sha256sum.c -lcephfs -lssl -lcrypto -L./lib
	
Execute sha256sum.c:

	build$ ./a.out ceph.conf <file>

Output:

	wrote 1024 bytes
	read 1024 bytes
	2edc986847e209b4016e141a6dc8716d3207350f416969382d431539bf292e4a

For verifying the sha256sum, I have written a simple programme verify_sha256sum.c:

	gcc verify_sha256sum.c -lssl -lcrypto
	./a.out <some_file>

Output:

	bytesRead is 1024
	2edc986847e209b4016e141a6dc8716d3207350f416969382d431539bf292e4a

And as verify_sha256sum.c has written in file <some_file>, this can be verified using:

	$ sha256sum <some-file>
	2edc986847e209b4016e141a6dc8716d3207350f416969382d431539bf292e4a  <some_file> 
		
As mentioned in project discription page, this code is written using this as a reference:
https://github.com/ceph/ceph/blob/d7692a24c74b5e7b6d08eadce03e6b6efa344d61/qa/libceph/trivial_libceph.c
