# File Types
The file types are   
1. Regular file. The most common type of file, which contains data of some form. There is no distinction to the UNIX kernel whether this data is text or binary. Any interpretation of the contents of a regular file is left to the application processing the file.   
ex) /etc/passwd

2. Directory file. A file that contains the names of other files and pointers to information on these files. Any process that has read permission for a directory file can read the contents of the directory, but only the kernel can write directly to a directory file. Processes must use the functions described in this chapter to make changes to a directory.   
ex) /etc

3. Block special file. A type of file providing buffered I/O access in fixed-size units to devices such as disk drives.   
ex) /dev/sr0


4. Character special file. A type of file providing unbuffered I/O access   
ex) /dev/tty

5. FIFO. A type of file used for communication between processes. It's sometimes called a named pipe.   
ex) /var/lib/oprofile/opd_pipe

6. Socket. A type of file used for network communication between processes. A socket can also be used for non-network communication between processes on a single host.   
ex) /dev/log

7. Symbolic link. A type of file that points to another file.   
ex) /dev/cdrom


# 4.4 Set-User-ID and Set-Group-ID
When we execute a program file, the effective user ID is usually the real user ID, and the effective group ID is usually the real group ID. However, we can also set a special flag in the file's mode word (st_mode) that says, "When this file is executed, set the effective user ID of the proces to be owner of the file (st_uid)." Similarly, we can set another bit in the file's mode word that causes the effective group ID to be the group owner of the file (st_gid). These two bits in the file's mode word are called the set-user-ID bit and the set-group-ID bit.   
For example, if the owner of the file is the superuser and if the file's set-user-ID bit is set, then while that program file is running as a process, it has superuser privileges. This happens regardless of the real user ID of the process that executes the file. As an example, the uNIX System that allows anyone to change his or her password, passwd(1), is a set-user-ID program. This is required so that the program can write the new password to the password file, typically either /etc/psswd or /etc/shadow, files that should be writable only by the superuser. Because a process that is running set-user-ID to some other user usually assumes extra permissions, it must be written carefully..

# file.hole
```
gcc -o create_hole_file create_hole_file.c
./create_hole_file
```
You can see file.hole created with 16KB size.
Only contents inside of it is abcdefghij and ABCDEFJI, also between them is filled with \0.


