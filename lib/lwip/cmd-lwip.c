// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Maxim Uvarov, maxim.uvarov@linaro.org
 */

#include <common.h>
#include <command.h>
#include <console.h>
#include <display_options.h>
#include <memalign.h>
#include <net.h>
#include <image.h>

#include "apps/ping/lwip_ping.h"
#include "ulwip.h"

extern int uboot_lwip_init(void);
extern int uboot_lwip_loop_is_done(void);

static int do_lwip_info(struct cmd_tbl *cmdtp, int flag, int argc,
		      char *const argv[])
{
	printf("TBD: %s\n", __func__);
	return CMD_RET_SUCCESS;
}

static int do_lwip_init(struct cmd_tbl *cmdtp, int flag, int argc,
		      char *const argv[])
{
	if (!uboot_lwip_init())
		return CMD_RET_SUCCESS;
	return CMD_RET_FAILURE;
}

static int lwip_empty_tmo(void) { return 0; };
int (*ulwip_tmo)(void) = lwip_empty_tmo;
void ulwip_set_tmo(int (*tmo)(void))
{
	ulwip_tmo = tmo;
}

static void ulwip_clear_tmo(void)
{
	ulwip_tmo = lwip_empty_tmo;
}

static void ulwip_timeout_handler(void)
{
	eth_halt();
	ulwip_tmo();
	net_set_state(NETLOOP_FAIL);	/* we did not get the reply */
	ulwip_loop_set(0);
}

static int ulwip_loop(void)
{
	ulwip_loop_set(1);
	if (net_loop(LWIP) < 0) {
		ulwip_loop_set(0);
		return CMD_RET_FAILURE;
	}
	ulwip_loop_set(0);
	return CMD_RET_SUCCESS;
}

#if !defined(CONFIG_CMD_LWIP_REPLACE_PING)
static
#endif
int do_lwip_ping(struct cmd_tbl *cmdtp, int flag, int argc,
		 char *const argv[])
{
	if (argc < 2) {
		printf("argc = %d, error\n", argc);
		return CMD_RET_USAGE;
	}

	uboot_lwip_init();

	eth_init(); /* activate u-boot eth dev */

	printf("Using %s device\n", eth_get_name());
	printf("pinging addr: %s\n", argv[1]);

	net_set_timeout_handler(1000UL, ulwip_timeout_handler);

	if (lwip_ping_init(argv[1])) {
		printf("ping init fail\n");
		return CMD_RET_FAILURE;
	}

	ping_send_now();

	return ulwip_loop();
}

extern int lwip_wget(ulong addr, char *url);

#if !defined(CONFIG_CMD_LWIP_REPLACE_WGET)
static
#endif
int do_lwip_wget(struct cmd_tbl *cmdtp, int flag, int argc,
		 char *const argv[])
{
	char *url;

	if (argc < 2) {
		printf("argc = %d, error\n", argc);
		return CMD_RET_USAGE;
	}
	url = argv[1];

	uboot_lwip_init();

	eth_init(); /* activate u-boot eth dev */

	lwip_wget(image_load_addr, url);

	return ulwip_loop();
}

#if defined(CONFIG_CMD_LWIP_TFTP)
extern int lwip_tftp(ulong addr, char *filename);
#if !defined(CONFIG_CMD_LWIP_REPLACE_TFTP)
static
#endif
int do_lwip_tftp(struct cmd_tbl *cmdtp, int flag, int argc,
		 char *const argv[])
{
	char *filename;
	ulong addr;
	char *end;
	int ret;

	switch (argc) {
	case 1:
		filename = env_get("bootfile");
		break;
	case 2:
		/*
		 * Only one arg - accept two forms:
		 * Just load address, or just boot file name. The latter
		 * form must be written in a format which can not be
		 * mis-interpreted as a valid number.
		 */
		addr = hextoul(argv[1], &end);
		if (end == (argv[1] + strlen(argv[1]))) {
			image_load_addr = addr;
			filename = env_get("bootfile");
		} else {
			filename = argv[1];
		}
		break;
	case 3:
		image_load_addr = hextoul(argv[1], NULL);
		filename = argv[2];
		break;
	default:
		return CMD_RET_USAGE;
	}

	uboot_lwip_init();

	eth_init(); /* activate u-boot eth dev */

	ret = lwip_tftp(image_load_addr, filename);
	if (ret)
		return ret;

	return ulwip_loop();
}
#endif

#if defined(CONFIG_CMD_LWIP_DHCP)
extern int ulwip_dhcp(void);

#if !defined(CONFIG_CMD_LWIP_REPLACE_DHCP)
static
#endif
int do_lwip_dhcp(void)
{
	int ret;
	char *filename;

	uboot_lwip_init();

	ret = ulwip_dhcp();

	net_set_timeout_handler(2000UL, ulwip_timeout_handler);

	ulwip_loop();
	if (IS_ENABLED(CONFIG_CMD_LWIP_TFTP)) {
		ulwip_clear_tmo();

		filename = env_get("bootfile");
		if (!filename) {
			printf("no bootfile\n");
			return CMD_RET_FAILURE;
		}

		eth_init(); /* activate u-boot eth dev */
		net_set_timeout_handler(20000UL, ulwip_timeout_handler);
		lwip_tftp(image_load_addr, filename);

		ret =  ulwip_loop();
	}

	return ret;
}

static int _do_lwip_dhcp(struct cmd_tbl *cmdtp, int flag, int argc,
		char *const argv[])
{
	return do_lwip_dhcp();
}
#endif

static struct cmd_tbl cmds[] = {
	U_BOOT_CMD_MKENT(info, 1, 0, do_lwip_info, "Info and stats", ""),
	U_BOOT_CMD_MKENT(init, 1, 0, do_lwip_init,
			 "initialize lwip stack", ""),
#if defined(CONFIG_CMD_LWIP_PING)
	U_BOOT_CMD_MKENT(ping, 2, 0, do_lwip_ping,
			 "send ICMP ECHO_REQUEST to network host",
			 "pingAddress"),
#endif
#if defined(CONFIG_CMD_LWIP_WGET)
	U_BOOT_CMD_MKENT(wget, 2, 0, do_lwip_wget, "", ""),
#endif
#if defined(CONFIG_CMD_LWIP_TFTP)
	U_BOOT_CMD_MKENT(tftp, 3, 0, do_lwip_tftp,
			"boot image via network using TFTP protocol\n",
			"[loadAddress] [[hostIPaddr:]bootfilename]"),
#endif
#if defined(CONFIG_CMD_LWIP_DHCP)
	U_BOOT_CMD_MKENT(dhcp, 1, 0, _do_lwip_dhcp,
			"boot image via network using DHCP/TFTP protocol",
			""),
#endif
};

static int do_ops(struct cmd_tbl *cmdtp, int flag, int argc,
		     char *const argv[])
{
	struct cmd_tbl *cp;

	cp = find_cmd_tbl(argv[1], cmds, ARRAY_SIZE(cmds));

	argc--;
	argv++;

	if (cp == NULL || argc > cp->maxargs)
		return CMD_RET_USAGE;
	if (flag == CMD_FLAG_REPEAT && !cmd_is_repeatable(cp))
		return CMD_RET_SUCCESS;

	return cp->cmd(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(
	lwip, 4, 1, do_ops,
	"LWIP sub system",
	"info - display info\n"
	"init - init LWIP\n"
	"ping addr - pingAddress\n"
	"wget http://IPadress/url/\n"
	"tftp [loadAddress] [[hostIPaddr:]bootfilename]\n"
	"dhcp - boot image via network using DHCP/TFTP protocol\n"
	);

/* Old command kept for compatibility. Same as 'mmc info' */
U_BOOT_CMD(
	lwipinfo, 1, 0, do_lwip_info,
	"display LWIP info",
	"- display LWIP stack info"
);
