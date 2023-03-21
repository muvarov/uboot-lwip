// SPDX-License-Identifier: GPL-2.0

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#include <common.h>
#include <command.h>
#include <console.h>

#include "http_client.h"
#include "../../../lwip/ulwip.h"

static ulong daddr;
static httpc_connection_t settings;

static err_t httpc_recv(void *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err)
{
	struct pbuf *q;
	LWIP_UNUSED_ARG(err);

	if (!p)
		return ERR_BUF;

	for (q = p; q != NULL; q = q->next) {
		memcpy((void *)daddr, q->payload, q->len);
		printf("downloaded chunk size %d, to addr 0x%lx\n", q->len, daddr);
		daddr += q->len;
	}
	altcp_recved(pcb, p->tot_len);
	pbuf_free(p);
	return ERR_OK;
}

static void httpc_result(void *arg, httpc_result_t httpc_result, u32_t rx_content_len,
			 u32_t srv_res, err_t err)
{
	if (httpc_result == HTTPC_RESULT_OK) {
		printf("\n%d bytes successfully downloaded.\n", rx_content_len);
		ulwip_exit(0);
	} else {
		printf("\nhttp eroror: %d\n", httpc_result);
		ulwip_exit(-1);
	}
}

int lwip_wget(ulong addr, char *url)
{
	err_t err;
	int port = 80;
	char *server_name;
	httpc_state_t *connection;

	daddr = addr;
	server_name = env_get("serverip");
	if (!server_name) {
		printf("error: serverip variable has to be set\n");
		return CMD_RET_FAILURE;
	}

	printf("downloading %s to addr 0x%lx\n", url, addr);
	memset(&settings, 0, sizeof(httpc_connection_t));
	settings.result_fn = httpc_result;
	err = httpc_get_file_dns(server_name, port, url, &settings,
				 httpc_recv, NULL,  &connection);
	if (err != ERR_OK) {
		printf("httpc_init_connection failed\n");
		return err;
	}
	return 0;
}
