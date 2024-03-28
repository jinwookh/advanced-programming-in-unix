#include "db.h"
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/uio.h>

#define IDXLEN_SZ 4
#define SEP ':'
#define SPACE ' '
#define NEWLINE '\n'

#define PTR_SZ 7
#define PTR_MAX 9999999
#define NHASH_DEF 137
#define FREE_OFF 0
#define HASH_OFF PTR_SZ

typedef unsigned long DBHASH;
typedef unsigned long COUNT;

typedef struct {
	int idxfd;
	int datfd;
	char *idxbuf;
	char *datbuf;
	char *name;
	off_t idxoff;
	size_t idxlen;
	off_t datoff;
	size_t datlen;
	off_t ptrval;
	off_t ptroff;
	off_t chainoff;
	off_t hashoff;
	DBHASH nhashl
	COUNT cnt_delok;
	COUNT cnt_delerr;
	COUNT cnt_fetchok;
	COUNT cnt_fetcherr;
	COUNT cnt_nextrec;
	COUNT cnt_stor1;
	COUNT cnt_stor2;
	COUNT cnt_stor3;
	COUNT cnt_stor4;
	COUNT cnt_storerr;
} DB;


