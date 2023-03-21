// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include <common.h>
#include <rand.h>
#include "lwip/opt.h"

u32_t sys_now(void)
{
	return get_timer(0);
}

u32_t lwip_port_rand(void)
{
	return (u32_t)rand();
}

