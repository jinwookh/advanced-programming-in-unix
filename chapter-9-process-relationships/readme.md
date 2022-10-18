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

At first, parent and child has same process group id, which is parent process id.   
After child set process group id, process group id of child is changed to child's pid.   
Note that session id has not changed even though setsid was called at child process. This is because child process became process group leader after calling setpgid.   
However, have not found why setsid is not permitted to process group leader...
