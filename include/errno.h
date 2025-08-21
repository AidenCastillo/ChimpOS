#ifndef ERRNO_H
#define ERRNO_H

#define EPERM -1   // Operation not permitted
#define ENOENT -2  // No such file or directory
#define ESRCH -3   // No such process
#define EINTR -4  // Interrupted system call
#define EIO -5     // Input/output error
#define ENXIO -6  // No such device or address
#define EIOCTL -7  // Invalid ioctl command
#define EBUSY -8   // Device or resource busy
#define EEXIST -9  // File exists
#define ENOSPC -10 // No space left on device
#define EAGAIN -11 // Resource temporarily unavailable
#define ENOMEM -12  // Out of memory
#define EILSEQ -13 // Illegal byte sequence
#define EFAULT -14 // Bad address
#define ENOTDIR -15 // Not a directory
#define ENAMETOOLONG -16 // File name too long
#define ELOOP -17 // Too many symbolic links
#define ENOSYS -18 // Function not implemented
#define ENOTEMPTY -19 // Directory not empty
#define EILSEQ -20 // Illegal byte sequence
#define EFBIG -21 // File too large
#define EINVAL -22  // Invalid argument
#define EROFS -23 // Read-only file system
#define EPIPE -24 // Broken pipe
#define EDEADLK -25 // Resource deadlock avoided
#define ENOTRECOVERABLE -26 // State not recoverable
#define EOWNERDEAD -27 // Owner died
#endif
