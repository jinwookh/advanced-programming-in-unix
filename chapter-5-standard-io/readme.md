# 5.4 Buffering
The goal of the buffering provided by the standard I/O library is to use the minimum number of read and write calls.   
Also, this library tries to do its buffering automatically for each I/O stream, obviating the need for the application to worry about it.    
Unfortunately, the single aspect of the standard I/O library that generates the most confusion is its buffering.   

Three types of buffering are provided:

1. Fully buffered. In this case, actual I/O takes place when the standard I/O buffer is filled. Files residing on disk are normally fully buffered by the standard I/O library. The buffer used is usually obtained by one of the standard I/O functions calling malloc the first time I/O is performed on a stream.

   The term flush describes the writing of a standard I/O buffer. A buffer can be flushed automatically by the standard I/O routines, such as when a buffer fills, or we can call the function fflush to flush a stream.    
Unfortunately, in the UNIX environment, flush means two different things. In terms of the standard I/O library, it means writing out the contents of a buffer, which may be partially filled. In terms of the terminal driver, such as the tcflush function in Chapter 18, it means to discard the data that's already stored in a buffer.
   
2. Line buffered. In this case, the standard I/O library performs I/O when a newline character is encountered on input or output. This allows us to output a single character at a time(with the standard I/O fputc function), knowing that actual I/O will take place only when we finish writing each line. Line buffering is typically used on a stream when it referes to a terminal-standard input and standard output, for example.

   Line buffering comes with two caveats. First, the size of the buffer that the standard I/O library uses to collect each line is fixed, so I/O might take place if we fill this buffer before writing a newline. Second, whenever input is requested through the standard I/O library from either (a) an unbuffered stream or (b) a line-buffered stream (that requires data to be requested from the kernel), all line-buffered output streams are flushed. The reason for the qualifier on (b) is that the requested data may already be in the buffer, which doesn't require data to be read from the kernel. Obviously, any input from an unbuffered stream, item(a), requires data to be obtained from the kernel.

3. Unbuffered. The standard I/O library does not buffer the characters. If we write 15 characters with the standard I/O fputs function, for example. we expect these 15 characters to be output as soon as possible, probably with the write function from Section 3.8.

   The standard error stream, for example , is normally unbuffered so that any error messages are displayed as quickly as possible, regardless of whether they contain a newline.


# fmemopen exercise
Advance Programming in the Unix Environment 172p    
fmemopen 예제의 목적은 flush가 어떻게 동작하는지 확인하는 것이다.
fflush를 사용하지 않으면 buffer에는 내용이 들어가면 안 된다.
그리고 fseek, fclose를 사용하면 자동으로 flush가 된다.

하지만 Mac에서 직접 코드를 실행해보니, flush를 사용하지 않더라도 buffer에서 내용을 확인할 수 있었다.

# print_buffer exercisei
Advanced Programming in the Unix Environment 164p   
print_buffer의 목적은 stdout, stdin, stderr에 할당하는 버퍼의 크기를 확인하는 것이다.
   

출력결과:
```
enter any character
asd
one line to standard error
stream = stdin, line buffered, buffer size =  4096
stream = stdout, line buffered, buffer size =  4096
stream = stderr, unbuffered, buffer size =  1
stream = /etc/passwd, fully buffered, buffer size =  4096
```
책에서는 stdin, stdout의 경우 버퍼 사이즈가 1024라고 나오지만, Mac에서 코드를 실행하니 버퍼 크기가 4096이다.   
Mac에서의 버퍼 구현체는 책 예제에서 사용한 운영체제와 다른 것 같다.

# second_caveat exercise
second_caveat.c 예제는input stream을 받을 때 output stream이 자동으로 flush되는지 확인하기 위해 작성했다.(Advanced Programming in Unix Environment 145p)

프로그램을 실행하면 다음과 같이 출력한다.
```
one line going on..
 here's the second one.
```
핵심은 here's the second one이 언제 출력되는가 인데, 실행하면 input을 받기 직전에 출력된다. 즉, input i/o 요청은 output stream이 자동으로 flush되게 한다.

one line going on과 here's the second one 사이에 `\n`이 있기 때문에, fputs로 해당 문자열을 출력할 경우 one line going on만 출력된다. here's the second one은 버퍼에 남아 있다.

here's the second one은 input io가 요청되거나, 프로그램이 종료될 때 flush되면서 출력된다.

