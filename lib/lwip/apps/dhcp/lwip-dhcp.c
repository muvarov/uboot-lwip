// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include <common.h>
#include <command.h>
#include <console.h>

#include <lwip/dhcp.h>
#include <lwip/prot/dhcp.h>

#include "../../../lwip/ulwip.h"

static struct dhcp dhcp;
static bool dhcp_is_set;
extern struct netif uboot_netif;

static int ulwip_dhcp_tmo(void)
{
	switch (dhcp.state) {
	case DHCP_STATE_BOUND:
		env_set("bootfile", dhcp.boot_file_name);
		env_set("ipaddr", ip4addr_ntoa(&dhcp.offered_ip_addr));
		env_set("netmask", ip4addr_ntoa(&dhcp.offered_sn_mask));
		env_set("serverip", ip4addr_ntoa(&dhcp.server_ip_addr));
		printf("DHCP client bound to address %s\n", ip4addr_ntoa(&dhcp.offered_ip_addr));
		break;
	default:
		return 0;
	}

	return 0;
}

int ulwip_dhcp(void)
{
	int err;

	ulwip_set_tmo(ulwip_dhcp_tmo);

	if (!dhcp_is_set) {
		dhcp_set_struct(&uboot_netif, &dhcp);
		dhcp_is_set = true;
	}
	err = dhcp_start(&uboot_netif);
	if (err)
		printf("dhcp_start error %d\n", err);

	return err;
}
