/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * include/configs/rcar-gen4-common.h
 *	This file is R-Car Gen4 common configuration file.
 *
 * Copyright (C) 2021 Renesas Electronics Corporation
 */

#ifndef __RCAR_GEN4_COMMON_H
#define __RCAR_GEN4_COMMON_H

#include <asm/arch/rmobile.h>

#define CONFIG_REMAKE_ELF

/* boot option */

#define CONFIG_SYS_BOOTPARAMS_LEN	SZ_128K

/* Generic Interrupt Controller Definitions */
#define GICR_BASE	(GICR_LPI_BASE)

/* console */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BAUDRATE_TABLE	{ 38400, 115200, 921600, 1843200 }

/* PHY needs a longer autoneg timeout */
#define PHY_ANEG_TIMEOUT		20000

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

#define DRAM_RSV_SIZE			0x08000000
#define CONFIG_SYS_SDRAM_BASE		(0x40000000 + DRAM_RSV_SIZE)
#define CONFIG_SYS_SDRAM_SIZE		(0x80000000u - DRAM_RSV_SIZE)
#define CONFIG_VERY_BIG_RAM
#define CONFIG_MAX_MEM_MAPPED		(0x80000000u - DRAM_RSV_SIZE)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)

/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/* ENV setting */

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"bootm_size=0x10000000\0"

#define CONFIG_BOOTCOMMAND	\
	"run bootcmd_emmc"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x48080000\0" \
	"file_kernel=/boot/Image\0" \
	"loadaddr_dtb=0x48000000\0" \
	"file_dtb=/boot/r8a779g0-hel.dtb\0" \
	"loadaddr_dtbo=0x48060000\0" \
	\
	"load_kernel=${loadcmd} ${loadaddr} ${file_kernel};${loadcmd} ${loadaddr_dtb} ${file_dtb}\0" \
	"ipaddr=192.168.0.20\0" \
	"serverip=192.168.0.1\0" \
	"serverfold=/export/rfs\0" \
	"bootargs_nfs=setenv bootargs rw root=/dev/nfs nfsroot=${serverip}:${serverfold},nfsvers=3 ip=dhcp cma=${cma_size},clk_ignore_unused\0" \
	"load_tftp=setenv loadcmd tftp\0" \
	"bootcmd_nfs=run bootargs_nfs;run load_tftp;run load_kernel;booti ${loadaddr} - ${loadaddr_dtb}\0" \
	"bootargs_mmc=setenv bootargs rw root=/dev/mmcblk0p2 rootfstype=ext4 rootwait cma=${cma_size},clk_ignore_unused pci=pcie_bus_perf\0" \
	"load_emmc=setenv loadcmd ext4load mmc 0:2\0" \
	"bootcmd_emmc=run bootargs_mmc;run load_emmc;run load_kernel;booti ${loadaddr} - ${loadaddr_dtb}\0" \
	"cma_size=900M\0"

#endif	/* __RCAR_GEN4_COMMON_H */
