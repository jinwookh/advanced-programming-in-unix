# Process Control

## 8.2 Process Identifiers
...   
There are some special processes, but the details differ form implementation to implementation.   
 Process ID 0 is usually the scheduler process and is often known as the swapper. No program on disk corresponds to this proces, which is part of the kernel and is known as a system process.   
 Process ID 1 is usually the init process and is invoked by the kernel at the end of the bootstrap procedure. The program file ifor this process was /etc/init in older versions of the UNIX System and is /sbin/init in newer versions. This process is resonseible for bringing up a UNIX system after the kernel has been bootstrapped. `init` usually reads the system-dependent initialization files-the `/etc/rc*` files or `/etc/inittab` and the files in `/etc/init.d`-and brings the system to a certain state, such as multiuser.   
 The init process never dies. It is a normal user process, not a system process within the kernel, like the swapper, although it does run with superuser privileges. Later in this chapter, we'll see how init becomes the parent process of any orphaned child process.
 ...

## 8.3 fork Function(creating child process)
There are two normal cases for handling the descriptors after a fork.
1. The parent waits for the child to complete. In this case, the parent does not need to do anything with its descriptors. When the child terminates, any of the shared descriptors that the child read from or wrote to will have their file offsets updated accordingly.

2. Both the parent and the child go their own ways. Here, after the fork, the parent close the descriptors that it doesn't need, and the child does the same thing. This way, neither interferes with the other's open descriptors. This scenario is often found with network servers.

Besides the open files, numerous other properties of the aprent are inherited by the child:
- Real user ID, real group ID, effective user ID, and effective group ID
- Supplementary group IDs
- Process group ID
- Sessdion ID
- Controlling terminal
- The set-user-ID and set-group-ID flags
- Current working directory
- Root directory
- File mode creation mask
- Signal mask and dispositions
- The close-on-exec flags for any open file descriptors
- Environment
- Attached shared memory segments
- Memory mappings
- Resource limits

The differences between the parent and child are
- The return value from fork are different.
- The process IDs are different.
- The two processes have different parent process IDs: the parent process ID of the child is the parent; the parent process ID of the parent doesn't change.
- The child's tms_utime, tms_Stime, tms_cutime, and tms_cstime values are set to 0.
- File locks set by the parent are not inherited by the child.
- Pending alarams are cleared for the child.
- The set of pending signals for the child is set to the empty set.


## 8.8 wait and waitpid functions
When a process terminates, either normally or abnormally, the kernel notifies the parent by sending the SIGCHLD signal to the parent. Beacuse the termination of a child is an asynchronous event-it can happen at any time while the parent is running-this signal is the asynchronous notification from the kernel to the parent. The parent can choose to ignore this signal, or it can provide a function that is called when the signal occurs: a signal handler. The default for this singal is to be ignored..   
For now, we need to be aware that a process that calls wait or waitpid can
- Block, if all of its children are still running.
- Return immediately with the termination status of a child, if a child has terminated and is waiting for its termination status to be fetched
- Return immediately with an error, if it doesn't have any child processes

If the process is calling wait because it received the SIGCHLD signal, we expect wait to return immediately.   
But if we call it at any random point in time, it can block.
```
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
```
The differences between these two functions are as follows:
- The wait function can block the caller until a child process terminates, whereas waitpid has an option that prevents it from blocking.
- The waitpid function doesn't wait for the child that terminates first; it has a number of options that control which process it waits for.

If a child has already terminated and is a zombie, wait returns immediately with that child's status.   
Otherwise, it blocks the caller until a child terminates. If the caller blocks and has multiple children, wait returns when one terminates. We can always tell which child terminated, because the process ID is returned by the function.


## 8.11 Changing User IDs and Group IDs (related to 4.4 set-user-ID)
Example   
To see the utility of the saved set-user-ID feature, let's examine the operation of a program that uses it. We'll look at the at(1) program, which we can use to schedule commands to be run at some time in the future.
   
To prevent being tricked into running commands that we aren't allowed to run, or reading or writing files that we aren't allowed to access, the `at` command and the daemon that ultimately runs the commadns on our behalf have to switch between sets of privileges: ours and those of the daemon. The following steps take place.

1. Assuming that the `at` program file is owned by root and has its set-user-ID bit set, when we run it, we have
```
real user ID = our user ID(unchanged)
effective user ID = root
saved set-user ID = root
```

2. The first thing the `at` command does is reduce its privileges sot hat it runs with our privileges. It calls the seteuid function to set the effective user ID to our real user ID. After this, we have
```
real user ID = our user ID(unchanged)
effective user ID = our user ID
saved set-user-ID = root(unchanged)
```

3. The `at` program runs with our privileges until it needs to access the configuration files that control which commands are to be run and the time at which they need to run. These files are owned by the daemon that will run the commands for us. The `at` command calls seteuid to set the effective user ID to root. This call is allowed because the argument to setuid equals the saved set-user-ID. (This is why we need the saved set-user-ID.)    
After this, we have   
```
real user ID = our user ID(unchanged)
effective user ID = root
saved set-user-ID = root(unchanged)
```
Because the effective user ID is root, file access is allowed.

4. After the files are modified to record the commadns to be run and the time at which they are to be run, the `at` command lowers its privileges by calling seteuid to set its effective user ID to our user ID. This prevents any accidental misuse of privilege. At this point, we have
```
real user ID = our user ID(unchanged)
effective user ID = our user ID
saved set-user-ID = root(unchanged)
```

5. The daemon starts out running with root privileges. To run commands on our behalf, the daemon calls fork and the child calls setuid to change its user ID to our user ID. Because the child is running with root privileges, this changes all of the IDs. We have
```
real user ID = our user ID
effective user ID = our user ID
saved set-user-ID = our user ID
```
Now the daemon can safely execute commands on our behalf, because it can access only the files to which we normally have access. We have no additional permissions.
