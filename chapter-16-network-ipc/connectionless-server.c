#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define MAXADDRLEN 256
#define BUFLEN 128
#define QLEN 10

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
	int n;
	socklen_t alen;

	FILE *fp;
	char buf[BUFLEN];
	char abuf[MAXADDRLEN];
	struct sockaddr *addr = (struct sockaddr *) abuf;
	printf("ready to get into for loop..\n");
	for (;;) {
		alen = MAXADDRLEN;
		if ((n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0) {
			printf("ruptimed: recvfrom error: %s", strerror(errno));
			exit(1);
		}
		printf("got request from client..\n");

		if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
			printf("error: %s\n", strerror(errno));
			sendto(sockfd, buf, strlen(buf), 0, addr, alen);
		} else {
			if (fgets(buf, BUFLEN, fp) != NULL)
				sendto(sockfd, buf, strlen(buf), 0, addr, alen);
			/**
				Since UDP packets can be unordered, and also there can be an packet loss, we try to make single response in single packet.
			**/
			pclose(fp);
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

	if ((sockfd = initserver(SOCK_DGRAM, (struct sockaddr*) &serv_addr, sizeof(serv_addr), QLEN)) >= 0) {
		printf("server initiated. ready to response.\n");
		server(sockfd);
		exit(0);
	}
	exit(1);
}
