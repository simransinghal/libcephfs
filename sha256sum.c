#define _FILE_OFFSET_BITS 64
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "../src/include/cephfs/libcephfs.h"
#include <unistd.h>

#define MB64 (1<<26)

int main(int argc, const char **argv)
{
	struct ceph_mount_info *cmount;
	int ret, fd, len;
	char buffer[102];
	unsigned char hash[SHA256_DIGEST_LENGTH];

	if (argc < 3) {
		fprintf(stderr, "usage: %s <conf> <file>\n", argv[0]);
		exit(1);
	}

	ret = ceph_create(&cmount, NULL);
	if (ret) {
		fprintf(stderr, "ceph_create=%d\n", ret);
		exit(1);
	}

	ret = ceph_conf_read_file(cmount, argv[1]);
	if (ret) {
		fprintf(stderr, "ceph_conf_read_file=%d\n", ret);
		exit(1);
	}

	ret = ceph_conf_parse_argv(cmount, argc, argv);
	if (ret) {
		fprintf(stderr, "ceph_conf_parse_argv=%d\n", ret);
		exit(1);
	}

	ret = ceph_mount(cmount, NULL);
	if (ret) {
		fprintf(stderr, "ceph_mount=%d\n", ret);
		exit(1);
	}

	ret = ceph_chdir(cmount, "/");
	if (ret) {
		fprintf(stderr, "ceph_chdir=%d\n", ret);
		exit(1);
	}


	fd = ceph_open(cmount, argv[2], O_CREAT|O_TRUNC|O_RDWR, 0777); 
	if (fd < 0) {
		fprintf(stderr, "ceph_open=%d\n", fd);
		exit(1);
	}

        memset(buffer, 'b', sizeof(buffer));

	len = ceph_write(cmount, fd, buffer, sizeof(buffer), 0);
	if (len < 0) {
		fprintf(stderr, "ceph_write=%d\n", len);
		exit(1);
	}

	fprintf(stdout, "wrote %d bytes\n", len);
	
	ret = ceph_close(cmount, fd);
	if(ret){
                fprintf(stderr, "ceph_close=%d\n", ret);
                exit(1);
	}

        fd = ceph_open(cmount, argv[2], O_RDONLY, 0777); 
        if (fd < 0) {
                fprintf(stderr, "ceph_open=%d\n", fd);
                exit(1);
	}

	len = ceph_read(cmount, fd, buffer, sizeof(buffer), 0);
	if (len < 0) {
                fprintf(stderr, "ceph_read=%d\n", len);
                exit(1);
	}

	fprintf(stdout, "read %d bytes\n", len);

        ret = ceph_close(cmount, fd);
        if(ret){
                fprintf(stderr, "ceph_close=%d\n", ret);
                exit(1);
        }

	ceph_shutdown(cmount);

        SHA256_CTX sha256;
        SHA256_Init(&sha256);
	SHA256_Update(&sha256, buffer, len);
	SHA256_Final(hash, &sha256);
	
	for (len = 0; len < SHA256_DIGEST_LENGTH; ++len)
		printf("%02x", hash[len]);

	putchar('\n');	

	return 0;
}
