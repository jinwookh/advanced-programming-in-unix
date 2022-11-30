#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

static void sig_int(int);
void print_mask(const char *str) {
        sigset_t sigset;
        int errno_save;

        errno_save = errno;
        if (sigprocmask(0, NULL, &sigset) < 0) {
                printf("sigprocmask error");
                exit(1);
        }
        else {
                printf("%s", str);
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
	sigset_t newmask, oldmask, waitmask;
	print_mask("program start: ");

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		printf("signal(SIGNAL) erro\n");
		exit(1);
	}
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		printf("SIG_BLOCK error\n");
		exit(1);
	}

	print_mask("in critical region: ");

	if (sigsuspend(&waitmask) != -1) {
		printf("sigsuspend error\n");
		exit(1);
	}

	print_mask("after return from sigsuspend: ");

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		printf("SIG_SETMASK error\n");
		exit(1);
	}
	print_mask("program exit: ");
	exit(0);
}

static void sig_int(int signo) {
	print_mask("\n in sig_int: ");
}
