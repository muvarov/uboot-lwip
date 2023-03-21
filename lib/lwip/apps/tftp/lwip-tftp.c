// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include <common.h>
#include <command.h>
#include <console.h>

#include "lwip/apps/tftp_client.h"
#include "lwip/apps/tftp_server.h"
#include "tftp_example.h"

#include <string.h>

#include "../../../lwip/ulwip.h"

#if LWIP_UDP

static ulong daddr;
static ulong size;

static void *tftp_open(const char *fname, const char *mode, u8_t is_write)
{
	LWIP_UNUSED_ARG(mode);
	return NULL;
}

static void tftp_close(void *handle)
{
	printf("\ndone\n");
	printf("Bytes transferred = %ld (0x%lx hex)\n", size, size);
	ulwip_exit(0);
}

static int tftp_read(void *handle, void *buf, int bytes)
{
	return 0;
}

static int tftp_write(void *handle, struct pbuf *p)
{
	struct pbuf *q;

	for (q = p; q != NULL; q = q->next) {
		memcpy((void *)daddr, q->payload, q->len);
		/* printf("downloaded chunk size %d, to addr 0x%lx\n", q->len, daddr); */
		daddr += q->len;
		size += q->len;
		printf("#");
	}

	return 0;
}

/* For TFTP client only */
static void tftp_error(void *handle, int err, const char *msg, int size)
{
	char message[100];

	LWIP_UNUSED_ARG(handle);

	memset(message, 0, sizeof(message));
	MEMCPY(message, msg, LWIP_MIN(sizeof(message)-1, (size_t)size));

	printf("TFTP error: %d (%s)", err, message);
}

static const struct tftp_context tftp = {
	tftp_open,
	tftp_close,
	tftp_read,
	tftp_write,
	tftp_error
};

int lwip_tftp(ulong addr, char *fname)
{
	void *f = (void *)0x1; /*fake handle*/
	err_t err;
	ip_addr_t srv;
	int ret;
	char *server_ip;

	if (!fname || addr == 0)
		return CMD_RET_FAILURE;

	size = 0;
	daddr = addr;
	server_ip = env_get("serverip");
	if (!server_ip) {
		printf("error: serverip variable has to be set\n");
		return CMD_RET_FAILURE;
	}

	ret = ipaddr_aton(server_ip, &srv);
	LWIP_ASSERT("ipaddr_aton failed", ret == 1);

	printf("TFTP from server %s; our IP address is %s\n",
			server_ip, env_get("ipaddr"));
	printf("Filename '%s'.\n", fname);
	printf("Load address: 0x%lx\n", daddr);
	printf("Loading:");

	err = tftp_init_client(&tftp);
	if (!(err == ERR_OK || err == ERR_USE))
		printf("tftp_init_client err: %d\n", err);

	err = tftp_get(f, &srv, TFTP_PORT, fname, TFTP_MODE_OCTET);
	/* might return different errors, like routing problems */
	if (err != ERR_OK) {
		printf("tftp_get err=%d\n", err);
	}
	LWIP_ASSERT("tftp_get failed", err == ERR_OK);

	return err;
}
#else
#error "UDP has to be supported"
#endif /* LWIP_UDP */
