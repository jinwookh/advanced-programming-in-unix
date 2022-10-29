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

## getpgrp-tcgetsid
1. compile it
```
gcc -o getpgrp-tcgetsid getpgrp-tcgetsid.c
```

2. run it
```
./getpgrp-tcgetsid
```

3. example result
```
process's group id: 8364
process's session id: 99291
get foreground process group of fd 0: 8364
get foreground process group of fd 1: 8364
get foreground process group of fd 2: 8364
get process group id of session leader of fd 0: 99291
get process group id of session leader of fd 1: 99291
get process group id of session leader of fd 2: 99291
```

tcgetpgrp returns foreground process group associated with the terminal open on fd.   
tcgetsid funciton allows an application to obtain the process group ID for the session leader given a file descriptor for the controlling TTY.


## orphaned-process-group
1. compile it
```
gcc -o orphaned-process-group orphaned-process-group.c
```

2. run it
```
./orphaned-process-group
```

3. example result
```
parent: pid = 39111, ppid = 23720, pgroupid = 39111, foreground pgroupid = 39111, session id= 23719
child: pid = 39112, ppid = 39111, pgroupid = 39111, foreground pgroupid = 39111, session id= 23719
SIGHUP received, pid = 39112
child: pid = 39112, ppid = 1, pgroupid = 39111, foreground pgroupid = 23720, session id= 23719
read error 5 on controlling TTY
```
read error 5 gives error because calling process is orphaned.   
see man 2 read for error code 5.

definition of orphaned process group in the book:   
parent of every member is either itself a member of the group or is not a member of the group's session.   
Another way of saying this si that the process group is not orphaned as long as a process in the group has a parent in a differen process group but in the same ssion.   

definition of orphaned process group in gnu.org:   
process groups that continue running even after the session leader has terminated are marked as orphaned process group.   
   
Reading source code can help.   
see will_become_orphaned_pgrp method in exit.c:   
https://github.com/torvalds/linux/blob/1440f576022887004f719883acb094e7e0dd4944/kernel/exit.c

Note that exit.c checks whether there is stopped jobs when sending SIGHUP signal torphaned process group.
Maybe that's why SIGHUP is not sent to child process if kill(getpid(), SIGTSTP) code is removed from orphaned-process-group.c


