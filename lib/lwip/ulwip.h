/* SPDX-License-Identifier: GPL-2.0 */

int ulwip_enabled(void);
int ulwip_in_loop(void);
int ulwip_loop_set(int loop);
int ulwip_exit(int err);
int uboot_lwip_poll(void);
int ulwip_app_get_err(void);
void ulwip_set_tmo(int (*tmo)(void));
