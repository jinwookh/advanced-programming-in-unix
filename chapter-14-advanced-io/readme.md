# 14.2 Nonblocking I.O
... The slow system calls are those that can block forever. They include,

- Reads that can block the caller forever if data isn't present with certain file types(pipes, terminal devices, and network devices)
- Writes that can block the caller forever if the data can't be accepted immediately by these same file types(e.g., no room in the pipe, network flow control)
- Opens the block until some condition occurs on certain file types (such as an open of a terminal device that waits until an attached modem answers the phone, or an open of a FIFO for writing only, when no other process has the FIFO open for reading)
- Reads and writes of files that have mandatory record locking enabled
- Certain ioctl operations
- Some of the interprocess communications functions
