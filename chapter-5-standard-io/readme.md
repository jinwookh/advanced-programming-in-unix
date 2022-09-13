# fmemopen exercise
fmemopen 예제의 목적은 flush가 어떻게 동작하는지 확인하는 것이다.
fflush를 사용하지 않으면 buffer에는 내용이 들어가면 안 된다.
그리고 fseek, fclose를 사용하면 자동으로 flush가 된다.

하지만 Mac에서 직접 코드를 실행해보니, flush를 사용하지 않더라도 buffer에 내용을 확인할 수 있었다.
