/* vi: set sw=4 ts=4: */
/*
 * fallocate() for uClibc - Based off of posix_fallocate() by Erik Andersen
 * http://www.opengroup.org/onlinepubs/9699919799/functions/posix_fallocate.html
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1 or later, see the file COPYING.LIB in this tarball.
 */

#include <sys/syscall.h>
#include <fcntl.h>
#include <bits/kernel-features.h>
#include <stdint.h>

#if defined __NR_fallocate
extern __typeof(fallocate) __libc_fallocate attribute_hidden;
int attribute_hidden __libc_fallocate(int fd, int mode, __off_t offset, __off_t len)
{
	int ret;

# if __WORDSIZE == 32
	uint32_t off_low = offset;
	uint32_t len_low = len;
	/* may assert that these >>31 are 0 */
	uint32_t zero = 0;
	INTERNAL_SYSCALL_DECL(err);
	ret = (int) (INTERNAL_SYSCALL(fallocate, err, 6, fd, mode,
		__LONG_LONG_PAIR (zero, off_low),
		__LONG_LONG_PAIR (zero, len_low)));
# elif __WORDSIZE == 64
	INTERNAL_SYSCALL_DECL(err);
	ret = (int) (INTERNAL_SYSCALL(fallocate, err, 4, fd, mode, offset, len));
# else
# error your machine is neither 32 bit or 64 bit ... it must be magical
# endif
	if (unlikely(INTERNAL_SYSCALL_ERROR_P (ret, err)))
		return INTERNAL_SYSCALL_ERRNO (ret, err);
	return 0;
}

# if defined __UCLIBC_LINUX_SPECIFIC__ && defined __USE_GNU
strong_alias(__libc_fallocate,fallocate)
#  if __WORDSIZE == 64
strong_alias(__libc_fallocate,fallocate64)
#  endif
# endif
#endif
