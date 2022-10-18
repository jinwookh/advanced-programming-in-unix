#include <unistd.h>
#include <errno.h>

int main(int argc, char *arv[]) {
	pid_t pid;

	pid = fork();

	// child process
	if (pid == 0) {
	   sleep(1);
	   // prints group id
	   printf("child process's group id: %d\n", getpgrp());
	   // prints session id(0 means calling process itself)
	   printf("child process's session id: %d\n", getsid(0));

	   // sets new process group id of itself.
	   // sets new process group id to its' process id.
	   // setpgid(0, 0);


	   // establishes new session
	   setsid();
	   printf("after setting new pgid -> pgid: %d\n", getpgrp());
	   printf("after setting new session id -> sid: %d\n", getsid(0));

	   exit(0);
	}
	
	// parent process
	printf("parent process's group id: %d\n", getpgrp());
	printf("parent process's session id: %d\n", getsid(0));
	exit(0);
}

