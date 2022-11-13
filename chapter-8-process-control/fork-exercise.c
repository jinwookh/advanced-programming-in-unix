#include <stdio.h>
#include <unistd.h>

int globalvar =6;
char buf[] = "a write to standard out\n";

int main (void) {
	int var;
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf) - 1) {
		printf("write error\n");
		exit(1);
	}
 	printf("before fork\n");

	if ((pid = fork()) < 0) {
		printf("fork error\n");
		exit(1);
	} else if (pid == 0) {
		globalvar ++;
		var++;
	} else {
		sleep(2);
	}

	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globalvar, var);
	exit(0);
}



