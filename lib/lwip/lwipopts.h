/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>
 */

#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

#include "lwipopts.h"

#if defined(CONFIG_LWIP_LIB_DEBUG)
#define LWIP_DEBUG 1
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL
#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON
#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_OFF
#define MEMP_DEBUG                      LWIP_DBG_OFF
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_ON
#define AUTOIP_DEBUG                    LWIP_DBG_ON
#define DNS_DEBUG                       LWIP_DBG_OFF
#define IP6_DEBUG                       LWIP_DBG_OFF
#define DHCP6_DEBUG                     LWIP_DBG_OFF
#else
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL
#define LWIP_DBG_TYPES_ON               LWIP_DBG_OFF
#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_OFF
#define MEMP_DEBUG                      LWIP_DBG_OFF
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define AUTOIP_DEBUG                    LWIP_DBG_OFF
#define DNS_DEBUG                       LWIP_DBG_OFF
#define IP6_DEBUG                       LWIP_DBG_OFF
#define DHCP6_DEBUG                     LWIP_DBG_OFF
#endif
#define LWIP_TESTMODE                   0

#if defined(CONFIG_LWIP_LIB_NOASSERT)
#define LWIP_NOASSERT 1
#define LWIP_ASSERT(message, assertion)
#endif

#include "lwip/debug.h"

#define SYS_LIGHTWEIGHT_PROT            0
#define NO_SYS                          0

#define MEM_ALIGNMENT                   1
#define MEM_SIZE                        CONFIG_LWIP_LIB_MEM_SIZE

#define MEMP_NUM_PBUF                   4
#define MEMP_NUM_RAW_PCB                2
#define MEMP_NUM_UDP_PCB                4
#define MEMP_NUM_TCP_PCB                2
#define MEMP_NUM_TCP_PCB_LISTEN         2
#define MEMP_NUM_TCP_SEG                16
#define MEMP_NUM_REASSDATA              1
#define MEMP_NUM_ARP_QUEUE              2
#define MEMP_NUM_SYS_TIMEOUT            4
#define MEMP_NUM_NETBUF                 2
#define MEMP_NUM_NETCONN               32
#define MEMP_NUM_TCPIP_MSG_API          8
#define MEMP_NUM_TCPIP_MSG_INPKT        8
#define PBUF_POOL_SIZE                  8

#define LWIP_ARP                        1

#define IP_FORWARD                      0
#define IP_OPTIONS_ALLOWED              1
#define IP_REASSEMBLY                   1
#define IP_FRAG                         1
#define IP_REASS_MAXAGE                 3
#define IP_REASS_MAX_PBUFS              4
#define IP_FRAG_USES_STATIC_BUF         0

#define IP_DEFAULT_TTL                  255

#define LWIP_ICMP                       1

#define LWIP_RAW                        1

#if defined(CONFIG_LWIP_LIB_DHCP)
#define LWIP_DHCP                       1
#define LWIP_DHCP_BOOTP_FILE		1
#else
#define LWIP_DHCP                       0
#endif
#define LWIP_DHCP_DOES_ACD_CHECK	0

#define LWIP_AUTOIP                     0

#define LWIP_SNMP                       0

#define LWIP_IGMP                       0

#if defined(CONFIG_LWIP_LIB_DNS)
#define LWIP_DNS                        1
#else
#define LWIP_DNS                        0
#endif

#if defined(CONFIG_LWIP_LIB_TCP)
#define LWIP_UDP                        1
#else
#define LWIP_UDP                        0
#endif

#if defined(CONFIG_LWIP_LIB_TCP)
#define LWIP_TCP                        1
#else
#define LWIP_TCP                        0
#endif

#define LWIP_LISTEN_BACKLOG             0

#define PBUF_LINK_HLEN                  CONFIG_LWIP_LIB_PBUF_LINK_HLEN
#define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(TCP_MSS + 40 + PBUF_LINK_HLEN)

#define LWIP_HAVE_LOOPIF                0

#if defined(CONFIG_LWIP_LIB_NETCONN)
#define LWIP_NETCONN                    1
#else
#define LWIP_NETCONN                    0
#define LWIP_DISABLE_MEMP_SANITY_CHECKS 1
#endif

#if defined(CONFIG_LWIP_LIB_SOCKET)
#define LWIP_SOCKET                     1

#define SO_REUSE                        1
#else
#define LWIP_SOCKET                     0
#define SO_REUSE                        0
#endif

#define LWIP_STATS                      0

#define PPP_SUPPORT                     0

#define LWIP_TCPIP_CORE_LOCKING		0

#if defined(CONFIG_LWIP_LIB_LOOPBACK)
#define LWIP_NETIF_LOOPBACK		1
#else
#define LWIP_NETIF_LOOPBACK		0
#endif
/* use malloc instead of pool */
#define MEMP_MEM_MALLOC                 1
#define MEMP_MEM_INIT			1
#define MEM_LIBC_MALLOC			1

#endif /* LWIP_LWIPOPTS_H */
