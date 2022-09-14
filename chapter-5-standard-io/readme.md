# fmemopen exercise
fmemopen 예제의 목적은 flush가 어떻게 동작하는지 확인하는 것이다.
fflush를 사용하지 않으면 buffer에는 내용이 들어가면 안 된다.
그리고 fseek, fclose를 사용하면 자동으로 flush가 된다.

하지만 Mac에서 직접 코드를 실행해보니, flush를 사용하지 않더라도 buffer에서 내용을 확인할 수 있었다.

# print_buffer exercise
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


