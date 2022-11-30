#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t quitflag;

static void sig_int(int signo) {
	if (signo == SIGINT)
		printf("\ninterrupt\n");
	else if (signo == SIGQUIT)
		quitflag = 1;
}

int main(void) {
	sigset_t oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		printf("signal(SIGINT) error\n");
		exit(1);
	}

	if (signal(SIGQUIT, sig_int) == SIG_ERR) {
		printf("signal(SIGQUIT) error\n");
		exit(1);
	}

	sigemptyset(&zeromask);

	while (quitflag == 0)
		sigsuspend(&zeromask);

	exit(0);

}
