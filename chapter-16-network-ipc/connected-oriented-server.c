#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define QLEN 10
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen) {
	int fd;
	int err = 0;
	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return(-1);
	if (bind(fd, addr, alen) < 0)
		goto errout;
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0)
			goto errout;
	}
	return(fd);

errout:
	err = errno;
	close(fd);
	errno = err;
	return(-1);
}

void server(int sockfd) {
	int childfd, status;
	pid_t pid;

	for (;;) {
		if ((childfd = accept(sockfd, NULL, NULL)) < 0) {
			printf("ruptimed: accept error: %s\n", strerror(errno));
			exit(1);
		}
		if ((pid = fork()) < 0) {
			printf("ruptimed: fork error: %s\n", strerror(errno));
			exit(1);
		}
		else if (pid == 0) {
			if (dup2(childfd, STDOUT_FILENO) != STDOUT_FILENO || dup2(childfd, STDERR_FILENO) != STDERR_FILENO) {
				printf("ruptimed: error with dup2\n");
				exit(1);
			}

			close(childfd);
			execl("/usr/bin/uptime", "uptime", (char *)0);
			printf("ruptimed: unexpected return from exec: %s", strerror(errno));
		}
		else {
			close(childfd);
			waitpid(pid, &status, 0);
		}
	}
}

int main(int argc, char *argv[]) {
	struct sockaddr_in serv_addr;
	int sockfd;

	if (argc != 1) {
		printf("usage:ruptimed");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(atoi("1234"));	

	if ((sockfd = initserver(SOCK_STREAM, (struct sockaddr*) &serv_addr, sizeof(serv_addr), QLEN)) >= 0) {
		server(sockfd);
		exit(0);
	}
	exit(1);
}
