#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>


static void my_alarm(int signo) {
	struct passwd *rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL) {
		printf("getpwnam(root) error\n");
		exit(1);
	}

}

int main(void) {
	struct passwd *ptr;
	struct itimerval value;	
	signal(SIGALRM, my_alarm);

	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 100000; // inital alarm at 100ms later

	value.it_interval.tv_sec = 0; 
    	value.it_interval.tv_usec = 200000; // set alarm for every 200ms

	setitimer(ITIMER_REAL, &value, NULL);

	for ( ; ; ) {
		if ((ptr = getpwnam("sar")) == NULL) {
			printf("getpwnam error\n");
		}
		if (strcmp(ptr->pw_name, "sar") != 0) {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
		}
	}
}
		
