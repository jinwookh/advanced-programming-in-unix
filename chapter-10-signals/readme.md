# Signals

## 10.1 Introduction
Signals are software interrupts. Most nontrivial application programs need to deal with signals. Signals provide a way of handling asynchronous events-for example, a user at a terminal typing the interrupt key to stop a program or the next program in a pipeline terminating prematurely.   
...


## signal-exercise
1. compile it
```
gcc -o signal-exercise signal-exercise.c
```

2. run it
```
./signal-exercise
```

3. run command kill to signal-exercise process at other window
example:   
```
kill -USR1 33333
kill -USR2 33333
```

4. At window of signal-exercise, you will see:   
```
received SIGUSR1
received SIGUSR2
```

