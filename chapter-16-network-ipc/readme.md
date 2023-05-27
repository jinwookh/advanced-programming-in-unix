# Network IPC

## getaddrinfo

1. compile it
```
gcc -o getaddrinfo getaddrinfo.c
```

2. run it
```
./getaddrinfo localhost 80
```

3. see the result
```
host: localhost
flags:  0
family: inet6
type: datagram
protocol: UDP

host: localhost
flags:  0
family: inet6
type: stream
protocol: TCP

host: localhost
address: 127.0.0.1
port: 80
flags:  0
family: inet
type: datagram
protocol: UDP

host: localhost
address: 127.0.0.1
port: 80
flags:  0
family: inet
type: stream
protocol: TCP
```
