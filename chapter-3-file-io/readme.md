# 3.5 close function
..When a process terminates, all of its open files are closed automatically by the kernel.    
Many programs take advantage of this fact and don't explicitly close open files.

# 3.6 lseek
lseek only records the current file offset within file offset within the kernel - it does not cause any I/O to take place.

# 3.10 File Sharing
![image](https://user-images.githubusercontent.com/31182783/187805023-3a889990-0534-4efe-8ef4-928f378fbf31.png)

- Process table entry is per process table.
- File table entry can be shared by multiple processes.(ex: file table is shared by both child and parent processes)
- There are only one file descriptor flag: FD_CLOEXEC
- There are many file status flags: ex) O_RDONLY, O_WRONLY, O_RDWR, O_EXEC, O_SEARCH,, O_APPEND, O_NONBLOCK, O_SYNC, O_DSYNC…

- After each write is complete, the current file offset in the file table entry is incremented by the number of bytes written. If this causes the current file offset to exceed the current file size, the current file size in the i-node table entry is set to the current file offset(for example, the file is extended).
- If a file is opend with the O_APPEND flag, a corresponding flag is set in the file status flags of the file table entry. Each time a write is performed for a file with this append flag set, the current file offset in the file table entry is first set to the current file size from the i-node table entry. This forces every write to be appended to the current end of file.
- If a file is positioned to its current end of file using lseek, all that happens is the current file offset in the file table entry is set to the current file size from the i-node table entry.
- The lseek function modifies only the current file offset in the file table entry. No I/O takes place.
