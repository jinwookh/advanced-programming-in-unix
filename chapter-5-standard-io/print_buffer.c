#include <stdio.h>
#include <stdlib.h>

void print_stdio(const char*, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);


int main(void) {
	FILE *fp;

	fputs("enter any character\n", stdout);
	if (getchar() == EOF)
		printf("getchar error!");
	fputs("one line to standard error\n", stderr);
	/** 
	   use getchar, fputs with stderr to make io on stdin and stderr.
	   For buffer to be allocated, io needs to be done at least once.
	**/


	print_stdio("stdin", stdin);
	print_stdio("stdout", stdout);
	print_stdio("stderr", stderr);

	if ((fp = fopen("/etc/passwd", "r")) == NULL)
		printf("fopen error!");
	if (getc(fp) == EOF)
		printf("getc error!");
	print_stdio("/etc/passwd",fp);
	exit(0);
}

void print_stdio(const char *name, FILE *fp) {
	printf("stream = %s, ", name);
	if (is_unbuffered(fp))
		printf("unbuffered");
	else if (is_linebuffered(fp))
		printf("line buffered");
	else
		printf("fully buffered");
	printf(", buffer size =  %d\n", buffer_size(fp));
}


int is_unbuffered(FILE *fp) {
	return fp->_flags & __SNBF;
}

int is_linebuffered(FILE *fp) {
	return fp->_flags & __SLBF;
}

int buffer_size(FILE *fp) {
	return fp->_bf._size;
}











