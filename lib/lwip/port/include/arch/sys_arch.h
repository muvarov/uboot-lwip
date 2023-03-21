/* SPDX-License-Identifier: GPL-2.0 */

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/err.h"

#define ERR_NEED_SCHED 123

void sys_arch_msleep(u32_t delay_ms);
#define sys_msleep(ms) sys_arch_msleep(ms)

#if SYS_LIGHTWEIGHT_PROT
typedef u32_t sys_prot_t;
#endif /* SYS_LIGHTWEIGHT_PROT */

#include <errno.h>

#define SYS_MBOX_NULL NULL
#define SYS_SEM_NULL  NULL

typedef u32_t sys_prot_t;

struct sys_sem;
typedef struct sys_sem *sys_sem_t;
#define sys_sem_valid(sem) (((sem) != NULL) && (*(sem) != NULL))
#define sys_sem_set_invalid(sem) do { if ((sem) != NULL) { *(sem) = NULL; }} while (0)

/* let sys.h use binary semaphores for mutexes */
#define LWIP_COMPAT_MUTEX 1
#define LWIP_COMPAT_MUTEX_ALLOWED 1

struct sys_mbox;
typedef struct sys_mbox *sys_mbox_t;
#define sys_mbox_valid(mbox) (((mbox) != NULL) && (*(mbox) != NULL))
#define sys_mbox_set_invalid(mbox) do { if ((mbox) != NULL) { *(mbox) = NULL; }} while (0)

struct sys_thread;
typedef struct sys_thread *sys_thread_t;

static inline u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	return 0;
};

static inline err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	return 0;
};

#define sys_sem_signal(s)

#endif /* LWIP_ARCH_SYS_ARCH_H */
