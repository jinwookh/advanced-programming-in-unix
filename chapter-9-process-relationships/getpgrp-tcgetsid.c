#include <unistd.h>
#include <errno.h>

int main(int argc, char *arv[]) {

	printf("process's group id: %d\n", getpgrp());
	printf("process's session id: %d\n", getsid(0));

	printf("get foreground process group of fd 0: %d\n", tcgetpgrp(0));
	printf("get foreground process group of fd 1: %d\n", tcgetpgrp(1));
	printf("get foreground process group of fd 2: %d\n", tcgetpgrp(2));

	printf("get process group id of session leader of fd 0: %d\n", tcgetsid(0));	
	printf("get process group id of session leader of fd 1: %d\n", tcgetsid(1));
	printf("get process group id of session leader of fd 2: %d\n", tcgetsid(2));
	exit(0);
}

