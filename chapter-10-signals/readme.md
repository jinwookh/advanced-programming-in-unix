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

This example is from Advanced Programming in the Unix Environment figure 10.2.

## noreentrant-function.c
1. compile it
```
gcc -o noreentrant-function noreentrant-function.c
```

2. run it
```
./noreentrant-function
```

3. example result
```
getpwnam error
Segmentation fault: 11
```
Most of the results are shown like above.   
However, it is not determined to single result since there is possibility that Segmentation fault would not happen.(which is pretty rare)   


explanation:   
SIGALARM signal is invoked at every 200ms.   
Since getpwnam is nonreentrant function, receiving signal in the middle of getpwnam can be dangerous.   
If getpwnam called free, and the signal was received right after, then signal handler called free, at last data structures maintained by malloc and free can be corrupted.


This example is from Advanced Programming in the Unix Environment figure 10.5.   
At the book, alarm(1) was placed.   
However, alarm(1) was not enought to invoke segmentation fault. It could not show feature of nonreentrant function since nonreentrant function getpwnam ended before 1 second passed.   
So I changed to setitimer function, making alarm goes on every 200ms. This made signal handler intervene through middle of the getpwnam function.

## sigchld.c
1. compile it
```
gcc -o sigchld sigchld.c
```

2. run it
```
./sigchld
```

3. example output
```
SIGCLD received
pid = 47973
```


## sleep1.c sleep2.c
1. compile it
```
gcc -o sleep1 sleep1.c
```

2. run it
```
./sleep1
```

3. example output
```
sleep starts...
sleep done!
```

sleep1, sleep2 both implements sleep function.   
Difference is that sleep2 uses setjmp and longjmp to prevent case of pausing forever.(which was capable in sleep1)


## sleep2_interfere
1. compile it
```
gcc -o sleep2_interfere sleep2_interfere.c
```

2. run it
```
./sleep2_interfere
```

3. example result
```
sleep starting..
^C
sig_int handling starting..
sleep end!
```
sig_int handler never went to end at above example, because sig_alarm handler made through end of the program.

## signal_mask
1. compile it
```
gcc -std=c99 -D_GNU_SOURCE -o signal_mask signal_mask.c
```

2. run it
```
./signal_mask
```

3. example result
```
blocked signal: 
```
Above result means that there is no blocked signal.

## signal-pending
1. compile it
```
gcc -std=c99 -D_GNU_SOURCE -o signal-pending signal-pending.c
```

2. run it
```
./signal-pending
```

3. example result
```
^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
^\Quit: 3
```
Use ctrl+\ to manually send SIGQUIT signal.   
Also note that signal is not queued. It means that no matter how many signal you generate, signal handling function will be executed once for consecutive signals.


## sigaction
1. compile it
```
gcc -o sigaction sigaction.c
```

2. run it
```
./sigaction
```

3. example result
```
received SIGUSR1
received SIGUSR2

```
You need give USR1 or USR2 signal to sigaction process.   
Use `kill -USR1 process_id`,for example.   

This exercise is from `Advanced Programming in the Unix Environment` figure 10.18.
It implements signal function with sigaction function.

## sigsuspend
1. compile it
```
gcc -o sigsuspend sigsuspend.c
```

2. run it
```
./sigsuspend
```

3. example result
```
program start: 
in critical region:  SIGINT
^C
 in sig_int:  SIGINT SIGUSR1
after return from sigsuspend:  SIGINT
program exit: 
```
1. At first, there was no blocked signals at program start because it was default setting.
2. in critical region, SIGINT was added to blocked signal set intentionally.
3. interrupt received.
4. at interrupt handler, SIGINT SIGUSR1 two signals are added in blocked signal set.   
Both are added because SIGUSR1 was added by sigsuspend function by parameter, and SIGINT was added automatically by SIGINT interrupt.
5. after rertun from sigsuspend, signal mask was restored to the part where critical region was going on. SIGUSR1 is gone because sigsuspend function was returned.
6. At program exit, there is no signals in blocked set. This is whole blocked set returned to default intentionally.
