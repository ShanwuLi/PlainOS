/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __PLAINOS_ERRNO_H__
#define __PLAINOS_ERRNO_H__

#define OK                 0     /* OK */
#define	EPERM              1     /* Operation not permitted */
#define	ENOENT             2     /* No such file or directory */
#define	ESRCH              3     /* No such process */
#define	EINTR              4     /* Interrupted system call */
#define	EIO                5     /* I/O error */
#define	ENXIO              6     /* No such device or address */
#define	E2BIG              7     /* Argument list too long */
#define	ENOEXEC            8     /* Exec format error */
#define	EBADF              9     /* Bad file number */
#define	ECHILD             10    /* No child processes */
#define	EAGAIN             11    /* Try again */
#define	ENOMEM             12    /* Out of memory */
#define	EACCES             13    /* Permission denied */
#define	EFAULT             14    /* Bad address */
#define	ENOTBLK            15    /* Block device required */
#define	EBUSY              16    /* Device or resource busy */
#define	EEXIST             17    /* File exists */
#define	EXDEV              18    /* Cross-device link */
#define	ENODEV             19    /* No such device */
#define	ENOTDIR            20    /* Not a directory */
#define	EISDIR             21    /* Is a directory */
#define	EINVAL             22    /* Invalid argument */
#define	ENFILE             23    /* File table overflow */
#define	EMFILE             24    /* Too many open files */
#define	ENOTTY             25    /* Not a typewriter */
#define	ETXTBSY            26    /* Text file busy */
#define	EFBIG              27    /* File too large */
#define	ENOSPC             28    /* No space left on device */
#define	ESPIPE             29    /* Illegal seek */
#define	EROFS              30    /* Read-only file system */
#define	EMLINK             31    /* Too many links */
#define	EPIPE              32    /* Broken pipe */
#define	EDOM               33    /* Math argument out of domain of func */
#define	ERANGE             34    /* Math result not representable */


#define EFULL              35    /* full error */
#define EEMPTY             36    /* empty error */
#define EREGED             37    /* Has registered */
#define ETIMEOUT           38    /* timeout */
#define ENOSUPPORT         39    /* not support */
#define ERROR              -40   /* Unknow error */

#define ERR_TO_PTR(err)    ({ extern long __errno_start[]; \
	                          long *ptr; \
	                          if ((err) < ERROR || (err) > OK) \
	                              ptr = NULL; \
	                          else \
	                              ptr = __errno_start - err; \
	                          ptr;})

#define PTR_TO_ERR(ptr)    ({ extern long __errno_start[]; \
	                          extern long __errno_end[]; \
	                          long err; \
	                          if ((ptr) < __errno_start || (ptr) > __errno_end) \
	                              err = ERROR; \
	                          else \
	                              err = *(ptr); \
	                          err;})

#define IS_ERR_OR_NULL(ptr) ({ extern long __errno_start[]; \
	                           extern long __errno_end[];   \
	                           bool is_err_or_null = false; \
	                           if (((ptr) >= __errno_start && (ptr) <= __errno_end) || \
	                               (ptr) == NULL) \
	                               is_err_or_null = true; \
	                           is_err_or_null;})

#endif /* __PLAINOS_ERRNO_H__ */
