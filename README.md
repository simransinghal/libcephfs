# libcephfs

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

	sudo make vstart

Start up a ceph cluster:

	sudo MON=3 OSD=1 MDS=1 MGR=1 RGW=1 ../src/vstart.sh -n -d

Mount the Ceph file system:

	sudo mkdir /mnt/mycephfs
	sudo mount -t ceph 10.2.128.199:40999:/ /mnt/mycephfs -o name=admin,secret=AQC8lb5acMj/GBAAmgEiAKA3VbZ6daQooGUpSA==
	
Output of `$ mount`

	......
	......
	......
	10.2.128.199:40999:/ on /mnt/mycephfs type ceph (rw,relatime,name=admin,secret=<hidden>,acl,wsize=16777216)

My testfile is `helloworld.c`:

	$ sudo cat /mnt/mycephfs/helloworld.c
	
	#include <stdio.h>
	int main()
	{
		printf("Hello, World!");
		return 0;
	}


