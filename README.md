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

My testfile is `helloworld.c`:

	$ sudo cat /mnt/mycephfs/helloworld.c
	
	#include <stdio.h>
	int main()
	{
		printf("Hello, World!");
		return 0;
	}

Compile sha256sum.c file:
	
	/build$ gcc sha256sum.c -lcephfs -lssl -lcrypto -L./lib
	
Execute sha256sum.c:

	build$ ./a.out ceph.conf helloworld.c
	e42c89335961c5b03eb86eb1e1bbb96f62a38f791f215af17112229f28bb968a
	
As mentioned in project discription page, this code is written using this is as a reference:
https://github.com/ceph/ceph/blob/d7692a24c74b5e7b6d08eadce03e6b6efa344d61/qa/libceph/trivial_libceph.c
