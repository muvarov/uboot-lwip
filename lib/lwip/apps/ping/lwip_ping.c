// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include "lwip/opt.h"
#include "lwip/ip_addr.h"
#include "ping.h"

#include "../../../lwip/ulwip.h"

static ip_addr_t ip_target;

static int ulwip_ping_tmo(void)
{

	printf("ping failed; host %s is not alive\n", ipaddr_ntoa(&ip_target));
	return 0;
}

int lwip_ping_init(char *ping_addr)
{
	int err;

	err = ipaddr_aton(ping_addr, &ip_target);
	if (err == 0) {
		printf("wrong ping addr string \"%s\" \n", ping_addr);
		return -1;
	}

	ulwip_set_tmo(ulwip_ping_tmo);

	ping_init(&ip_target);

	return 0;
}
