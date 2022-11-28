#include <signal.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

static void sig_usr(int signo) {
        if (signo == SIGUSR1)
                printf("received SIGUSR1\n");
        else if (signo == SIGUSR2)
                printf("received SIGUSR2\n");
        else {
                printf("received a signal %d\n", signo);
                exit(1);
        }
}


void * signal_custom(int signo, void * func) {
	struct sigaction act, old_act;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo != SIGALRM) {
		act.sa_flags |= SA_RESTART;
	}
	if (sigaction(signo, &act, &old_act) < 0)
		return SIG_ERR;
	return old_act.sa_handler;
}

int main (void) {
        if (signal_custom(SIGUSR1, sig_usr) == SIG_ERR) {
                printf("can't catch SIGUSR1");
                exit(1);
        }
        if (signal_custom(SIGUSR2, sig_usr) == SIG_ERR) {
                printf("can't catch SIGUSR2");
                exit(1);
        }
        for ( ; ; )
                pause();
}


