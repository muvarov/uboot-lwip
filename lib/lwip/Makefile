# SPDX-License-Identifier: GPL-2.0+
#
# (C) Copyright 2023 Linaro Ltd. <maxim.uvarov@linaro.org>

LWIPDIR=lwip-external/src

ccflags-y += -I$(CURDIR)/lib/lwip/port/include
ccflags-y += -I$(CURDIR)/lib/lwip/lwip-external/src/include -I$(CURDIR)/lib/lwip

obj-$(CONFIG_LWIP_LIB) += $(LWIPDIR)/core/init.o \
	$(LWIPDIR)/core/def.o \
	$(LWIPDIR)/core/dns.o \
	$(LWIPDIR)/core/inet_chksum.o \
	$(LWIPDIR)/core/ip.o \
	$(LWIPDIR)/core/mem.o \
	$(LWIPDIR)/core/memp.o \
	$(LWIPDIR)/core/netif.o \
	$(LWIPDIR)/core/pbuf.o \
	$(LWIPDIR)/core/raw.o \
	$(LWIPDIR)/core/stats.o \
	$(LWIPDIR)/core/sys.o \
	$(LWIPDIR)/core/altcp.o \
	$(LWIPDIR)/core/altcp_alloc.o \
	$(LWIPDIR)/core/altcp_tcp.o \
	$(LWIPDIR)/core/tcp.o \
	$(LWIPDIR)/core/tcp_in.o \
	$(LWIPDIR)/core/tcp_out.o \
	$(LWIPDIR)/core/timeouts.o \
	$(LWIPDIR)/core/udp.o

# IPv4
obj-$(CONFIG_LWIP_LIB) += $(LWIPDIR)/core/ipv4/acd.o \
        $(LWIPDIR)/core/ipv4/autoip.o \
        $(LWIPDIR)/core/ipv4/dhcp.o \
        $(LWIPDIR)/core/ipv4/etharp.o \
        $(LWIPDIR)/core/ipv4/icmp.o \
        $(LWIPDIR)/core/ipv4/igmp.o \
        $(LWIPDIR)/core/ipv4/ip4_frag.o \
        $(LWIPDIR)/core/ipv4/ip4.o \
        $(LWIPDIR)/core/ipv4/ip4_addr.o
# IPv6
obj-$(CONFIG_LWIP_LIB) += $(LWIPDIR)/core/ipv6/dhcp6.o \
        $(LWIPDIR)/core/ipv6/ethip6.o \
        $(LWIPDIR)/core/ipv6/icmp6.o \
        $(LWIPDIR)/core/ipv6/inet6.o \
        $(LWIPDIR)/core/ipv6/ip6.o \
        $(LWIPDIR)/core/ipv6/ip6_addr.o \
        $(LWIPDIR)/core/ipv6/ip6_frag.o \
        $(LWIPDIR)/core/ipv6/mld6.o \
        $(LWIPDIR)/core/ipv6/nd6.o
# API
obj-$(CONFIG_LWIP_LIB) += $(LWIPDIR)/api/api_lib.o \
	$(LWIPDIR)/api/api_msg.o \
	$(LWIPDIR)/api/err.o \
	$(LWIPDIR)/api/if_api.o \
	$(LWIPDIR)/api/netbuf.o \
	$(LWIPDIR)/api/netdb.o \
	$(LWIPDIR)/api/netifapi.o \
	$(LWIPDIR)/api/sockets.o \
	$(LWIPDIR)/api/tcpip.o

# Netdevs
obj-$(CONFIG_LWIP_LIB) += $(LWIPDIR)/netif/ethernet.o

obj-$(CONFIG_LWIP_LIB) += port/if.o
obj-$(CONFIG_LWIP_LIB) += port/sys-arch.o

obj-$(CONFIG_CMD_LWIP) += cmd-lwip.o

.PHONY: $(obj)/apps/ping/ping.c
$(obj)/apps/ping/ping.o: $(obj)/apps/ping/ping.c
$(obj)/apps/ping/ping.c:
	cp ./lib/lwip/lwip-external/contrib/apps/ping/ping.c $(obj)/apps/ping/ping.c

obj-$(CONFIG_CMD_LWIP) += apps/ping/ping.o
obj-$(CONFIG_CMD_LWIP) += apps/ping/lwip_ping.o

$(obj)/apps/http/http_clinet.o: $(obj)/apps/http/http_client.c
.PHONY: $(obj)/apps/http/http_client.c
$(obj)/apps/http/http_client.c:
	cp ./lib/lwip/lwip-external/src/apps/http/http_client.c $(obj)/apps/http/http_client.c
	cp ./lib/lwip/lwip-external/src/include/lwip/apps/http_client.h $(obj)/apps/http/http_client.h

obj-$(CONFIG_CMD_LWIP_WGET) += apps/http/http_client.o
obj-$(CONFIG_CMD_LWIP_WGET) += apps/http/lwip-wget.o

$(obj)/apps/tftp/tftp.o: $(obj)/apps/tftp/tftp.c
.PHONY: $(obj)/apps/tftp/tftp.c
$(obj)/apps/tftp/tftp.c:
	cp ./lib/lwip/lwip-external/src/apps/tftp/tftp.c $(obj)/apps/tftp/tftp.c
	cp ./lib/lwip/lwip-external/src/include/lwip/apps/tftp_client.h $(obj)/apps/tftp/tftp_client.h
	cp ./lib/lwip/lwip-external/src/include/lwip/apps/tftp_common.h $(obj)/apps/tftp/tftp_common.h
	cp ./lib/lwip/lwip-external/contrib/examples/tftp/tftp_example.h $(obj)/apps/tftp/tftp_example.h

obj-$(CONFIG_CMD_LWIP_TFTP) += apps/tftp/tftp.o
obj-$(CONFIG_CMD_LWIP_TFTP) += apps/tftp/lwip-tftp.o

obj-$(CONFIG_CMD_LWIP_DHCP) += apps/dhcp/lwip-dhcp.o
