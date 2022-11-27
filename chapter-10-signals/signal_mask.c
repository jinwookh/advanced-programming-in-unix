#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)
#define _POSIX_SOURCE

void print_mask() {
	sigset_t sigset;
	int errno_save;

	errno_save = errno;
	if (sigprocmask(0, NULL, &sigset) < 0) {
		printf("sigprocmask error");
		exit(1);
	}
	else {
		printf("blocked signal: ");
		if (sigismember(&sigset, SIGINT))
			printf(" SIGINT");
		if (sigismember(&sigset, SIGQUIT))
			printf(" SIGQUIT");
		if (sigismember(&sigset, SIGUSR1))
			printf(" SIGUSR1");
		if (sigismember(&sigset, SIGALRM))
			printf(" SIGALRM");

		printf("\n");
	}
	errno = errno_save;
}

int main(void) {
	print_mask();
}

