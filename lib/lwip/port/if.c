// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include <common.h>
#include <command.h>
extern int eth_init(void); /* net.h */
extern void string_to_enetaddr(const char *addr, uint8_t *enetaddr); /* net.h */
extern struct in_addr net_ip;
extern u8 net_ethaddr[6];

#include "lwip/debug.h"
#include "lwip/arch.h"
#include "netif/etharp.h"
#include "lwip/stats.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/netif.h"

#include "lwip/ip.h"

#define IFNAME0 'e'
#define IFNAME1 '0'

static struct pbuf *low_level_input(struct netif *netif);
static int uboot_net_use_lwip;

int ulwip_enabled(void)
{
	return uboot_net_use_lwip;
}

/* 1 - in loop
 * 0 - no loop
 */
static int loop_lwip;

/* ret 1 - in loop
 *     0 - no loop
 */
int ulwip_in_loop(void)
{
	return loop_lwip;
}

void ulwip_loop_set(int loop)
{
	loop_lwip = loop;
}

static int ulwip_app_err;

void ulwip_exit(int err)
{
	ulwip_app_err = err;
	ulwip_loop_set(0);
}

int ulwip_app_get_err(void)
{
	return ulwip_app_err;
}

struct uboot_lwip_if {
};

#if defined(CONFIG_CMD_LWIP_DHCP)
struct netif uboot_netif;
#else
static struct netif uboot_netif;
#endif

#define LWIP_PORT_INIT_NETMASK(addr)  IP4_ADDR((addr), 255, 255, 255, 0)

extern uchar *net_rx_packet;
extern int    net_rx_packet_len;

int uboot_lwip_poll(void)
{
	struct pbuf *p;
	int err;

	p = low_level_input(&uboot_netif);
	if (!p) {
		printf("error p = low_level_input = NULL\n");
		return 0;
	}

	err = ethernet_input(p, &uboot_netif);
	if (err)
		printf("ip4_input err %d\n", err);

	return 0;
}

static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *p, *q;
	u16_t len = net_rx_packet_len;
	uchar *data = net_rx_packet;

#if ETH_PAD_SIZE
	len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
	if (p) {
#if ETH_PAD_SIZE
		pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif
		/* We iterate over the pbuf chain until we have read the entire
		 * packet into the pbuf.
		 */
		for (q = p; q != NULL; q = q->next) {
			/* Read enough bytes to fill this pbuf in the chain. The
			 * available data in the pbuf is given by the q->len
			 * variable.
			 * This does not necessarily have to be a memcpy, you can also preallocate
			 * pbufs for a DMA-enabled MAC and after receiving truncate it to the
			 * actually received size. In this case, ensure the tot_len member of the
			 * pbuf is the sum of the chained pbuf len members.
			 */
			MEMCPY(q->payload, data, q->len);
			data += q->len;
		}
		//acknowledge that packet has been read();

#if ETH_PAD_SIZE
		pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
		LINK_STATS_INC(link.recv);
	} else {
		//drop packet();
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
	}

	return p;
}

static int ethernetif_input(struct pbuf *p, struct netif *netif)
{
	struct ethernetif *ethernetif;

	ethernetif = netif->state;

	/* move received packet into a new pbuf */
	p = low_level_input(netif);

	/* if no packet could be read, silently ignore this */
	if (p) {
		/* pass all packets to ethernet_input, which decides what packets it supports */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("%s: IP input error\n", __func__));
			pbuf_free(p);
			p = NULL;
		}
	}
	return 0;
}

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	int err;

	err = eth_send(p->payload, p->len);
	if (err != 0) {
		printf("eth_send error %d\n", err);
		return ERR_ABRT;
	}
	return ERR_OK;
}

err_t uboot_lwip_if_init(struct netif *netif)
{
	struct uboot_lwip_if *uif = (struct uboot_lwip_if *)malloc(sizeof(struct uboot_lwip_if));

	if (!uif) {
		printf("uboot_lwip_if: out of memory\n");
		return ERR_MEM;
	}
	netif->state = uif;

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	netif->hwaddr_len = ETHARP_HWADDR_LEN;
	string_to_enetaddr(env_get("ethaddr"), netif->hwaddr);
#if defined(CONFIG_LWIP_LIB_DEBUG)
	printf("              MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2],
	       netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
#endif

#if LWIP_IPV4
	netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
	netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
	netif->linkoutput = low_level_output;
	netif->mtu = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	eth_init(); /* activate u-boot eth dev */

	if (IS_ENABLED(CONFIG_LWIP_LIB_DEBUG)) {
		printf("Initialized LWIP stack\n");
	}

	return ERR_OK;
}

int uboot_lwip_init(void)
{
	ip4_addr_t ipaddr, netmask, gw;

	if (uboot_net_use_lwip)
		return CMD_RET_SUCCESS;

	ip4_addr_set_zero(&gw);
	ip4_addr_set_zero(&ipaddr);
	ip4_addr_set_zero(&netmask);

	ipaddr_aton(env_get("ipaddr"), &ipaddr);
	ipaddr_aton(env_get("ipaddr"), &netmask);

	LWIP_PORT_INIT_NETMASK(&netmask);
	if (IS_ENABLED(CONFIG_LWIP_LIB_DEBUG)) {
		printf("Starting lwIP, IP: %s\n", ip4addr_ntoa(&ipaddr));
		printf("               GW: %s\n", ip4addr_ntoa(&gw));
		printf("             mask: %s\n", ip4addr_ntoa(&netmask));
	}

	if (!netif_add(&uboot_netif, &ipaddr, &netmask, &gw,
		       &uboot_netif, uboot_lwip_if_init, ethernetif_input))
		printf("err: netif_add failed!\n");

	netif_set_up(&uboot_netif);
	netif_set_link_up(&uboot_netif);
#if LWIP_IPV6
	netif_create_ip6_linklocal_address(&uboot_netif, 1);
	printf("             IPv6: %s\n", ip6addr_ntoa(netif_ip6_addr(uboot_netif, 0)));
#endif /* LWIP_IPV6 */

	uboot_net_use_lwip = 1;

	return CMD_RET_SUCCESS;
}

/* placeholder, not used now */
void uboot_lwip_destroy(void)
{
	uboot_net_use_lwip = 0;
}
