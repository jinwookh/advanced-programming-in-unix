#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void print_family(struct addrinfo *aip) {
	printf("family: ");
	switch (aip -> ai_family) {
	case AF_INET:
		printf("inet\n");
		break;
	case AF_INET6:
		printf("inet6\n");
		break;
	case AF_UNIX:
		printf("unix\n");
		break;
	case AF_UNSPEC:
		printf("unspecified\n");
	default:
		printf("unknown\n");
	}
}

void print_type(struct addrinfo *aip) {
	printf("type: ");
	switch (aip->ai_socktype) {
	case SOCK_STREAM:
		printf("stream\n");
		break;
	case SOCK_DGRAM:
		printf("datagram\n");
		break;
	case SOCK_SEQPACKET:
		printf("seqpacket\n");
		break;
	case SOCK_RAW:
		printf("raw\n");
		break;
	default:
		printf("unknown (%d)\n",  aip->ai_socktype);
	}
}

void print_protocol(struct addrinfo *aip) {
	printf("protocol: ");
	switch (aip->ai_protocol) {
	case 0:
		printf("default\n");
		break;
	case IPPROTO_TCP:
		printf("TCP\n");
		break;
	case IPPROTO_UDP:
		printf("UDP\n");
		break;
	case IPPROTO_RAW:
		printf("raw\n");
		break;
	default:
		printf("unknown (%d)\n", aip->ai_protocol);
	}
}

void print_flags(struct addrinfo *aip) {
	printf("flags: ");
	if (aip->ai_flags == 0) {
		printf(" 0\n");
	} else {
		if (aip->ai_flags & AI_PASSIVE)
			printf("passive");
		if (aip->ai_flags & AI_CANONNAME)
			printf(" canon");
		if (aip->ai_flags & AI_NUMERICHOST)
			printf(" numhost");
		if (aip->ai_flags & AI_NUMERICSERV)
			printf(" numserv");
		if (aip->ai_flags & AI_V4MAPPED)
			printf(" v4mapped");
		if (aip->ai_flags & AI_ALL)
			printf(" all");
	}
}

int main(int argc, char *argv[]) {
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	fflush(stdout);
	

	if (argc != 3){
		printf("usage: %s nodename service", argv[0]);
		exit(1);
	}
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = 0;
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	
	if ((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0) {
		printf("getaddrinfo error: %s", gai_strerror(err));
		exit(1);
	}

	for (aip = ailist; aip != NULL; aip = aip->ai_next) {	
		printf("host: %s\n", aip->ai_canonname? aip->ai_canonname : "-");
		if (aip->ai_family == AF_INET) {
			sinp = (struct sockaddr_in *) aip ->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
			printf("address: %s\n", addr? addr: "unknown");
			printf("port: %d\n", ntohs(sinp->sin_port));
		}
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n");
	}
	exit(0);
}





