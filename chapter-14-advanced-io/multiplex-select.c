#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 500
void error_handling(char *buf);

int main(int argc, char *argv[]) {
	int serv_sock, client_sock;
	struct sockaddr_in serv_addr, client_addr;
	struct timeval timeout;
	// 파일 상태 테이블 선언	
	fd_set reads, cpy_reads;

	socklen_t addr_size;
	int fd_max, str_len, fd_num, i, str_wrote_len;
	char buf[BUF_SIZE];

	printf("before doing argument check!\n");
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	printf("argument check! done.\n");

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	printf("bind success!!\n");

	FD_ZERO(&reads); // fd_set 테이블을 초기화한다.
	FD_SET(serv_sock, &reads); // 서버 소켓(리스팅 소켓)의 이벤트 검사를 위해 서버 소캣 1개를 fd_Set 테이블에 추가한다.
	fd_max = serv_sock;

	while(1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;

		if (fd_num == 0)
			continue;

		for (i=0; i<fd_max+1; i++) {
			if (FD_ISSET(i, &cpy_reads)) { // fd_set 테이블을 검사한다.
				if (i == serv_sock) { // 서버 소켓(리스닝 소켓)에 이벤트(연결 요청) 발생
					addr_size = sizeof(client_addr);
					client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &addr_size);
					FD_SET(client_sock, &reads);// fd_set 테이블에 클라이언트 소켓 디스크립터 1개를 추가한다.
					if (fd_max < client_sock)
						fd_max = client_sock;
					printf("connected client: %d \n", client_sock);
				}

				else { // 클라이언트와 연결된 소켓에 이벤트 발생
					str_len = read(i, buf, BUF_SIZE);
					printf("read contents: %s, from %d\n", buf, i);
					if (str_len == 0) {
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					} 
					else {
						str_wrote_len = write(i, buf, str_len);
						printf("wrote contents to %d, size: %d\n", i, str_wrote_len);
					}
				}
			}
		}
	}
	
	close(serv_sock);
	return 0;
}

void error_handling(char *buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}










