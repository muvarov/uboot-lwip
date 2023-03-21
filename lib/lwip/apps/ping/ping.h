/* SPDX-License-Identifier: GPL-2.0 */

#include "../../../lwip/ulwip.h"

#include "lwip/prot/ip4.h"

#define ip4_print_parts(a, b, c, d) \
	printf("%" U16_F ".%" U16_F ".%" U16_F ".%" U16_F, a, b, c, d);

#define ip4_print(ipaddr) \
	ip4_print_parts(\
			(u16_t)((ipaddr) != NULL ? ip4_addr1_16(ipaddr) : 0), \
			(u16_t)((ipaddr) != NULL ? ip4_addr2_16(ipaddr) : 0), \
			(u16_t)((ipaddr) != NULL ? ip4_addr3_16(ipaddr) : 0), \
			(u16_t)((ipaddr) != NULL ? ip4_addr4_16(ipaddr) : 0))


#define LWIP_DEBUG 1 /* ping_time is under ifdef*/
#define PING_RESULT(cond) { \
	if (cond == 1) { \
		printf("host "); \
		ip4_print(addr); \
		printf(" is alive\n"); \
		printf(" %"U32_F" ms\n", (sys_now() - ping_time)); \
		ulwip_exit(0); \
	} else { \
		printf("ping failed; host "); \
		ip4_print(addr); \
		printf(" is not alive\n"); \
		ulwip_exit(-1); \
	} \
     } while (0);

#include "lwip/ip_addr.h"
void ping_init(const ip_addr_t *ping_addr);
