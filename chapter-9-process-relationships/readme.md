# Process Relationships

## get-pgid-sessionid
1. compile it
```
gcc -o get-pgid-sessionid get-pgid-sessionid.c
```

2. run it
```
./get-pgid-sessionid
```

3. example output
```
parent process's group id: 93890
parent process's session id: 32298

child process's group id: 93890
child process's session id: 32298
after setting new pgid -> pgid: 93891
after setting new session id -> sid: 32298
```
You can see that child's session id have never changed.   
The reason is that child became process group leader of new process group after calling setpgid.   
`setsid` gives error if process is already a process group leader.
   
If you remove setting pgid in this code,
```
// setpgid(0, 0);
```

You can see that new session is created.
```
$child process's group id: 99805
child process's session id: 99291
after setting new pgid -> pgid: 99806
after setting new session id -> sid: 99806
```

