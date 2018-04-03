#include <stdio.h>
#include <openssl/sha.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int sha256_file(char *path)
{
	FILE *file = fopen(path, "w");
	if(!file) return -534;

	char buf[1024];
	memset(buf, 'a', sizeof(buf));

	int results = fputs(buf, file);
	if (results == EOF) return -534;

	fclose(file);

	file = fopen(path, "rb");
	if(!file) return -534;

	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);

	int bytesRead = 0;
	while((bytesRead = fread(buf, 1, sizeof(buf), file)))
	{
		printf("bytesRead is %d\n",bytesRead);
		SHA256_Update(&sha256, buf, bytesRead);
	}
	SHA256_Final(hash, &sha256);

	for (int len = 0; len < SHA256_DIGEST_LENGTH; ++len)
		printf("%02x", hash[len]);
	
	putchar('\n');

	fclose(file);
	return 0;
}
int main(int argc, char **argv)
{
	sha256_file(argv[1]);
	return 0;
}
