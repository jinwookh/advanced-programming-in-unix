# Process Relationships

## 9.2 Terminal Logins
If we log in correctly, `login` process will
- Change to our home directory (chdir) (change current working directory)
- Change the ownership of our terminal device (chown)
- Change athe access permissions for our terminal device so we have permission to read from and write to it
- Set our group IDs by calling setgid and initgroups
- Initialize the environment with all the information that login has: our home directory (HOME), shell (SHELL), user name (USER and LOGNAME), and a default path (PATH)
- Change to our user ID (setuid) and invoke our login shell, as in
```execl("/bin/sh", "-sh", (char *)0);```
...   
Recall from our discussion of the setuid function in Section 8.11 that since it is called by a superuser process, setuid changes all three user IDs: the real user ID, effective userID, and saved set-user-ID. The call to setgid that was done earlier by login has the same effect on all three group IDs. 


## 9.3 Network Logins
In BSD, a single process waits for most network connections: the inetd process, somethimes called the internet superserver. In this section, we'll look at the sequence of processes involved in network logins for a BSD system...   
As part of the system start-up, `init` invokes a shell that executes the shell script /etc/rc. One of the daemon that is started by this shell script is inetd. Once the shell script terminates, the aprent process of inetd becomes init; inetd waits for TCP/IP connection requests to arrive at the host. When a connection request arrvies for it to handle, inetd does a fork and exec of the appropriate program.   
Let's assume that a TCP connection request arrives for the TELNET server. TELNET is a remote login application that uses the TCP protocol. A user on another host(that is connected to the server's host through a network of some form) or on the same host initiates the login by starting the TELNET client:
```
telnet hostname
```

The client opens a TCP connection to hostname, and the program that's started on hostname is called the TELNET server. The client and the server then exchange data across the TCP connection using the TELNET application protocol. What has happened is that the user who started the client program is now logged in to the server's host.(This assumes, of course, that the user has a valid account on the server's host.)   
...   
The telnetd process then opens a pseudo terminal device and splits into two processes using fork. The parent handles the communication across the network connection, and the child does an exec of the login program. The parent and the child are connected through the pseudo terminal. Before doing the exec, the child sets up file descriptors 0 ,1, and 2 to the pseudo terminal. If we log in correctly, login performs the same steps we described earlier: it changes to our home directory and sets our group IDs, user ID, and our initial environment. Then login replaces itself with our login shell by calling exec.
