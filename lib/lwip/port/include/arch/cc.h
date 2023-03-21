/* SPDX-License-Identifier: GPL-2.0 */

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#ifndef LWIP_ARCH_CC_H
#define LWIP_ARCH_CC_H

#include <linux/types.h>
#include <linux/kernel.h>

#define LWIP_ERRNO_INCLUDE <errno.h>

#define LWIP_ERRNO_STDINCLUDE	1
#define LWIP_NO_UNISTD_H 1
#define LWIP_TIMEVAL_PRIVATE 1

extern unsigned int lwip_port_rand(void);
#define LWIP_RAND() (lwip_port_rand())

/* different handling for unit test, normally not needed */
#ifdef LWIP_NOASSERT_ON_ERROR
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
						    handler; }} while (0)
#endif

#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS

#define LWIP_PLATFORM_ASSERT(x) do {printf("Assertion \"%s\" failed at line %d in %s\n", \
				    x, __LINE__, __FILE__); } while (0)

static inline int atoi(const char *str)
{
	int r = 0;
	int i;

	for (i = 0; str[i] != '\0'; ++i)
		r = r * 10 + str[i] - '0';

	return r;
}

#define LWIP_ERR_T int

#endif /* LWIP_ARCH_CC_H */
