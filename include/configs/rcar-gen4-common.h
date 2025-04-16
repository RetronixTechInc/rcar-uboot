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

#define CONFIG_BOOTCOMMAND	\
	"run bootcmd_mmc"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"file_kernel=/boot/Image\0" \
	"file_dtb=/boot/r8a779g0-sbc.dtb\0" \
	"loadaddr=0x48080000\0" \
	"loadaddr_dtb=0x48000000\0" \
	"loadaddr_env=0x48060000\0" \
	\
	"locate_storage=mmc dev 0\0" \
	"load_kernel=${loadcmd} ${loadaddr} ${file_kernel};${loadcmd} ${loadaddr_dtb} ${file_dtb}\0" \
	"loadcmd_tftp=setenv loadcmd tftp\0" \
	"loadcmd_mmc=setenv loadcmd ext4load mmc 0:2\0" \
	\
	"ethaddr=2E:09:0A:06:F0:23\0" \
	"ipaddr=192.168.0.20\0" \
	"serverip=192.168.0.1\0" \
	"serverfold=/export/rfs\0" \
	\
	"cma_size=560M\0" \
	"pcie_option=pci=pcie_bus_perf\0" \
	\
	"panel_overlay_env=panel_overlay.env\0" \
	"panel_env_load=ext4load mmc 0:1 ${loadaddr_env} ${panel_overlay_env}\0" \
	"panel_env_import=env import -t ${loadaddr_env} ${filesize}\0" \
	"panel_overlay_script=echo Check panel dtb overlay...; " \
		"if run panel_env_load; " \
		"then " \
			"run panel_env_import; " \
			"if test -n ${panel_dtbo}; then run load_panel_dtbo; else echo No definition of panel_dtbo; fi; " \
		"else " \
			"echo There is no ${panel_overlay_env}; " \
		"fi;\0" \
	\
	"bootargs_nfs=setenv bootargs rw root=/dev/nfs nfsroot=${serverip}:${serverfold},nfsvers=3 ip=dhcp cma=${cma_size},clk_ignore_unused ${pcie_option}\0"\
	"bootcmd_nfs=run bootargs_nfs;run loadcmd_tftp;run load_kernel;run panel_overlay_script;booti ${loadaddr} - ${loadaddr_dtb}\0" \
	\
	"bootargs_mmc=setenv bootargs rw root=/dev/mmcblk0p2 rootfstype=ext4 rootwait cma=${cma_size},clk_ignore_unused ${pcie_option}\0"\
	"bootcmd_mmc=run bootargs_mmc;run loadcmd_mmc;run locate_storage;run load_kernel;run panel_overlay_script; booti ${loadaddr} - ${loadaddr_dtb}\0"

#endif	/* __RCAR_GEN4_COMMON_H */
