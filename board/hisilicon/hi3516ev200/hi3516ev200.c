/*
 * (C) Copyright 2015 Linaro
 * Peter Griffin <peter.griffin@linaro.org>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <spi_flash.h>
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <netdev.h>
#include <mmc.h>
#include <asm/sections.h>
#include <sdhci.h>

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
    /* Enable D-cache. I-cache is already enabled in start.S */
    dcache_enable();
}
#endif
static int boot_media = BOOT_MEDIA_UNKNOWN;
int get_boot_media(void)
{
    unsigned int reg_val, boot_mode, spi_device_mode;
    int boot_media = BOOT_MEDIA_UNKNOWN;

    reg_val = readl(SYS_CTRL_REG_BASE + REG_SYSSTAT);
    boot_mode = GET_SYS_BOOT_MODE(reg_val);

    switch (boot_mode) {
        case BOOT_FROM_SPI:
            spi_device_mode = GET_SPI_DEVICE_TYPE(reg_val);
            if (spi_device_mode) {
                boot_media = BOOT_MEDIA_NAND;
            } else {
                boot_media = BOOT_MEDIA_SPIFLASH;
            }
            break;
        case BOOT_FROM_EMMC:
            boot_media = BOOT_MEDIA_EMMC;
            break;
        default:
            boot_media = BOOT_MEDIA_UNKNOWN;
            break;
    }

    return boot_media;
}

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

#define COMP_MODE_ENABLE ((unsigned int)0x0000EAEF)

static inline void delay(unsigned long loops)
{
    __asm__ volatile ("1:\n"
                      "subs %0, %1, #1\n"
                      "bne 1b" : "=r" (loops) : "0" (loops));
}

/* get uboot start media. */
/*int get_boot_media(void)
{
    return boot_media;
}*/

int get_text_base(void)
{
    return CONFIG_SYS_TEXT_BASE;
}

static void boot_flag_init(void)
{
    unsigned int reg, boot_mode, spi_device_mode;

    /* get boot mode */
    reg = __raw_readl(SYS_CTRL_REG_BASE + REG_SYSSTAT);
    boot_mode = GET_SYS_BOOT_MODE(reg);

    switch (boot_mode) {
        case BOOT_FROM_SPI:
            spi_device_mode = GET_SPI_DEVICE_TYPE(reg);
            if (spi_device_mode) {
                boot_media = BOOT_MEDIA_NAND;
            } else {
                boot_media = BOOT_MEDIA_SPIFLASH;
            }
            break;
        case BOOT_FROM_EMMC:    /* emmc mode */
            boot_media = BOOT_MEDIA_EMMC;
            break;
        default:
            boot_media = BOOT_MEDIA_UNKNOWN;
            break;
    }
}

int board_early_init_f(void)
{
    return 0;
}

#define UBOOT_DATA_ADDR     (0x41000000)
#define UBOOT_DATA_SIZE     (0x80000)
int save_bootdata_to_flash(void)
{
    unsigned int sd_update_flag;
    void *buf = NULL;

    sd_update_flag = readl(REG_BASE_SCTL + REG_SC_GEN4);
    if (sd_update_flag == START_MAGIC) {
#if defined(CONFIG_HIFMC)
        static struct spi_flash *flash;
        unsigned int val;
        if (boot_media == BOOT_MEDIA_SPIFLASH) {
            flash = spi_flash_probe(0, 0, 1000000, 0x3);
            if (!flash) {
                printf("Failed to initialize SPI flash\n");
                return -1;
            }

            /* erase the address range. */
            printf("Spi flash erase...\n");
            val = flash->erase(flash, 0, UBOOT_DATA_SIZE);
            if (val) {
                printf("SPI flash sector erase failed\n");
                return 1;
            }

            buf = map_physmem((unsigned long)UBOOT_DATA_ADDR,
                              UBOOT_DATA_SIZE, MAP_WRBACK);
            if (!buf) {
                puts("Failed to map physical memory\n");
                return 1;
            }

            /* copy the data from RAM to FLASH */
            printf("Spi flash write...\n");
            val = flash->write(flash, 0, UBOOT_DATA_SIZE, buf);
            if (val) {
                printf("SPI flash write failed, return %d\n",
                       val);
                unmap_physmem(buf, UBOOT_DATA_SIZE);
                return 1;
            }

            unmap_physmem(buf, UBOOT_DATA_SIZE);
        }
        if (boot_media == BOOT_MEDIA_NAND) {
            struct mtd_info *nand_flash;
            size_t length = UBOOT_DATA_SIZE;

            nand_flash = nand_info[0];

            printf("Nand flash erase...\n");
            val = nand_erase(nand_flash, 0, UBOOT_DATA_SIZE);
            if (val) {
                printf("Nand flash erase failed\n");
                return 1;
            }

            buf = map_physmem((unsigned long)UBOOT_DATA_ADDR,
                              UBOOT_DATA_SIZE, MAP_WRBACK);
            if (!buf) {
                puts("Failed to map physical memory\n");
                return 1;
            }

            printf("Nand flash write...\n");
            val = nand_write(nand_flash, 0, &length, buf);
            if (val) {
                printf("Nand flash write failed, return %d\n",
                       val);
                unmap_physmem(buf, UBOOT_DATA_SIZE);
                return 1;
            }

            unmap_physmem(buf, UBOOT_DATA_SIZE);
        }
#endif
#if defined(CONFIG_SUPPORT_EMMC_BOOT)
        if (boot_media == BOOT_MEDIA_EMMC) {
            struct mmc *mmc = find_mmc_device(0);

            if (!mmc) {
                return 1;
            }

            (void)mmc_init(mmc);

            buf = map_physmem((unsigned long)UBOOT_DATA_ADDR,
                              UBOOT_DATA_SIZE, MAP_WRBACK);
            if (!buf) {
                puts("Failed to map physical memory\n");
                return 1;
            }

            printf("MMC write...\n");
            blk_dwrite(mmc_get_blk_desc(mmc), 0, (UBOOT_DATA_SIZE >> 9), buf);
            unmap_physmem(buf, UBOOT_DATA_SIZE);
        }
#endif

        printf("update success!\n");
    }

    return 0;
}

int auto_update_flag = 0;
int bare_chip_program = 0;

#define REG_BASE_GPIO0          0x120b0000
#define GPIO0_0_DATA_OFST       0x4
#define GPIO_DIR_OFST       0x400

int is_bare_program(void)
{
    return 1;
}

int is_auto_update(void)
{
#if (defined CONFIG_AUTO_SD_UPDATE) || (defined CONFIG_AUTO_USB_UPDATE)
    /* to add some judgement if neccessary */
    unsigned int  val[3];

    writel(REG_BASE_GPIO0 + GPIO_DIR_OFST, 0);

    val[0] = readl(REG_BASE_GPIO0 + GPIO0_0_DATA_OFST);
    if(val[0]) {
        return 0;
    }

    udelay(10000);
    val[1] = readl(REG_BASE_GPIO0 + GPIO0_0_DATA_OFST);
    udelay(10000);
    val[2] = readl(REG_BASE_GPIO0 + GPIO0_0_DATA_OFST);
    udelay(10000);

    if (val[0] == val[1] && val[1] == val[2] && val[0] == 0) {
        return 1;    /* update enable */
    } else {
        return 0;
    }

#else
    return 0;
#endif
}

void detect_memory(void) {
	ulong tested_ram = get_ram_size((long *)CONFIG_SYS_SDRAM_BASE, PHYS_SDRAM_1_SIZE)
		/ 1024 / 1024;
	printf("RAM size: %dMB\n", tested_ram);

	char msize[128];
	sprintf(msize, "%dM", tested_ram);
	setenv("totalmem", msize);
}

int misc_init_r(void)
{
#ifdef CONFIG_RANDOM_ETHADDR
    random_init_r();
#endif
    detect_memory();
    setenv("verify", "n");

#if (CONFIG_AUTO_UPDATE == 1)
    /* auto update flag */
    if(is_auto_update()) {
        auto_update_flag = 1;
    } else {
        auto_update_flag = 0;
    }

    /* bare chip program flag */
    if(is_bare_program()) {
        bare_chip_program = 1;
    } else {
        bare_chip_program = 0;
    }

#ifdef CFG_MMU_HANDLEOK
    dcache_stop();
#endif

#ifdef CFG_MMU_HANDLEOK
    dcache_start();
#endif

#endif

#if (CONFIG_AUTO_UPDATE == 1)
    int update_flag = -1;
    extern int do_auto_update(void);
    if (auto_update_flag) {
        update_flag = do_auto_update();
    }
    if (bare_chip_program && !auto_update_flag) {
        save_bootdata_to_flash();
    }
    extern int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
    if (update_flag == 0) {
        do_reset(NULL, 0, 0, NULL);
    }
#endif
    return 0;
}

int board_init(void)
{
    DECLARE_GLOBAL_DATA_PTR;

    gd->bd->bi_arch_number = MACH_TYPE_HI3516EV200;
    gd->bd->bi_boot_params = CFG_BOOT_PARAMS;

    boot_flag_init();

    return 0;
}

int dram_init(void)
{
    DECLARE_GLOBAL_DATA_PTR;

    gd->ram_size = PHYS_SDRAM_1_SIZE;
    return 0;
}

void reset_cpu(ulong addr)
{
    writel(0x12345678, REG_BASE_SCTL + REG_SC_SYSRES);
    while(1);
}

int timer_init(void)
{
    /*
     **Under uboot, 0xffffffff is set to load register,
     ** timer_clk = BUSCLK/2/256.
     ** e.g. BUSCLK = 50M, it will roll back after 0xffffffff/timer_clk
     ** = 43980s = 12hours
     **/
    __raw_writel(0, CFG_TIMERBASE + REG_TIMER_CONTROL);
    __raw_writel(~0, CFG_TIMERBASE + REG_TIMER_RELOAD);

    /*32 bit, periodic*/
    __raw_writel(CFG_TIMER_CTRL, CFG_TIMERBASE + REG_TIMER_CONTROL);

    return 0;
}

int board_eth_init(bd_t *bis)
{
    int rc = 0;

#ifdef CONFIG_NET_HISFV300
    rc = hieth_initialize(bis);
#endif
    return rc;
}

#ifdef CONFIG_GENERIC_MMC
extern int hisi_sdhci_add_port(int index, u32 regbase, u32 type);
extern int hisi_mmc_init(int index);

int board_mmc_init(bd_t *bis)
{
    int ret = 0;

#ifdef CONFIG_HISI_SDHCI

#ifndef CONFIG_HIFMC
    ret = hisi_sdhci_add_port(0, EMMC_BASE_REG, MMC_TYPE_MMC);
    if (!ret) {
        ret = hisi_mmc_init(0);
        if (ret) {
            printf("No EMMC device found !\n");
        }
    }
#else

#ifdef CONFIG_AUTO_SD_UPDATE
    ret = hisi_sdhci_add_port(0, SDIO0_BASE_REG, MMC_TYPE_SD);
    if (ret) {
        return ret;
    }

    ret = hisi_mmc_init(0);
    if (ret) {
        printf("No SD device found !\n");
    }
#endif

#endif
#endif

    return ret;
}
#endif
#ifdef CONFIG_ARMV7_NONSEC
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
}

void smp_kick_all_cpus(void)
{
}

void smp_waitloop(unsigned previous_address)
{
}
#endif

