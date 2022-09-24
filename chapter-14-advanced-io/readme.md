# 14.2 Nonblocking I.O
... The slow system calls are those that can block forever. They include,

- Reads that can block the caller forever if data isn't present with certain file types(pipes, terminal devices, and network devices)
- Writes that can block the caller forever if the data can't be accepted immediately by these same file types(e.g., no room in the pipe, network flow control)
- Opens the block until some condition occurs on certain file types (such as an open of a terminal device that waits until an attached modem answers the phone, or an open of a FIFO for writing only, when no other process has the FIFO open for reading)
- Reads and writes of files that have mandatory record locking enabled
- Certain ioctl operations
- Some of the interprocess communications functions

# 14.4 I/O multiplexing
https://slideplayer.com/slide/16886668   
https://notes.shichao.io/unp/ch6   

select function   
ex)   
```
FD_ZERO(&rset);
for(;;) {
	FD_SET(fileno(fp), &rset);
	FD_SET(sockfd, &rset);
	maxfdp1 = max(fileno(fp), sockfd) + 1;
	select(maxfdp1, &rset, NULL, NULL, NULL);
	if (FD_ISSET(sockfd, &rset)) {
		/* socket is readable */
		readline(sockfd, recvline, MAXLINE);
		fputs(recvline, stdout);
	}
	if (FD_ISSET(fileno(fp), &rset)) {
		/* input is readable */
		fgets(sendline, MAXLINE, fp);
		write(sockfd, sendline, strlen(sendline));
	}
}
```


# nonblocking-write.c
O_NONBLOCK flag 동작을 확인하는 예제다.

(1) 컴파일한다.
```
gcc -o noblocking-write nonblocking-write.c
```

(2) noblocking-write를 실행한다.
input 파일 크기는 1kB를 넘어가야 non blocking 동작을 확인할 수 있다.(1KB이면 한 번의 write으로 프로그램이 끝나기 때문)
```
./nonblocking-write < sth.txt 2>stderr3.out
```
sth.txt에 있는 내용이 터미널에 출력되고, 프로그램의 로그는 stderr3.out에 출력한다.



644kB인 input으로 실험했을 때, 다음과 같은 결과가 나왔다.
```
nwrite = 1012, errno = 0
nwrite = 1005, errno = 0
nwrite = 1015, errno = 0
nwrite = 1024, errno = 0
nwrite = -1, errno = 35
nwrite = -1, errno = 35
...
```
12628의write 시스템 콜이 발생했고, 그 중 12136개가 fail했다.   
성공한 write는 최대 1024byte, 최소 928byte를 기록했다.


** errno = 35는 EAGAIN 에러다.
man 2 intro에 보면 `Resource temporarily unavailable.  This is a temporary condition and later calls to the same routine may complete normally.`.  
이라고 나와 있다.

