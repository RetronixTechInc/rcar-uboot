/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * include/configs/raptor.h
 *     This file is Raptor board configuration.
 *
 * Copyright (C) 2021 Renesas Electronics Corp.
 */

#ifndef __RAPTOR_H
#define __RAPTOR_H

#include "rcar-gen4-common.h"

/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

/* Board Clock */
/* XTAL_CLK : 16.66MHz */
#define CONFIG_SYS_CLK_FREQ	16666666u

/* Generic Timer Definitions (use in assembler source) */
#define COUNTER_FREQUENCY	0xFE502A	/* 16.66MHz from CPclk */

#endif /* __RAPTOR_H */

