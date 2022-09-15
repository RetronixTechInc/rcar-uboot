/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * include/configs/rcar-gen3-common.h
 *	This file is R-Car Gen3 common configuration file.
 *
 * Copyright (C) 2015-2017 Renesas Electronics Corporation
 */

#ifndef __RCAR_GEN3_COMMON_H
#define __RCAR_GEN3_COMMON_H

#include <asm/arch/rmobile.h>

#define CONFIG_REMAKE_ELF

#ifdef CONFIG_SPL
#define CONFIG_SPL_TARGET	"spl/u-boot-spl.scif"
#endif

/* boot option */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/* Generic Interrupt Controller Definitions */
#define CONFIG_GICV2
#define GICD_BASE	0xF1010000
#define GICC_BASE	0xF1020000

/* console */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 38400 }

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

#define DRAM_RSV_SIZE			0x08000000
#define CONFIG_SYS_SDRAM_BASE		(0x40000000 + DRAM_RSV_SIZE)
#define CONFIG_SYS_SDRAM_SIZE		(0x80000000u - DRAM_RSV_SIZE)
//#define CONFIG_SYS_LOAD_ADDR		0x58000000
#define CONFIG_SYS_LOAD_ADDR		0x48080000
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR
#define CONFIG_VERY_BIG_RAM
#define CONFIG_MAX_MEM_MAPPED		(0x80000000u - DRAM_RSV_SIZE)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(8 * 1024 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)

/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/* ENV setting */
#define CONFIG_ENV_OVERWRITE

//#define CONFIG_IPADDR			10.0.0.242

#define CONFIG_EXTRA_ENV_SETTINGS					\
	"bootargs_ram=setenv bootargs root=/dev/ram0 rdinit=/sbin/init eth_type=${eth_type} geth_ms=${geth_ms}\0"					\
	"bootargs_sys_nfs=setenv bootargs rw root=/dev/nfs nfsroot=${nfs_server}:/export/rfs ip=dhcp cma=560M nfsvers=3 ov10635.dvp_order=1 eth_type=${eth_type} geth_ms=${geth_ms}\0"		\
	"bootargs_sys_p2=setenv bootargs init=/sbin/init root=/dev/mmcblk0p2 rootwait rw ov10635.dvp_order=1 eth_type=${eth_type} geth_ms=${geth_ms}\0"	\
	"bootcmd_ram=run bootargs_ram storage r_kernel r_dtb r_ramdisk;booti ${loadaddr} ${rd_loadaddr} ${dtb_loadaddr}\0"				\
	"bootcmd_nfs=run bootargs_sys_nfs load_mmc_p0; booti ${loadaddr} - ${dtb_loadaddr}\0"								\
	"bootcmd_mmc=run bootargs_sys_p2 load_mmc_p1; booti ${loadaddr} - ${dtb_loadaddr}\0"								\
	"load_mmc_p0=run storage r_kernel r_dtb\0"			\
	"load_mmc_p1=run storage; ext4load mmc 0:1 ${loadaddr} Image; ext4load mmc 0:1 ${dtb_loadaddr} r8a77980-es2-condor.dtb\0"			\
	"load_mmc_tftp=tftp ${loadaddr} Image;tftp ${dtb_loadaddr} r8a77980-es2-condor.dtb\0"								\
	"r_dtb=mmc read ${dtb_loadaddr} 0x6400 0x400\0"			\
	"r_kernel=mmc read ${loadaddr} 0x6800 0xF000\0"			\
	"r_ramdisk=mmc read ${rd_loadaddr} 0x18800 0x8000\0"		\
	"dtb_loadaddr=0x48000000\0"					\
	"rd_loadaddr=0x52000000\0"					\
	"eth_type=avb\0"						\
	"geth_ms=2\0"							\
	"platform=r8a77980\0"						\
	"storage=mmc dev 0\0"						\
	"usb_pgood_delay=2000\0"					\
	"bootm_size=0x10000000\0"

#define CONFIG_BOOTCOMMAND	\
	"run bootcmd_ram"

/* SPL support */
#if defined(CONFIG_R8A7795) || defined(CONFIG_R8A7796) || defined(CONFIG_R8A77965)
#define CONFIG_SPL_BSS_START_ADDR	0xe633f000
#define CONFIG_SPL_BSS_MAX_SIZE		0x1000
#else
#define CONFIG_SPL_BSS_START_ADDR	0xe631f000
#define CONFIG_SPL_BSS_MAX_SIZE		0x1000
#endif
#define CONFIG_SPL_STACK		0xe6304000
#define CONFIG_SPL_MAX_SIZE		0x7000
#ifdef CONFIG_SPL_BUILD
#define CONFIG_CONS_SCIF2
#define CONFIG_SH_SCIF_CLK_FREQ		65000000
#endif

#endif	/* __RCAR_GEN3_COMMON_H */
