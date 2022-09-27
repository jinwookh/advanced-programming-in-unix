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

# 14.5.2 BSD Asynchronous I/O
Asynchronous I/O ion BSD-derived systems is a combination of two signals: SIGIO and SIGURG.   
The former is the general asynchronous I/O signal, and the latter is used only to notify the process that out-of-band data has arrived on a network connection.   
To receive the SIGIO signal, we need to perform three steps.
1. Establsish a signal handler for SIGIO, by calling either signal or sigaction.
2. Set the process ID or process group ID to receive the signal for the descriptor, by calling fcntl with a command of F_SETOWN.
3. Enable asynchronous I/O on the descriptor by calling fnctl with a command of F_SETFL to set the O_ASYNC file status flag.   
Step 3 can be performed only on descriptors that refer to terminals or networks, which is a fundamental limitaiton of the BSD asyncrhonous I/O facility.   
For the SIGURG signal, we need perform only steps 1 and 2. SIGURG is generated only for descriptors that refer to network connections that support out-of-band data, such as TCP connections.


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

# multiplex-write.c
다음 예제를 그대로 구현했다.   
https://engineering.linecorp.com/ko/blog/do-not-block-the-event-loop-part1/

** 
https://github.com/jinwookh/linux-c-example/tree/main/chapter-bonus-tcp-chat
위 파일의 chat_client로multiplex-select 서버를 호출하면, client의 두 번째 발송은 echo되지 않는다. 이유는 모르겠다.   
-> buffer size를1024로 늘리니, client의 두 번째 발송이 echo되지 않는 현상이 사라졌다.   
-> client에서 1024byte만큼 메시지를 보내고 있어, 서버에서도 1024바이트만큼 read할 수 있도록 버퍼 크기를 1024로 수정했다.
