#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_hup(int signo) {
	printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void print_ids(char *name) {
	printf("%s: pid = %ld, ppid = %ld, pgroupid = %ld, foreground pgroupid = %ld, session id= %ld\n", name, (long) getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO), (long)getsid(STDIN_FILENO));
}


int main(void) {
	char c;
	pid_t pid;
	print_ids("parent");

	if ((pid = fork()) < 0 ) {
		printf("fork error\n");
	} else if (pid > 0) {
		sleep(5);
	} else {
		print_ids("child");
		signal(SIGHUP, sig_hup);
		kill(getpid(), SIGTSTP);
		print_ids("child");
		if (read(STDIN_FILENO, &c, 1) != 1)
			printf("read error %d on controlling TTY\n", errno);
	}
	exit(0);
}

