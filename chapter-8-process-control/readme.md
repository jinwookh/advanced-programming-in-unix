# Process Control

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

