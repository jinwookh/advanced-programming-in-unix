#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define BUFLEN 128
#define TIMEOUT 10

void sigalarm(int signo) {
	printf("alarm has awaken me\n");

}

void print_uptime(int sockfd, struct addrinfo *aip) {
	int n;
	char buf[BUFLEN];

	buf[0] = 0;
	if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0) {
		printf("sendto error\n");
		exit(1);
	}
	alarm(TIMEOUT);
	if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0) {
		if (errno != EINTR)
			alarm(0);
		printf("recv error\n");
		exit(1);
	}
	alarm(0);
	write(STDOUT_FILENO, buf, n);
}

int main(int argc, char *argv[]) {
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;
	struct sigaction sa;
	char *port;

	if (argc != 2) {
		printf("usage: port (host is always localhost)\n");
		exit(1);
	}
	sa.sa_handler = sigalarm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) < 0) {
		printf("sigaction error\n");
		exit(1);
	}
	
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	
	port = argv[1];

	if ((err = getaddrinfo("localhost", port, &hint, &ailist)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		exit(1);
	}

	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
			err = errno;
		} else {
			print_uptime(sockfd, aip);
			exit(0);
		}
	}
	fprintf(stderr, "can't contact %s port: %s\n", argv[1], strerror(err));
	exit(1);
}




