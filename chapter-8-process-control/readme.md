# Process Control

## fork-exercise
1. compile it
```
gcc -o fork-exercise fork-exercise.c
```

2. run it
```
./wait_exercise
```

3. You will see pretty much like this if output is printed in terminal.
```
a write to standard out
before fork
pid = 4128, glob = 7, var = 89
pid = 4127, glob = 6, var = 88
```

However, if you redirect output to some file with the same execution, you will see
```
a write to standard out
before fork
pid = 4022, glob = 7, var = 89
before fork
pid = 4021, glob = 6, var = 88
```
Why 'before fork' is printed twice?   
   
The book(Advanced Programming in the Unix Environment) says,   
"When we run the programming interactively, we get only a single copy of the first printf line,  because the standard output buffer is flushed by the newline. When we redirect standard output to a file, however, we get two copies of the printf line.   
In this second case, the printf before the fork is called once, but the line remains in the buffer when fork is called.   
The buffer is then copied into the child when the parent's data space is copied to the child. 
Both the parent and the child now have a standard I/O buffer with this line in it.   
The second printf, right before the exit, just appends its data to the existing buffer. When each process terminates, its copy of the buffer is finally flushed."
   
Then probably there are separate buffer for each process?   
After fork, child copies unflushed buffer to it's memory. So let's not forget flushing it before fork.


## wait-exercise
1. compile it
```
gcc -o wait-exercise wait-exercise.c
```

2. run it
```
./wait_exercise
```

3. message printed
```
child normal termination!!
child abonormal termination!! signal number = 6
child abonormal termination!! signal number = 8
```
- first child is terminated with normal one because it exited with exit(7).
- second child is considered as abonormal because it used abort().
- third child is considered as abnormal because it generated SIGFPE.(devide by 0 signal)

