#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void clr_fl(int fd, int flags);
void set_fl(int fd, int flags);

char buf[500000];

int main(void) {
	int ntowrite, nwrite;
	char *ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK); /* set nonblocking */
	
	ptr = buf;
	while (ntowrite > 0) {
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0 ) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK); /* clear nonblocking */
	exit(0);
}

void clr_fl(int fd, int flags) {
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		printf("fcntl F_GETFL error");

	val &= ~flags; /* turn flags off */

	if (fcntl(fd, F_SETFL, val) < 0)
		printf("fcntl F_SETFL error");

}

void set_fl(int fd, int flags) {
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		printf("fcntl F_GETFL error");
	val |= flags;

	if (fcntl(fd, F_SETFL, val) < 0)
		printf("fcntl F_SETFL error");

}
