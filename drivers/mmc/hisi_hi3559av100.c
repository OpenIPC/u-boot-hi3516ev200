/*
 * Copyright (c) 2017- 2018 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define EMMC_PHY_INIT_CTRL      (EMMC_PHY_BASE + 0x4)
#define PHY_INIT_EN             BIT(0)
#define PHY_DLYMEAS_EN          BIT(2)
#define PHY_ZCAL_EN             BIT(3)

#define EMMC_PHY_DLY_CTL1       (EMMC_PHY_BASE + 0x254)
#define PHY_INV_CLK             BIT(31)

#define EMMC_PHY_IOCTL_PUPD     (EMMC_PHY_BASE + 0x260)
#define RG_EMMC_PUPD_EN         0x7ff
#define RG_EMMC_PULL_UP         0x2ff

#define EMMC_PHY_IOCTL_RONSEL_1_0   (EMMC_PHY_BASE + 0x264)
#define RG_EMMC_RONSEL1             0x7ff
#define RG_EMMC_RONSEL0             0x7ff
#define EMMC_PHY_IOCTL_OD_RONSEL_2  (EMMC_PHY_BASE + 0x268)

#define EMMC_PHY_IOCTL_IOE      (EMMC_PHY_BASE + 0x26c)
#define DA_EMMC_IE              0x7ff
#define DA_EMMC_E               0x6ff

#define EMMC_RSTN_PULL_UP_EN	BIT(8)
#define PIN_SEL_EMMC_FUNC	0x1
#define PIN_SEL_EMMC_POWER_EN	0x2

#define REG_PERI_STAT	(SYS_CTRL_REG_BASE + 0x30)
#define EMMC_PULL_UP_DISABLE    BIT(13)

#define REG_MISC_CTRL17         (MISC_REG_BASE + MISC_CTRL17)
#define REG_MISC_CTRL18         (MISC_REG_BASE + MISC_CTRL18)
#define SDIO0_PWRSW_SEL_1V8		BIT(5)
#define SDIO0_PWR_EN			BIT(4)
#define SDIO0_IO_MODE_SEL_1V8	BIT(1)
#define SDIO0_PWR_CTRL_BY_MISC  BIT(0)

#define SDIO_SAMPL_DLL_SRST_REQ	(0x1 << 30)
#define SDIO_DRV_DLL_SRST_REQ	(0x1 << 29)
#define SDIO_CLK_EN		(0x1 << 28)
#define SDIO_SRST_REQ	(0x1 << 27)

#define REG_EMMC_DRV_DLL_CTRL	(CRG_REG_BASE + 0x1b0)
#define REG_SDIO0_DRV_DLL_CTRL  (CRG_REG_BASE + 0x1d4)
#define SDIO_DRV_PHASE_SEL_MASK (0x1f << 24)
#define sdio_drv_sel(phase)     ((phase) << 24)

#define REG_EMMC_DS_DLL_CTRL		(CRG_REG_BASE + 0x1b4)
#define EMMC_DS_DLL_MODE_SSEL       BIT(13)
#define EMMC_DS_DLL_SSEL_MASK       0x1fff
#define REG_EMMC_DS180_DLL_CTRL     (CRG_REG_BASE + 0x1b8)
#define EMMC_DS180_DLL_BYPASS       BIT(15)
#define REG_EMMC_DS_DLL_STATUS      (CRG_REG_BASE + 0x1c8)
#define EMMC_DS_DLL_LOCK            BIT(15)
#define EMMC_DS_DLL_MDLY_TAP_MASK   0x1fff

#define REG_EMMC_DRV_DLL_STATUS     (CRG_REG_BASE + 0x1c4)
#define REG_SDIO0_DRV_DLL_STATUS    (CRG_REG_BASE + 0x1e8)
#define SDIO_DRV_DLL_LOCK           BIT(15)

#define REG_EMMC_SAMPL_DLL_CTRL     (CRG_REG_BASE + 0x1a8)
#define REG_SDIO0_SAMPL_DLL_CTRL    (CRG_REG_BASE + 0x1ec)
#define SDIO_SAMPL_DLL_SLAVE_EN     BIT(16)

#define REG_EMMC_SAMPL_DLL_STATUS   (CRG_REG_BASE + 0x1bc)
#define REG_SDIO0_SAMPL_DLL_STATUS  (CRG_REG_BASE + 0x1e0)
#define SDIO_SAMPL_DLL_SLAVE_READY  BIT(14)

#define REG_EMMC_SAMPLB_DLL_CTRL    0x1ac
#define SDIO_SAMPLB_DLL_CLK_MASK    (0x1f << 24)
#define sdio_samplb_sel(phase)      ((phase) << 24)


#define EMMC_MUXCTRL_REG_START  (IO_CONFIG_REG_BASE + 0x2000)
#define EMMC_MUXCTRL_REG_END    (IO_CONFIG_REG_BASE + 0x201c)
#define EMMC_MUXCTRL_REG_RSTN   (IO_CONFIG_REG_BASE + 0x202c)
#define EMMC_MUXCTRL_REG_POWER_EN   (IO_CONFIG_REG_BASE + 0x2030)

#define CLK_100K	100000
#define CLK_400K	400000
#define CLK_25M		5000000
#define CLK_49P5M	50000000
#define CLK_99M		9000000
#define CLK_150M	150000000
#define CLK_187P5M	187000000
#define CLK_198M	198000000

#define SHIFT_16 	16

static void hisi_dll_reset_assert(struct sdhci_host *host)
{
	unsigned long crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ? REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg |= SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, crg_addr);
}

static void hisi_dll_reset_deassert(struct sdhci_host *host)
{
	unsigned long crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ? REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg &= ~(SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ);
	writel(reg, crg_addr);
}

static void hisi_set_crg(struct sdhci_host *host, unsigned int clk)
{
	unsigned long crg_addr;
	unsigned int sel, reg;
	unsigned int clk_mux[] = {
		CLK_100K, CLK_25M, CLK_49P5M, CLK_99M,
		CLK_187P5M, CLK_150M, CLK_198M, CLK_400K
		};

	crg_addr = (host->type == MMC_TYPE_MMC) ? REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg &= ~MMC_CLK_SEL_MASK;

	if (clk <= MIN_FREQ) {
		sel = 0x7;
	} else {
		for (sel = 0x6; sel > 0; sel--) {
			if (clk >= clk_mux[sel])
				break;
		}
	}

	reg |= MMC_CLK_SEL(sel);
	writel(reg, crg_addr);
}

static void hisi_wait_ds180_dll_ready(void)
{
	/* Do nothing */
}

static void hisi_wait_ds_dll_lock(void)
{
	unsigned int reg;
	unsigned int timeout = 20;

	do {
		reg = readl(REG_EMMC_DS_DLL_STATUS);
		if (reg & EMMC_DS_DLL_LOCK)
			return;

		udelay(1000); /* delay 1000us */
		timeout--;
	} while (timeout > 0);

	printf("DS DLL master not locked.\n");
}

static void hisi_set_ds_dll_delay(void)
{
#if !defined(CONFIG_TARGET_HI3519AV100) && !defined(CONFIG_TARGET_HI3556AV100)
	unsigned int reg, mdly_tap;

	reg = readl(REG_EMMC_DS_DLL_STATUS);
	mdly_tap = reg & EMMC_DS_DLL_MDLY_TAP_MASK;

	reg = readl(REG_EMMC_DS_DLL_CTRL);
	reg &= ~(EMMC_DS_DLL_SSEL_MASK);
	reg |= (mdly_tap / 4 + 12) | EMMC_DS_DLL_MODE_SSEL; /* ssel mode en and select suitable phase  mdly_tap / 4 + 12 */
	writel(reg, REG_EMMC_DS_DLL_CTRL);

	reg = readl(REG_EMMC_DS180_DLL_CTRL);
	reg |= EMMC_DS180_DLL_BYPASS;
	writel(reg, REG_EMMC_DS180_DLL_CTRL);
#endif
}

static void hisi_mmc_priv_init(struct sdhci_host *host)
{
	unsigned short ctrl;
	unsigned int reg;

	ctrl = sdhci_readw(host, SDHCI_MSHC_CTRL);
	ctrl &= ~SDHCI_CMD_CONFLIT_CHECK;
	sdhci_writew(host, ctrl, SDHCI_MSHC_CTRL);

	reg = sdhci_readl(host, SDHCI_AXI_MBIIU_CTRL);
	reg |= SDHCI_GM_WR_OSRC_LMT | SDHCI_GM_RD_OSRC_LMT;
	sdhci_writel(host, reg, SDHCI_AXI_MBIIU_CTRL);

	reg  = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	reg &= ~(SDHCI_DATA_DLY_EN | SDHCI_CMD_DLY_EN);
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

void mmc_muxctrl_config(void)
{
	unsigned int reg_addr, val;

	/* set emmc multiplexing */
	for (reg_addr = EMMC_MUXCTRL_REG_START;
			reg_addr <= EMMC_MUXCTRL_REG_END;
			reg_addr += 0x4) {

		val = readl((long)reg_addr);
		val &= ~0xf;
		val |= PIN_SEL_EMMC_FUNC;
		writel(val, (long)reg_addr);
	}

	val = readl(EMMC_MUXCTRL_REG_RSTN);
	val &= ~0xf;
	val |= PIN_SEL_EMMC_FUNC;
	val |= EMMC_RSTN_PULL_UP_EN;
	writel(val, EMMC_MUXCTRL_REG_RSTN);

	val = readl(EMMC_MUXCTRL_REG_POWER_EN);
	val &= ~0xf;
	val |= PIN_SEL_EMMC_POWER_EN;
	writel(val, EMMC_MUXCTRL_REG_POWER_EN);
}

int mmc_phy_init(void)
{
	unsigned int reg;
	unsigned int timeout = 10;

	mmc_muxctrl_config();

	/* emmc phy config */
	reg = readl(REG_MISC_CTRL17);
	reg &= ~UFS_EN;
	reg |= (DA_UFS_REFCLK_OEN | DA_UFS_RST_OEN);
	writel(reg, REG_MISC_CTRL17);

	reg = readl(REG_MISC_CTRL17);
	reg &= ~(EMMC_ISO_EN | RG_EMMC_LHEN_IN);
	writel(reg, REG_MISC_CTRL17);

	/* enable phy init */
	reg = readl(EMMC_PHY_INIT_CTRL);
	reg |= (PHY_INIT_EN | PHY_ZCAL_EN);
	reg &= (~PHY_DLYMEAS_EN);
	writel(reg, EMMC_PHY_INIT_CTRL);

	/* wait phy init and zcal done */
	for (;;) {
		reg = readl(EMMC_PHY_INIT_CTRL);
		if (!(reg & (PHY_INIT_EN | PHY_ZCAL_EN)))
			break;
		udelay(100); /* delay 100us */
		if (timeout-- == 0) {
			printf("wait mmc phy cal done time out\n");
			return -1;
		}
	}

	/* DRV50 */
	writel((RG_EMMC_RONSEL1 << SHIFT_16) | RG_EMMC_RONSEL0, EMMC_PHY_IOCTL_RONSEL_1_0);

	/* DRV50, set OD to 0 */
	writel(0, EMMC_PHY_IOCTL_OD_RONSEL_2);

	/* do not invert the TXCLK */
	reg = readl(EMMC_PHY_DLY_CTL1);
	reg &= ~PHY_INV_CLK;
	writel(reg, EMMC_PHY_DLY_CTL1);

	writel((DA_EMMC_IE << SHIFT_16) | DA_EMMC_E, EMMC_PHY_IOCTL_IOE);

	/* enable pull up */
	reg = readl(REG_PERI_STAT);
	if (!(reg & EMMC_PULL_UP_DISABLE))
		writel((RG_EMMC_PULL_UP << SHIFT_16) | RG_EMMC_PUPD_EN, EMMC_PHY_IOCTL_PUPD);
	else
		writel((RG_EMMC_PULL_UP << SHIFT_16) & (~RG_EMMC_PUPD_EN), EMMC_PHY_IOCTL_PUPD);

	return 0;
}

static void sd_hardware_init(void)
{
	unsigned int reg, ctrl;

	/* clk enable */
	reg = readl(REG_SDIO0_CRG);
	reg |= SDIO_CLK_EN;
	writel(reg, REG_SDIO0_CRG);

	/* reset assert */
	reg = readl(REG_SDIO0_CRG);
	reg |= SDIO_SRST_REQ | SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, REG_SDIO0_CRG);
	udelay(25); /* delay 25us */

	/* reset deassert */
	reg &= ~SDIO_SRST_REQ;
	writel(reg, REG_SDIO0_CRG);
	udelay(1); /* delay 1us */

	ctrl = readl(REG_MISC_CTRL18);
	ctrl |= SDIO0_PWR_EN | SDIO0_PWR_CTRL_BY_MISC;
	ctrl &= ~(SDIO0_IO_MODE_SEL_1V8 | SDIO0_PWRSW_SEL_1V8);
	writel(ctrl, REG_MISC_CTRL18);

	udelay(5000); /* delay 5000us */
}

static int emmc_hardware_init(void)
{
	unsigned int reg;
	unsigned int timeout = 10;

	mmc_muxctrl_config();

	/* emmc phy config */
	reg = readl(REG_MISC_CTRL17);
	reg &= ~UFS_EN;
	reg |= (DA_UFS_REFCLK_OEN | DA_UFS_RST_OEN);
	writel(reg, REG_MISC_CTRL17);

	reg = readl(REG_MISC_CTRL17);
	reg &= ~(EMMC_ISO_EN | RG_EMMC_LHEN_IN);
	writel(reg, REG_MISC_CTRL17);

	/* enable phy init */
	reg = readl(EMMC_PHY_INIT_CTRL);
	reg |= (PHY_INIT_EN | PHY_ZCAL_EN);
	reg &= (~PHY_DLYMEAS_EN);
	writel(reg, EMMC_PHY_INIT_CTRL);

	/* wait phy init and zcal done */
	for (;;) {
		reg = readl(EMMC_PHY_INIT_CTRL);
		if (!(reg & (PHY_INIT_EN | PHY_ZCAL_EN)))
			break;
		udelay(100); /* delay 100us */
		if (timeout-- == 0) {
			printf("wait mmc phy cal done time out\n");
			return -1;
		}
	}

	/* DRV50 */
	writel((RG_EMMC_RONSEL1 << SHIFT_16) | RG_EMMC_RONSEL0, EMMC_PHY_IOCTL_RONSEL_1_0);

	/* DRV50, set OD to 0 */
	writel(0, EMMC_PHY_IOCTL_OD_RONSEL_2);

	/* do not invert the TXCLK */
	reg = readl(EMMC_PHY_DLY_CTL1);
	reg &= ~PHY_INV_CLK;
	writel(reg, EMMC_PHY_DLY_CTL1);

	writel((DA_EMMC_IE << SHIFT_16) | DA_EMMC_E, EMMC_PHY_IOCTL_IOE);

	/* enable pull up */
	reg = readl(REG_PERI_STAT);
	if (!(reg & EMMC_PULL_UP_DISABLE))
		writel((RG_EMMC_PULL_UP << SHIFT_16) | RG_EMMC_PUPD_EN, EMMC_PHY_IOCTL_PUPD);
	else
		writel((RG_EMMC_PULL_UP << SHIFT_16) & (~RG_EMMC_PUPD_EN), EMMC_PHY_IOCTL_PUPD);

	/* eMMC clk enable */
	reg = readl(REG_EMMC_CRG);
	reg |= SDIO_CLK_EN;
	writel(reg, REG_EMMC_CRG);

	/* eMMC reset assert */
	reg = readl(REG_EMMC_CRG);
	reg |= SDIO_SRST_REQ | SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, REG_EMMC_CRG);
	udelay(25); /* delay 25us */

	/* eMMC reset deassert */
	reg &= ~SDIO_SRST_REQ;
	writel(reg, REG_EMMC_CRG);
	udelay(1); /* delay 1us */

	return 0;
}

static void hisi_mmc_set_ioconfig(struct sdhci_host *host)
{
	unsigned int reg;

	if (host->type == MMC_TYPE_MMC) {
		if (host->mmc->timing == MMC_TIMING_MMC_HS200
				|| host->mmc->timing == MMC_TIMING_MMC_HS400
				|| host->mmc->timing == MMC_TIMING_MMC_HS) {
			reg = sdhci_readw(host, SDHCI_EMMC_CTRL);
			reg |= SDHCI_CARD_IS_EMMC;
			sdhci_writew(host, reg, SDHCI_EMMC_CTRL);

			/* set drv strength to 50ohm */
			writel(0x0, EMMC_PHY_IOCTL_RONSEL_1_0);
			writel(0x6ff, EMMC_PHY_IOCTL_OD_RONSEL_2);
		}
	}
}

static void hisi_set_phase(struct sdhci_host *host)
{
	unsigned int drv_phase, sample_phase;

	if (host->mmc->timing == MMC_TIMING_MMC_HS400) {
		drv_phase = 8;    /* 8 for 90 degree */
		sample_phase = host->tuning_phase;
	} else if (host->mmc->timing == MMC_TIMING_MMC_HS200) {
		drv_phase = 20;   /* 20 for 225 degree */
		sample_phase = host->tuning_phase;
	} else {
		drv_phase = 16;   /* 16 for  180 degree */
		sample_phase = 0; /* 0 for 0 degree */
	}

	hisi_set_drv_phase(host, drv_phase);
	hisi_enable_sample(host);
	hisi_set_sampl_phase(host, sample_phase);

	udelay(25); /* delay 25us */
}
