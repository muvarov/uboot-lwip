/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#ifndef LWIP_PING_H
#define LWIP_PING_H

#include <lwip/ip_addr.h>

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS   0
#endif

int lwip_ping_init(char *ping_addr);

void ping_raw_init(void);
void ping_send_now(void);

#endif /* LWIP_PING_H */
