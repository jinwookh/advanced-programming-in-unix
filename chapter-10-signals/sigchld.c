#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

static void sig_cld(int);

int main() {
	pid_t pid;

	if (signal(SIGCHLD, sig_cld) == SIG_ERR) {
		printf("signal error\n");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		printf("fork error\n");
		exit(1);
	}
	else if (pid == 0) {
		sleep(2);
		exit(0);
	}

	pause();
	exit(0);
}

static void sig_cld(int signo) {
	pid_t pid;
	int status;

	printf("SIGCLD received\n");

	if (signal(SIGCHLD, sig_cld) == SIG_ERR) {
		printf("signal error");
		exit(1);
	}
	if ((pid = wait(&status)) < 0) {
		printf("wait error");
		exit(1);
	}
	printf("pid = %d\n", pid);
}
