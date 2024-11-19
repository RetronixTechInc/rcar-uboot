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
    "file_dtb=/boot/r8a779g0-raptor.dtb\0" \
    "loadaddr_dtbo=0x48060000\0" \
	\
    "dtbo_pcie=/boot/r8a779g0-raptor-overlay-pcie1x4.dtb\0" \
    "pcie_check=if gpio input 0; then echo pcie2x2; else echo pcie1x4;run pcie1x4;fi\0" \
    "pcie1x4=fdt addr ${loadaddr_dtb};fdt resize 8192;${loadcmd} ${loadaddr_dtbo} ${dtbo_pcie};fdt apply ${loadaddr_dtbo}\0" \
	\
    "dsi_example=dsi_0=SERDES or DP; dsi_1=SERDES or HDMI; dsi_panel0=10 or 7; dsi_panel1=10 or 7;\0" \
    "dsi_mem0_chk=0x4D000000\0" \
    "dsi_mem0_get=0x4D000001\0" \
    "dsi_mem1_chk=0x4D000002\0" \
    "dsi_mem1_get=0x4D000003\0" \
    "dsi_out0=if test X${dsi_0} = XSERDES ;then setenv dsi_set0 1;mw.b ${dsi_mem0_get} 1 1;else setenv dsi_set0 0;mw.b ${dsi_mem0_get} 0 1;fi;mw.b ${dsi_mem0_chk} 0 1;\0" \
    "dsi_out1=if test X${dsi_1} = XSERDES ;then setenv dsi_set1 1;mw.b ${dsi_mem1_get} 1 1;else setenv dsi_set1 0;mw.b ${dsi_mem1_get} 0 1;fi;mw.b ${dsi_mem1_chk} 0 1;\0" \
    "dsi_i2c_set=i2c dev 4;i2c mw 0x60 0x11.1 ${dsi_set0} 1;i2c mw 0x60 0x12.1 ${dsi_set1} 1\0" \
    "dsi_i2c_get=i2c dev 4;i2c read 0x60 0x13.1 1 ${dsi_mem0_get};i2c read 0x60 0x14.1 1 ${dsi_mem1_get}\0" \
    "dsi_gpio_get=if gpio input 0; then mw.b ${dsi_mem0_get} 0 1;mw.b ${dsi_mem1_get} 0 1; else mw.b ${dsi_mem0_get} 1 1;mw.b ${dsi_mem1_get} 1 1;fi\0" \
    "dsi_dtbo0_7=/boot/r8a779g0-raptor-overlay-dsi0-7.dtb\0" \
    "dsi_dtbo0_10=/boot/r8a779g0-raptor-overlay-dsi0-10.dtb\0" \
    "dsi_dtbo1_7=/boot/r8a779g0-raptor-overlay-dsi1-7.dtb\0" \
    "dsi_dtbo1_10=/boot/r8a779g0-raptor-overlay-dsi1-10.dtb\0" \
    "dsi_pancl0_7=fdt addr ${loadaddr_dtb};fdt resize 8192;${loadcmd} ${loadaddr_dtbo} ${dsi_dtbo0_7};fdt apply ${loadaddr_dtbo}\0" \
    "dsi_pancl0_10=fdt addr ${loadaddr_dtb};fdt resize 8192;${loadcmd} ${loadaddr_dtbo} ${dsi_dtbo0_10};fdt apply ${loadaddr_dtbo}\0" \
    "dsi_pancl1_7=fdt addr ${loadaddr_dtb};fdt resize 8192;${loadcmd} ${loadaddr_dtbo} ${dsi_dtbo1_7};fdt apply ${loadaddr_dtbo}\0" \
    "dsi_pancl1_10=fdt addr ${loadaddr_dtb};fdt resize 8192;${loadcmd} ${loadaddr_dtbo} ${dsi_dtbo1_10};fdt apply ${loadaddr_dtbo}\0" \
    "dsi_check0_7=if cmp.b ${dsi_mem0_get} ${dsi_mem0_chk} 1; then echo run DP; else echo run SERDES-0 7inch pancl;run dsi_pancl0_7;fi\0" \
    "dsi_check0_10=if cmp.b ${dsi_mem0_get} ${dsi_mem0_chk} 1; then echo run DP; else echo run SERDES-0 10inch pancl;run dsi_pancl0_10;fi\0" \
    "dsi_check1_7=if cmp.b ${dsi_mem1_get} ${dsi_mem1_chk} 1; then echo run HDMI; else echo run SERDES-1 7inch pancl;run dsi_pancl1_7;fi\0" \
    "dsi_check1_10=if cmp.b ${dsi_mem1_get} ${dsi_mem1_chk} 1; then echo run HDMI; else echo run SERDES-1 10inch pancl;run dsi_pancl1_10;fi\0" \
    "dsi_check0=if test X${dsi_panel0} = X10 ;then run dsi_check0_10;else run dsi_check0_7;fi\0" \
    "dsi_check1=if test X${dsi_panel1} = X10 ;then run dsi_check1_10;else run dsi_check1_7;fi\0" \
    "dsi_check_mcu=run dsi_out0;run dsi_out1;run dsi_i2c_set;run dsi_i2c_get;run dsi_check0;run dsi_check1;\0" \
    "dsi_check=run dsi_out0;run dsi_out1;run dsi_gpio_get;run dsi_check0;run dsi_check1;\0" \
	\
    "load_kernel=${loadcmd} ${loadaddr} ${file_kernel};${loadcmd} ${loadaddr_dtb} ${file_dtb}\0" \
    "ipaddr=192.168.0.20\0" \
    "serverip=192.168.0.1\0" \
    "serverfold=/export/rfs\0" \
    "bootargs_nfs=setenv bootargs rw root=/dev/nfs nfsroot=${serverip}:${serverfold},nfsvers=3 ip=dhcp cma=${cma_size},clk_ignore_unused\0" \
    "load_tftp=setenv loadcmd tftp\0" \
    "bootcmd_nfs=run bootargs_nfs;run load_tftp;run load_kernel;run pcie_check;run dsi_check;booti ${loadaddr} - ${loadaddr_dtb}\0" \
    "bootargs_mmc=setenv bootargs rw root=/dev/mmcblk0p2 rootfstype=ext4 rootwait cma=${cma_size},clk_ignore_unused\0" \
    "load_emmc=setenv loadcmd ext4load mmc 0:2\0" \
    "bootcmd_emmc=run bootargs_mmc;run load_emmc;run load_kernel;run pcie_check;run dsi_check;booti ${loadaddr} - ${loadaddr_dtb}\0" \
    "cma_size=900M\0"
    
#endif	/* __RCAR_GEN4_COMMON_H */
