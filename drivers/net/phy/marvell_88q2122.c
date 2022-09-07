// SPDX-License-Identifier: GPL-2.0+
/*
 * Micrel PHY drivers
 *
 * Copyright 2010-2011 Freescale Semiconductor, Inc.
 * author Andy Fleming
 * (C) 2012 NetModule AG, David Andrey, added KSZ9031
 * (C) Copyright 2017 Adaptrum, Inc.
 * Written by Alexandru Gagniuc <alex.g@adaptrum.com> for Adaptrum, Inc.
 */
#include <common.h>
#include <dm.h>
#include <env.h>
#include <errno.h>
#include <micrel.h>
#include <phy.h>

#define msleep(a) udelay(a * 1000)

#define REGISTER13_ADDR		0x0d
#define REGISTER14_ADDR		0x0e
#define	DEVADDR_OFFSET		0x4000

#define MARVELL_PHY_ID_88Q2122			0x002B0980
#define MARVELL_PHY_ID_88Q2122_MASK		0xfffffff0

#define MRVL_Q212X_LPSD_FEATURE_ENABLE 0

static int m88q2122_startup(struct phy_device *phydev)
{
//	int ret;

//	ret = genphy_update_link(phydev);
//	if (ret)
//		return ret;

	phydev->duplex = DUPLEX_FULL;
	phydev->speed = SPEED_1000;
	phydev->pause = 0;
	phydev->asym_pause = 0;

	phydev->link = 1;

	return 0;
}

int m88q2122_phy_extended_write(struct phy_device *phydev, int devaddr, int regnum, u16 mode, u16 val)
{
	/*select register addr for mmd*/
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER13_ADDR, devaddr);
	/*select register for mmd*/
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER14_ADDR, regnum);
	/*setup mode*/
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER13_ADDR, (mode | devaddr));
	/*write the value*/
	return	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER14_ADDR, val);
}

int m88q2122_phy_extended_read(struct phy_device *phydev, int devaddr, int regnum, u16 mode)
{
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER13_ADDR, devaddr);
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER14_ADDR, regnum);
	phy_write(phydev, MDIO_DEVAD_NONE, REGISTER13_ADDR, (devaddr | mode));
	return phy_read(phydev, MDIO_DEVAD_NONE, REGISTER14_ADDR);
}

static int m88q2122_phy_extread(struct phy_device *phydev, int addr, int devaddr, int regnum)
{
	return m88q2122_phy_extended_read(phydev, devaddr, regnum, DEVADDR_OFFSET);
}

static int m88q2122_phy_extwrite(struct phy_device *phydev, int addr, int devaddr, int regnum, u16 val)
{
	return m88q2122_phy_extended_write(phydev, devaddr, regnum, DEVADDR_OFFSET, val);
}


// Write a 16-bit value into a register through SMI interface.
// @param phydev bootstrap address of the PHY
// @param devAddr section of register map (Ex: 0x07 : Auto-Neg registers)
// @param regnum register address within section
// @param val 16-bit to write into register
// @return void
//static void regWrite ( uint16_t phydev, uint16_t devAddr, uint16_t regAddr, uint16_t data );
static void regWrite ( struct phy_device *phydev, u16 devAddr, u16 regnum, u16 val )
{
	return m88q2122_phy_extended_write(phydev, devAddr, regnum, DEVADDR_OFFSET, val);
}

// Read out a 16-bit value from a register through SMI interface.
// @param phydev bootstrap address of the PHY
// @param devAddr section of register map (Ex: 0x07 : Auto-Neg registers)
// @param regnum register address within section
// @return value in register if successful
//static int regRead ( uint16_t phydev, uint16_t devAddr, uint16_t regAddr );
static int regRead ( struct phy_device *phydev, u16 devAddr, u16 regnum )
{
	return m88q2122_phy_extended_read(phydev, devAddr, regnum, DEVADDR_OFFSET);
}

// Set Master/Slave mode of the PHY by software
// @param phydev bootstrap address of the PHY
// @param forceMaster
// @return void
static void setMasterSlave(struct phy_device *phydev, bool forceMaster)
{
	u16 regData = 0;
	regData = regRead(phydev, 1, 0x0834);

	if (forceMaster)
	{
		printf("Marvell m88Q2122 Set Master ; ");
		regData |= 0x4000;
	}
	else
	{
		printf("Marvell m88Q2122 Set Slave ; ");
		regData &= 0xBFFF;
	}

	regWrite(phydev, 1, 0x0834, regData);
	if (forceMaster && MRVL_Q212X_LPSD_FEATURE_ENABLE)
	{
		regWrite(phydev, 7, 0x8032, 0x005A);
	}
	else
	{
		regWrite(phydev, 7, 0x8032, 0x0064);
	}
	regWrite(phydev, 7, 0x8031, 0x0A01);
	regWrite(phydev, 7, 0x8031, 0x0C01);
}

// Check current master/slave setting
// @param phydev address of the PHY
// @return true if master, false if slave
static bool isMaster(struct phy_device *phydev)
{
	return (0x0 != (regRead(phydev, 7, 0x8001) & 0x4000));
}

// Software Reset procedure
// @param phydev address of the PHY
// @return void
static void softReset(struct phy_device *phydev)
{
//	printf("SoftReset ");
	int regData = regRead(phydev, 1, 0x0000);

	regData |= 1 << 11;
	regWrite(phydev, 1, 0x0000, regData);
	regWrite(phydev, 3, 0xFFE4, 0x000C);
	msleep(100);
	regWrite(phydev, 3, 0xFFE4, 0x06B6);
	regData &= ~(1 << 11);

	regWrite(phydev, 1, 0x0000, regData);
	msleep(100);
	regWrite(phydev, 3, 0xFC47, 0x0030);
	regWrite(phydev, 3, 0xFC47, 0x0031);
	regWrite(phydev, 3, 0xFC47, 0x0030);
	regWrite(phydev, 3, 0xFC47, 0x0000);
	regWrite(phydev, 3, 0xFC47, 0x0001);
	regWrite(phydev, 3, 0xFC47, 0x0000);
	regWrite(phydev, 3, 0x0900, 0x8000);
	regWrite(phydev, 1, 0x0900, 0x0000);
	regWrite(phydev, 3, 0xFFE4, 0x000C);
}

// Initialize PHY
// @param phydev address of the PHY
// @return void
static int m88Q2122_init(struct phy_device *phydev)
{
	int regData = 0;

	setMasterSlave(phydev, 1);

	printf("config init ... ");

	msleep(200);
	regWrite(phydev, 1, 0x0900, 0x4000);
	regWrite(phydev, 7, 0x0200, 0x0000);

	regData = regRead(phydev, 1, 0x0834);
	regData = (regData & 0xFFF0) | 0x0001;
	regWrite(phydev, 1, 0x0834, regData);

	regWrite(phydev, 3, 0xFFE4, 0x07B5);
	regWrite(phydev, 3, 0xFFE4, 0x06B6);
	msleep(500);
	regWrite(phydev, 3, 0xFFDE, 0x402F);
	regWrite(phydev, 3, 0xFE2A, 0x3C3D);
	regWrite(phydev, 3, 0xFE34, 0x4040);
	regWrite(phydev, 3, 0xFE4B, 0x9337);
	regWrite(phydev, 3, 0xFE2A, 0x3C1D);
	regWrite(phydev, 3, 0xFE34, 0x0040);
	regWrite(phydev, 3, 0xFE0F, 0x0000);
	regWrite(phydev, 3, 0xFC00, 0x01C0);
	regWrite(phydev, 3, 0xFC17, 0x0425);
	regWrite(phydev, 3, 0xFC94, 0x5470);
	regWrite(phydev, 3, 0xFC95, 0x0055);
	regWrite(phydev, 3, 0xFC19, 0x08d8);
	regWrite(phydev, 3, 0xFC1A, 0x0110);
	regWrite(phydev, 3, 0xFC1B, 0x0a10);
	regWrite(phydev, 3, 0xFC3A, 0x2725);
	regWrite(phydev, 3, 0xFC61, 0x2627);
	regWrite(phydev, 3, 0xFC3B, 0x1612);
	regWrite(phydev, 3, 0xFC62, 0x1C12);
	regWrite(phydev, 3, 0xFC9D, 0x6367);
	regWrite(phydev, 3, 0xFC9E, 0x8060);
	regWrite(phydev, 3, 0xFC00, 0x01C8);
	regWrite(phydev, 3, 0x8000, 0x0000);
	regWrite(phydev, 3, 0x8016, 0x0011);
	regWrite(phydev, 3, 0xFDA3, 0x1800);
	regWrite(phydev, 3, 0xFE02, 0x00C0);
	regWrite(phydev, 3, 0xFFDB, 0x0010);
	regWrite(phydev, 3, 0xFFF3, 0x0020);
	regWrite(phydev, 3, 0xFE40, 0x00A6);
	regWrite(phydev, 3, 0xFE60, 0x0000);
	regWrite(phydev, 3, 0xFE2A, 0x3C3D);
	regWrite(phydev, 3, 0xFE4B, 0x9334);
	regWrite(phydev, 3, 0xFC10, 0xF600);
	regWrite(phydev, 3, 0xFC11, 0x073D);
	regWrite(phydev, 3, 0xFC12, 0x000D);
	regWrite(phydev, 3, 0xFC13, 0x0010);
	//LPSD feature
	if (MRVL_Q212X_LPSD_FEATURE_ENABLE){
		if (isMaster(phydev)){
			regWrite(phydev, 7, 0x8032, 0x005A);
		}
		else{
			regWrite(phydev, 7, 0x8032, 0x0064);
		}
		regWrite(phydev, 7, 0x8031, 0x0A01);
		regWrite(phydev, 7, 0x8031, 0x0C01);
		regWrite(phydev, 3, 0x800C, 0x0008);
		regWrite(phydev, 7, 0x8032, 0x0001);
		regWrite(phydev, 7, 0x8031, 0x0A1B);
		regWrite(phydev, 7, 0x8031, 0x0C1B);
		regWrite(phydev, 7, 0x8032, 0x000B);
		regWrite(phydev, 7, 0x8031, 0x0A1C);
		regWrite(phydev, 7, 0x8031, 0x0C1C);
		regWrite(phydev, 3, 0xFE04, 0x0016);
	}
	else{
		regWrite(phydev, 7, 0x8032, 0x0064);
		regWrite(phydev, 7, 0x8031, 0x0A01);
		regWrite(phydev, 7, 0x8031, 0x0C01);
		regWrite(phydev, 3, 0x800C, 0x0000);
		regWrite(phydev, 7, 0x8032, 0x0002);
		regWrite(phydev, 7, 0x8031, 0x0A1B);
		regWrite(phydev, 7, 0x8031, 0x0C1B);
		regWrite(phydev, 7, 0x8032, 0x0003);
		regWrite(phydev, 7, 0x8031, 0x0A1C);
		regWrite(phydev, 7, 0x8031, 0x0C1C);
		regWrite(phydev, 3, 0xFE04, 0x0008);
	}

	softReset(phydev);
	printf("End \n");
	return 0;
}


static int m88q2122_config(struct phy_device *phydev)
{
	int ret;

	m88Q2122_init(phydev);

	return genphy_config(phydev);
}

static struct phy_driver m88q2122_driver = {
	.name = "Marvell 88q2122",
	.uid  = 0x002B0980,
	.mask = 0xfffffff0,
	.features = PHY_GBIT_FEATURES,
	.config   = &m88q2122_config,
	.startup  = &m88q2122_startup,
	.shutdown = &genphy_shutdown,
	.writeext = &m88q2122_phy_extwrite,
	.readext = &m88q2122_phy_extread,
};


int phy_marvell_88q2122_init(void)
{
	phy_register(&m88q2122_driver);
	return 0;
}
