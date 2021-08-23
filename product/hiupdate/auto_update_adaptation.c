/*
 * (C) Copyright 2003
 * Gary Jennejohn, DENX Software Engineering, gj@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <environment.h>
#include <command.h>
#include <malloc.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <linux/mtd/mtd.h>
#include <fat.h>
#include <console.h>
#include <mmc.h>
#include <sparse_format.h>
#if (CONFIG_AUTO_UPDATE == 1)  /* cover the whole file */

#ifdef CONFIG_AUTO_SD_UPDATE
#ifndef CONFIG_MMC
#error "should have defined CONFIG_MMC"
#endif
#include <mmc.h>
#include "mmc_init.c"
#endif

#if defined CONFIG_AUTO_USB_UPDATE
#ifndef CONFIG_HISI_MC
#if !defined CONFIG_USB_OHCI && !defined CONFIG_USB_XHCI_HCD
#error "should have defined CONFIG_USB_OHCI or CONFIG_USB_XHCI"
#endif
#endif
#ifndef CONFIG_USB_STORAGE
#error "should have defined CONFIG_USB_STORAGE"
#endif
#include <usb.h>
#include "usb_init.c"
#endif

#undef AU_DEBUG
#undef debug
#ifdef  AU_DEBUG
#define debug(fmt, args...) printf(fmt, ##args)
#else
#define debug(fmt, args...)
#endif  /* AU_DEBUG */

/* possible names of files on the medium. */
#define AU_CONFIG   "config"
/* config file's size < 1K */
#define CONFIG_MAX_SIZE  2048

#ifdef CONFIG_HISI_UPGRADE_BY_SEGMENT
#define SECTION_SIZE         (0x1000000)   /* 16M  */
#define BLOCK_SIZE           512
#define DEFAULT_BLOCK_SIZE   4096
#define CHUNK_HEAD_SIZE      12
#define BOUNDARY             (DEFAULT_BLOCK_SIZE + CHUNK_HEAD_SIZE)
#define HEX_DUMP_LEN         128
unsigned long map_size;
#endif

#define AU_FIRMWARE "u-boot.bin"
#define AU_KERNEL   "kernel"

int boot_medium_type = 0xff;

#include "nand.h"
struct flash_layout {
    long start;
    long end;
};

struct update_medium_interface {
    char name[20];
    int (*init)(void);
    int (*erase)(unsigned long offset, unsigned long len);
    int (*write)(unsigned long offset, unsigned long len, void *buf);
    int (*write_yaffs)(unsigned long offset, unsigned long len, void *buf);
    int (*write_ext4)(unsigned long offset, unsigned long len, void *buf);
};

#if defined(CONFIG_CMD_SF) || defined(CONFIG_CMD_NAND)
static void schedule_notify(unsigned long offset, unsigned long len,
                            unsigned long off_start)
{
    int percent_complete = -1;

    do {
        unsigned long long n = (unsigned long long)
                                (offset - off_start) * 100;
        int percent;

        do_div(n, len);
        percent = (int)n;

        /* output progress message only at whole percent
         * steps to reduce the number of messages
         * printed on (slow) serial consoles
         */
        if (percent != percent_complete) {
            printf("\rOperation at 0x%lx -- %3d%% complete.",
                   offset, percent);
        }
    } while (0);
}
#endif

#ifdef CONFIG_CMD_SF
static struct spi_flash *spinor_flash;
static int spinor_flash_init(void)
{
    spinor_flash = spi_flash_probe(0, 0, 0, 0);
    return 0;
}

static int spi_flash_erase_op(struct spi_flash *flash, unsigned long offset,
                              unsigned long len)
{
    int ret;
    struct mtd_info_ex *spiflash_info = get_spiflash_info();
    unsigned long erase_start, erase_len, erase_step;

    erase_start = offset;
    erase_len   = len;
    erase_step  = spiflash_info->erasesize;

    while (len > 0) {
        if (len < erase_step) {
            erase_step = len;
        }

        ret = flash->erase(flash, (u32)offset, erase_step);
        if (ret) {
            return 1;
        }

        len -= erase_step;
        offset += erase_step;
        /* notify real time schedule */
        schedule_notify(offset, erase_len, erase_start);
    }

    return ret;
}

static int spinor_flash_erase(unsigned long offset, unsigned long len)
{
    return spi_flash_erase_op(spinor_flash, offset, len);
}

static int spi_flash_write_op(struct spi_flash *flash, unsigned long offset,
                              unsigned long len, void *buf)
{
    int ret = 0;
    unsigned long write_start, write_len, write_step;
    char *pbuf = buf;
    struct mtd_info_ex *spiflash_info = get_spiflash_info();

    write_start = offset;
    write_len   = len;
    write_step  = spiflash_info->erasesize;

    while (len > 0) {
        if (len < write_step) {
            write_step = len;
        }

        ret = flash->write(flash, offset, write_step, pbuf);
        if (ret) {
            break;
        }

        offset += write_step;
        pbuf   += write_step;
        len    -= write_step;
        /* notify real time schedule */
        schedule_notify(offset, write_len, write_start);
    }

    return ret;
}

static int spinor_flash_write(unsigned long offset, unsigned long len, void *buf)
{
    return spi_flash_write_op(spinor_flash, offset, len, buf);
}
#endif

#ifdef CONFIG_CMD_NAND
struct mtd_info *nand_flash;
extern int nand_block_isbad(struct mtd_info *mtd, loff_t offs);

static int nand_flash_init(void)
{
    nand_flash = nand_info[0];
    return 0;
}

static int nand_flash_erase(unsigned long offset, unsigned long len)
{
    int ret;
    unsigned long erase_len;
    unsigned long erase_step;
    unsigned long length;
    nand_erase_options_t opts;

    memset(&opts, 0, sizeof(opts));

    length = len;
    erase_step = nand_flash->erasesize;
    erase_len = length;
    opts.length  = erase_step;
    opts.offset = offset;
    opts.quiet = 1;

    while (length > 0) {
        if (length < erase_step) {
            erase_step = length;
        }

        ret = nand_erase_opts(nand_flash, &opts);
        if (ret) {
            return 1;
        }

        length -= erase_step;
        opts.offset += erase_step;
        /* notify real time schedule */
        schedule_notify(opts.offset, erase_len, offset);
    }

    return ret;
}

static int nand_flash_write(unsigned long offset, unsigned long len,
                            void *buf)
{
    int ret = 0;
    unsigned long offset_notify, write_start, write_len, write_step;
    char *pbuf = buf;
    size_t length;
    if (offset == 0) {
        /* Make sure the length is block size algin */
        length = len & (nand_flash->erasesize - 1) ? (size_t)(len +
                 (nand_flash->erasesize - len % nand_flash->erasesize)) : len;
        write_step  = nand_flash->erasesize;
    } else {
        /* Make sure the length is writesize algin */
        length = len & (nand_flash->writesize - 1) ? (size_t)(len +
                 (nand_flash->writesize - len % nand_flash->writesize)) : len;
        write_step  = nand_flash->writesize;
    }

    write_start = offset;
    offset_notify = offset;
    write_len   = length;

    while (length > 0) {
        size_t block_offset = offset & (nand_flash->erasesize - 1);
        size_t *rw_size;

        if (nand_flash->_block_isbad(nand_flash,
            offset & ~((loff_t)nand_flash->erasesize - 1))) {
            printf("Skip bad block 0x%08llx\n",
                   offset & ~((loff_t)nand_flash->erasesize - 1));
            offset += nand_flash->erasesize - block_offset;
            continue;
        }

        rw_size = (size_t *)&write_step;

        ret = nand_flash->_write(nand_flash, (size_t)offset, *rw_size,
                                 rw_size, (u_char *)pbuf);
        if (ret) {
            printf("NAND write to offset %lx failed %d\n",
                   offset, ret);
            break;
        }

        offset += write_step;
        pbuf   += write_step;
        length -= write_step;
        offset_notify += write_step;
        /* notify real time schedule */
        schedule_notify(offset_notify, write_len, write_start);
    }

    return ret;
}

static int nand_flash_yaffs_write(unsigned long offset, unsigned long len,
                                  void *buf)
{
    int ret = 0;
    size_t rw_size = len;

    ret = nand_write_yaffs_skip_bad(nand_flash, offset, &rw_size,
                                    (u_char *)buf);
    if (ret) {
        printk("Write yaffs fail !!\n");
    }
    printk("Write yaffs done\n");

    return ret;
}

/*get count of area's bad block for nand flash*/
int get_bad_block_count(unsigned long offset, unsigned long len)
{
    int count = 0;
    unsigned long block_offset = 0;
    if (offset & (nand_flash->erasesize - 1)){
	block_offset = offset & (nand_flash->erasesize - 1);
    }

    if (len & (nand_flash->erasesize - 1)){
	len = (len + nand_flash->erasesize)&~(nand_flash->erasesize - 1);
    }

    for (int i = 0;i < len/nand_flash->erasesize;i++){
	if (nand_block_isbad(nand_flash,offset)){
	    count++;
	}
	offset += nand_flash->erasesize - block_offset;
    }
    return count;
}
#endif

void hex_dump(unsigned char *buf,int len,int addr) {

    int i,j,k;
    char binstr[80];

    for (i=0;i<len;i++) {
	if (0==(i%16)) {
	    sprintf(binstr,"%08x -",i+addr);
	    sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
	} else if (15==(i%16)) {
	    sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
	    sprintf(binstr,"%s  ",binstr);
	    for (j=i-15;j<=i;j++) {
		sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
	    }
	    printf("%s\n",binstr);
	} else {
	    sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
	}
    }

    if (0!=(i%16)) {
	k=16-(i%16);
	for (j=0;j<k;j++) {
	    sprintf(binstr,"%s   ",binstr);
	}
	sprintf(binstr,"%s  ",binstr);
	k=16-k;
	for (j=i-k;j<i;j++) {
	    sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
	}
	printf("%s\n",binstr);
    }
}

#ifdef CONFIG_SUPPORT_EMMC_BOOT
static int mmc_save_init(void)
{
    struct mmc *mmc = find_mmc_device(0);

    if (!mmc) {
        printf("%s:find mmc device failed\n", __func__);
        return -1;
    }

    (void)mmc_init(mmc);

    return 0;
}

static int mmc_save_write(unsigned long offset, unsigned long len, void *buf)
{
    struct mmc *mmc = find_mmc_device(0);

    if (!mmc) {
        printf("%s:find mmc device failed\n", __func__);
        return -1;
    }
    if (len % MMC_MAX_BLOCK_LEN) {
        blk_dwrite(mmc_get_blk_desc(mmc), (offset >> 9), (len >> 9) + 1, buf);
    } else {
        blk_dwrite(mmc_get_blk_desc(mmc), (offset >> 9), (len >> 9), buf);
    }

    return 0;
}
#ifndef CONFIG_HISI_UPGRADE_BY_SEGMENT
static int mmc_save_write_ext4(unsigned long offset, unsigned long len, void *buf)
{
    struct mmc *mmc = find_mmc_device(0);
    int retlen;

    if (!mmc) {
        printf("%s:find mmc device failed\n", __func__);
        return -1;
    }

    extern int ext4_unsparse(struct mmc * mmc, u32 dev, const u8 * pbuf, u32 blk, u32 cnt);
    if (len % MMC_MAX_BLOCK_LEN) {
        retlen = ext4_unsparse(mmc, 0, buf, (offset >> 9), (len >> 9) + 1);
    } else {
        retlen = ext4_unsparse(mmc, 0, buf, (offset >> 9), (len >> 9));
    }
    return retlen;
}
#endif
#endif

#define UPDATE_MEDIUM_SPINOR 0
#define UPDATE_MEDIUM_NAND   1
#define UPDATE_MEDIUM_EMMC   2


static struct update_medium_interface update_intf[3] = {
#ifdef CONFIG_CMD_SF
    {"spinor", spinor_flash_init, spinor_flash_erase, spinor_flash_write, NULL, NULL},
#else
    {"none", NULL, NULL, NULL, NULL, NULL},
#endif

#ifdef CONFIG_CMD_NAND
    {"nand", nand_flash_init, nand_flash_erase, nand_flash_write, nand_flash_yaffs_write, NULL},
#else
    {"none",  NULL,  NULL, NULL, NULL, NULL},
#endif
#ifdef CONFIG_SUPPORT_EMMC_BOOT
#ifndef CONFIG_HISI_UPGRADE_BY_SEGMENT
    {"emmc", mmc_save_init, NULL, mmc_save_write, NULL,  mmc_save_write_ext4}
#else
    {"emmc", mmc_save_init, NULL, mmc_save_write, NULL,  mmc_save_write}
#endif
#endif
};

static struct update_medium_interface *update_intf_p;

struct medium_interface {
    char name[20];
    int (*init) (void);
    void (*exit) (void);
};

#define MAX_UPDATE_INTF 3
static struct medium_interface s_intf[MAX_UPDATE_INTF] = {
#ifdef CONFIG_AUTO_SD_UPDATE
    { .name = "mmc", .init = mmc_stor_init, .exit = mmc_stor_exit, },
#endif
#ifdef CONFIG_AUTO_USB_UPDATE
    { .name = "usb", .init = usb_stor_init, .exit = usb_stor_exit, },
#endif
};

static int au_stor_curr_dev; /* current device */

/* index of each file in the following arrays */
#define IDX_FIRMWARE    0
#define IDX_KERNEL  1
#define IDX_ROOTFS  2

/* max. number of files which could interest us */
#define AU_MAXFILES 32

/* pointers to file names */
char *aufile[AU_MAXFILES] = {
    0
};

#define NAME_MAX_LEN 0x20
char aufile_table[AU_MAXFILES][NAME_MAX_LEN] = {{0,},};
unsigned long aufile_size[AU_MAXFILES] = {0};

/* sizes of flash areas for each file */
long ausize[AU_MAXFILES] = {0};

/* array of flash areas start and end addresses */
struct flash_layout aufl_layout[AU_MAXFILES] = {
    { 0,    0, }
};

#ifdef CONFIG_HISI_UPGRADE_BY_SEGMENT
typedef struct buff_layout {
	unsigned long offset;	/* chunk header offset in next buffer */
	u32 count;              /* chunk header count */
	u32 remain;             /* remain + offset = chunk_len */
	u32 len;                /* all the data lengrh we need to handle in buffer */
	u32 type;               /* the last chunk type */
	u8  flag;               /* 0: filesize < SECTION_SIZE; 1: filesize > SECTION_SIZE*/
}buff_layout_t;

buff_layout_t layout;           /* current buffer chunk information */
buff_layout_t perv_layout;      /* the previous buffer chunk information */
sparse_header_t sparse_header;  /* Sparse file header information */

#if (defined CONFIG_EMMC) || (defined CONFIG_CMD_UFS)
extern int get_unspare_header_info(const u8 *pbuf,sparse_header_t *sparse_header);
extern void print_chunk_info(chunk_header_t *chunk);

int get_buffer_chunk_layout(unsigned long offset,unsigned char *pbuf,unsigned long filesize, unsigned long pos)
{
    chunk_header_t *chunk = NULL;
    unsigned long off = 0;
    unsigned long chunk_len = 0;

    off = offset;

    /* big chunk handle */
    if (off > SECTION_SIZE){
        layout.offset = off - SECTION_SIZE;
        layout.len    = SECTION_SIZE;
        layout.type   = layout.type;
        layout.count  = 1;
        layout.remain = SECTION_SIZE;
        debug("big chunk: layout.count:%u,layout.remain:%u,layout.offset:%lu,layout.len:%u,layout.type:%#x\n", \
	    layout.count,layout.remain,layout.offset,layout.len,layout.type);
        return 0;
    }

    memset(&layout,0,sizeof(buff_layout_t));

    if (filesize <= SECTION_SIZE){
        layout.flag = 0;
    }else{
	layout.flag = 1;
    }

    do {
	chunk = (chunk_header_t *)(pbuf + off);
	print_chunk_info(chunk);
	if((chunk->chunk_type == CHUNK_TYPE_RAW) || (chunk->chunk_type == CHUNK_TYPE_DONT_CARE) || (chunk->chunk_type == CHUNK_TYPE_FILL)){
	    chunk_len = chunk->chunk_sz * sparse_header.blk_sz;
	    off += chunk->total_sz;

	    if ((off <= SECTION_SIZE - 1)){
		layout.count++;
		layout.len = off;

	    }else{
		/*  Special case handle: just have only one chunk header in buffer */
	        if (layout.count == 0){
		    layout.len = offset;
		}

		if (((SECTION_SIZE - 1 - layout.len) >= BOUNDARY)){
		    layout.count += 1;
		    layout.remain = SECTION_SIZE - 1 - layout.len - sparse_header.chunk_hdr_sz;
		    layout.remain = layout.remain &~(DEFAULT_BLOCK_SIZE - 1);
		    layout.offset = chunk_len - layout.remain;
		    layout.len 	  += layout.remain + sparse_header.chunk_hdr_sz;
		    layout.type   = chunk->chunk_type;
		}

	    }
	}else{
	    printf("This type of chunk:%x is not supported\n",chunk->chunk_type);
	    hex_dump(pbuf + off,HEX_DUMP_LEN,0);
	    return 1;
	}

    }while(((pos + layout.len) != filesize) && ((SECTION_SIZE - layout.len > DEFAULT_BLOCK_SIZE)));
    printf("layout.count:%u,layout.remain:%u,layout.offset:%lu,layout.len:%u,layout.type:%#x\n", \
	    layout.count,layout.remain,layout.offset,layout.len,layout.type);
    return 0;

}

#endif
#endif

/* where to load files into memory */
#if defined(CONFIG_TARGET_HI3559AV100) || defined(CONFIG_TARGET_HI3516EV200) || defined(CONFIG_TARGET_HI3516EV300) || defined(CONFIG_TARGET_HI3518EV300) || defined(CONFIG_TARGET_HI3516DV200)
#define LOAD_ADDR ((unsigned char *)0x42000000)
#elif defined(CONFIG_TARGET_HI3519AV100) || defined(CONFIG_TARGET_HI3556AV100)
#define LOAD_ADDR ((unsigned char *)0x22000000)
#else
#define LOAD_ADDR ((unsigned char *)0x83000000)
#endif
/* the app is the largest image */
#define MAX_LOADSZ ausize[IDX_ROOTFS]

#ifndef CONFIG_HISI_UPGRADE_BY_SEGMENT
static int au_do_update(int idx, long sz)
{
    unsigned long start, len;
    unsigned long write_len;
    int rc = 0;
    void *buf;
    char *pbuf;

    start = aufl_layout[idx].start;
    len = aufl_layout[idx].end - aufl_layout[idx].start + 1;

    /*
     * erase the address range.
     */
    if (boot_medium_type == BOOT_FROM_SPI || boot_medium_type == BOOT_FROM_NAND) {
        printf("%s erase...\n", update_intf_p->name);
        rc = update_intf_p->erase(start, len);
        if (rc) {
            printf("sector erase failed\n");
            return 1;
        }
    }

    buf = map_physmem((unsigned long)LOAD_ADDR, len, MAP_WRBACK);
    if (!buf) {
        puts("Failed to map physical memory\n");
        return 1;
    }

    /*
     * write the whole file to flash;uboot and rootfs image have head
     * kernel has head,it's head also be writed to flash
     */
    pbuf = buf;
    write_len = aufile_size[idx];

    /* copy the data from RAM to FLASH */
    printf("\n%s write...\n", update_intf_p->name);

    if (strstr(aufile[idx], "yaffs")) {
        if (update_intf_p->write_yaffs) {
            rc = update_intf_p->write_yaffs(start, write_len, pbuf);
        }
    } else if (strstr(aufile[idx], "ext4")) {
        if (update_intf_p->write_ext4) {
            rc = update_intf_p->write_ext4(start, len, pbuf);
        }
    } else {
        rc = update_intf_p->write(start, write_len, pbuf);
    }

    if (rc) {
	printf("write failed, return %d\n", rc);
	return 1;
    }

    unmap_physmem(buf, len);
    return 0;
}
#else
static int au_do_update(int idx, unsigned int sz, int segment, u32 offset, u16 type)
{
    int rc = 0;
    void *buf;
    char *pbuf;
#ifdef CONFIG_CMD_NAND
    int bad_block_count = 0;
    unsigned long tmp_start_addr = 0;
    unsigned long pages_len = 0;
#endif

    if (segment > 0){
        goto write_op;
    }

    if (boot_medium_type == BOOT_FROM_SPI || boot_medium_type == BOOT_FROM_NAND) {
	printf("%s erase...\n", update_intf_p->name);

	rc = update_intf_p->erase(aufl_layout[idx].start, (aufl_layout[idx].end - aufl_layout[idx].start + 1));
	if (rc) {
	    printf("sector erase failed\n");
	    return 1;
	}
    }

write_op:
    buf = map_physmem((unsigned long)LOAD_ADDR, map_size, MAP_WRBACK);
    if (!buf) {
	puts("Failed to map physical memory\n");
	return 1;
    }

    /*
     * write the whole file to flash;uboot and rootfs image have head
     * kernel has head,it's head also be writed to flash
     */
    pbuf = buf;

    /* copy the data from RAM to FLASH */
    printf("%s write...\n", update_intf_p->name);

    if (strstr(aufile[idx], "yaffs")) {
#ifdef CONFIG_CMD_NAND
	bad_block_count = 0;
	tmp_start_addr = aufl_layout[idx].start;
	if (update_intf_p->write_yaffs) {
	    pages_len = (sz / (nand_flash->writesize + nand_flash->oobsize)) * nand_flash->writesize;
	    bad_block_count = get_bad_block_count(tmp_start_addr, pages_len);
	    debug("write offset:%ld, bad block count:%d\n",aufl_layout[idx].start,bad_block_count);

	    tmp_start_addr += pages_len + bad_block_count * nand_flash->erasesize;

	    if (tmp_start_addr > aufl_layout[idx].end){
                printf("The actual length of the partition is greater than the specified \
			value due to the address offset caused by bad blocks.\n");
		return 1;
	    }
	    rc = update_intf_p->write_yaffs(aufl_layout[idx].start, sz, pbuf);
	    aufl_layout[idx].start += pages_len + bad_block_count * nand_flash->erasesize;
	}
#endif
    } else if (strstr(aufile[idx], "ext4")) {
	u32 write_len,chunk_len;
	if (update_intf_p->write_ext4) {
	    if (offset && (type == CHUNK_TYPE_RAW)){
		if (offset < SECTION_SIZE){
		    write_len = offset;
		    rc = update_intf_p->write_ext4(aufl_layout[idx].start, write_len, pbuf);
		    aufl_layout[idx].start += write_len;
		    pbuf += write_len;

		}else{
		    /* big chunk write */
		    write_len = SECTION_SIZE;
		    rc = update_intf_p->write_ext4(aufl_layout[idx].start, write_len, pbuf);
		    aufl_layout[idx].start += write_len;
		    return 0;
		}

	    }else{
		/* Skip the sparse header */
		pbuf += offset;
	    }
	    for (int chunk_count = 0;chunk_count < layout.count;chunk_count++){
		chunk_header_t *chunk = NULL;
		chunk = (chunk_header_t *)(pbuf);
		pbuf += sparse_header.chunk_hdr_sz;
		chunk_len = chunk->chunk_sz * sparse_header.blk_sz;
		switch (chunk->chunk_type) {
		    case CHUNK_TYPE_RAW:
			if (!layout.flag){
			    write_len = chunk_len;
			}else{
			    if (chunk_count == layout.count - 1){
				write_len = layout.remain;
				debug("layout.remian:%u, line:%d\n",layout.remain,__LINE__);
			    }else{
				write_len = chunk_len;
			    }

			}
			rc = update_intf_p->write_ext4(aufl_layout[idx].start, write_len, pbuf);
			aufl_layout[idx].start += write_len;
			pbuf += write_len;
			break;

		    case CHUNK_TYPE_DONT_CARE:
                        aufl_layout[idx].start += chunk_len;
			debug("CHUNK_TYPE_DONT_CARE: chunk_len:%u,chunk->total_size:%u,pbuf:%p,\
				line:%d\n",chunk_len,chunk->total_sz,pbuf,__LINE__);
			break;

		    default:
			printf("sparse: unknow chunk type %04x, chunk_count:%d,pubf:%p\n",
				chunk->chunk_type,chunk_count,pbuf);
			return 1;
		}

	    }
            printf("write a part of file %s finished\n",aufile[idx]);
	}
    }else if (strstr(aufile[idx], "ubifs")){
	if (boot_medium_type == BOOT_FROM_NAND){
#ifdef CONFIG_CMD_NAND
	    tmp_start_addr = aufl_layout[idx].start;
	    while (sz &(nand_flash->writesize -1)){
		sz = (sz + nand_flash->writesize)&~(nand_flash->writesize - 1);
	    }
	    bad_block_count = get_bad_block_count(tmp_start_addr,sz);
	    debug("write offset: start:%ld, bad block count:%d\n",tmp_start_addr,bad_block_count);
	    tmp_start_addr += sz + bad_block_count * nand_flash->erasesize;
	    if (tmp_start_addr > aufl_layout[idx].end){
                printf("The actual length of the partition is greater than the specified \
			value due to the address offset caused by bad blocks.\n");
	        return 1;
	    }
	    rc = update_intf_p->write((aufl_layout[idx].start), sz, pbuf);
	    aufl_layout[idx].start += sz + bad_block_count * nand_flash->erasesize;
#endif
	}else{
	    rc = update_intf_p->write((aufl_layout[idx].start), sz, pbuf);
	    aufl_layout[idx].start += sz;
	}

    }else {
	if (boot_medium_type == BOOT_FROM_NAND){
#ifdef CONFIG_CMD_NAND
	    tmp_start_addr = aufl_layout[idx].start;
	    while (sz &(nand_flash->writesize -1)){
		sz = (sz + nand_flash->writesize)&~(nand_flash->writesize - 1);
	    }
	    debug("write start:%ld, len:%u,pagesize:%u\n",aufl_layout[idx].start,sz,nand_flash->writesize);
	    bad_block_count = get_bad_block_count(tmp_start_addr,sz);
	    tmp_start_addr += sz + bad_block_count * nand_flash->erasesize;

	    if (strcmp((char *)AU_FIRMWARE, aufile[idx]) == 0){
		    if (tmp_start_addr > aufl_layout[idx].end){
		        printf("The address offset caused by bad blocks causes the u-boot.bin file to \
				overwrite the environment variable.\n");
			return 1;
		    }

	    }else if(strcmp((char *)AU_KERNEL, aufile[idx]) == 0){
	        if (tmp_start_addr > aufl_layout[idx].end){
		    printf("The address offset caused by bad blocks causes the kernel file to overwrite the \
			    File system.\n");
		    return 1;
		}
	    }
	    rc = update_intf_p->write((aufl_layout[idx].start), sz, pbuf);
	    aufl_layout[idx].start += sz + bad_block_count * nand_flash->erasesize;

#endif
	}else if(boot_medium_type == BOOT_FROM_EMMC){
	    while (sz &(BLOCK_SIZE -1)){
		sz = (sz + BLOCK_SIZE)&~(BLOCK_SIZE - 1);
	    }
	    debug("write start:%ld, len:%#x\n",aufl_layout[idx].start,sz);
	    rc = update_intf_p->write((aufl_layout[idx].start), sz, pbuf);
	    aufl_layout[idx].start += sz;
	}
    }

    if (rc) {
	printf("write failed, return %d\n", rc);
	return 1;
    }

    unmap_physmem(buf, map_size);

    return 0;
}
#endif

/*
 * If none of the update file(u-boot, kernel or rootfs) was found
 * in the medium, return -1;
 * If u-boot has been updated, return 1;
 * Others, return 0;
 */
#ifndef CONFIG_HISI_UPGRADE_BY_SEGMENT
static int update_to_flash(void)
{
    int i = 0;
    unsigned long sz;
    int res, cnt;
    int uboot_updated = 0;
    int updatefile_found = 0;

    /* just loop thru all the possible files */
    for (i = 0; i < AU_MAXFILES && aufile[i] != NULL; i++) {
        printf("\n");

        if (!fat_exists(aufile[i])) {
            printf("%s not found!\n", aufile[i]);
            continue;
        }

        /* get file's real size */
        if (!fat_size(aufile[i], (loff_t *)&sz)) {
            aufile_size[i] = (sz + CONFIG_SYS_CACHELINE_SIZE) & (~(CONFIG_SYS_CACHELINE_SIZE - 1));;
        } else {
            printf("get size of %s failed!\n", aufile[i]);
            continue;
        }
        printf("aligned size=0x%08lx\n", aufile_size[i]);

        memset(LOAD_ADDR, 0xff, aufile_size[i]);
        sz = file_fat_read(aufile[i], LOAD_ADDR, sz);
        debug("read %s sz %ld hdr %lu\n",
              aufile[i], sz, (unsigned long)sizeof(image_header_t));
        if (sz <= 0) {
            printf("read %s failed!\n", aufile[i]);
            continue;
        }
        /* get file's real size */
        aufile_size[i] = sz;
        printf("size=0x%08lx\n", aufile_size[i]);
        updatefile_found = 1;

        /* If u-boot had been updated, we need to
         * save current env to flash */
        if (strcmp((char *)AU_FIRMWARE, aufile[i]) == 0) {
            uboot_updated = 1;
        }

        /* this is really not a good idea, but it's what the */
        /* customer wants. */
        cnt = 0;
        do {
            res = au_do_update(i, sz);
            /* let the user break out of the loop */
            if (ctrlc() || had_ctrlc()) {
                clear_ctrlc();

                break;
            }
            cnt++;
        } while (res < 0);
    }

    if (uboot_updated == 1) {
        return 1;
    } else if (updatefile_found == 1) {
        return 0;
    } else {
        return -1;
    }
}
#else
static int update_to_flash(void)
{
    int i;
    long long sz;
    int res, cnt;
    int uboot_updated = 0;
    int updatefile_found = 0;
    loff_t actread;
    unsigned int opsz = 0;
    int nsect = 0;
    loff_t pos = 0;
    int extra = 0;
    int ret;
    unsigned int section_size,remain;

    memset(&layout,0,sizeof(buff_layout_t));
    /* just loop thru all the possible files */
    for (i = 0; i < AU_MAXFILES && aufile[i] != NULL; i++) {
	printf("\n\n");

	pos = 0;
	extra = 0;
	if (!fat_exists(aufile[i])) {
	    printf("%s not found!\n", aufile[i]);
	    continue;
	}

	/* get file's real size */
	if (!fat_size(aufile[i], (loff_t *)&sz)) {
	    aufile_size[i] = sz;
	} else {
	    printf("get size of %s failed!\n", aufile[i]);
	    continue;
	}
	printf("filesize size= %lu\n", aufile_size[i]);

	map_size = SECTION_SIZE;
	memset(LOAD_ADDR, 0xff, map_size);

#if (defined CONFIG_EMMC) || (defined CONFIG_CMD_UFS)
	if (strstr(aufile[i], "ext4") || strstr(aufile[i], "ufs")){
	    ret = file_fat_read_at(aufile[i], pos, LOAD_ADDR, 512, &actread);
	    if (ret) {
		printf("read %s failed, actread:%lld\n",aufile[i],actread);
	    }
	    memset(&sparse_header,0,sizeof(sparse_header_t));
	    get_unspare_header_info(LOAD_ADDR,&sparse_header);
	    layout.offset = sparse_header.file_hdr_sz;
	}
#endif

#ifdef CONFIG_CMD_NAND
	if (strstr(aufile[i], "yaffs2")){
	    if (sz > SECTION_SIZE){
		section_size = (SECTION_SIZE / (nand_flash->writesize + nand_flash->oobsize)) * \
			       (nand_flash->writesize + nand_flash->oobsize);
	    }else{
		section_size = sz;
	    }
	}else{
	    section_size = SECTION_SIZE;
	}
	debug("pagesize:%u, oobsize:%u, section_size:%u\n",nand_flash->writesize,\
		nand_flash->oobsize,section_size);
#endif

#ifndef CONFIG_CMD_NAND
	section_size = SECTION_SIZE;
#endif
	nsect = aufile_size[i] / section_size;
	remain = aufile_size[i] % section_size;

	if (remain){
	    extra = 1;
	}
#if (defined CONFIG_EMMC) || (defined CONFIG_CMD_UFS)
	if (strstr(aufile[i], "ext4") || strstr(aufile[i], "ufs")){
	    extra = 1;
	}
#endif
	for(int segment = 0;segment < nsect + extra; segment++){

	    if ((segment == nsect) && remain){
		opsz = remain;
	    }else{
		opsz = section_size;
	    }

	    printf("\nnsect:%d, remain:%u, section_size:%u, pos:%llu, opsz:%u, map_size:%lu\n",nsect,remain,section_size,pos,opsz,map_size);
	    memset(LOAD_ADDR, 0xff, map_size);
	    ret = file_fat_read_at(aufile[i], pos, LOAD_ADDR, opsz, &actread);
	    if (ret) {
		printf("read %s failed,opsz:%d,actread:%lld,pos:%lld\n", aufile[i],opsz,actread,pos);
	    }
#if (defined CONFIG_EMMC) || (defined CONFIG_CMD_UFS)
	    memset(&perv_layout,0,sizeof(buff_layout_t));
	    if (strstr(aufile[i], "ext4") || strstr(aufile[i], "ufs")){
		perv_layout.offset = layout.offset;
		perv_layout.type = layout.type;
		ret = get_buffer_chunk_layout(perv_layout.offset,LOAD_ADDR,aufile_size[i],pos);
		if (ret){
		    printf("Failed to parse the buffer.\n");
		}
	    }
#endif
	    printf("reading %d part of %s ,size:%u, start:%lld\n",segment,aufile[i],opsz,pos);
	    updatefile_found = 1;

	    /* If u-boot had been updated, we need to
	     * save current env to flash */
	    if (strcmp((char *)AU_FIRMWARE, aufile[i]) == 0) {
		uboot_updated = 1;
	    }

	    /* this is really not a good idea, but it's what the */
	    /* customer wants. */
	    cnt = 0;
	    do {
		res = au_do_update(i, opsz, segment, perv_layout.offset,perv_layout.type);
		/* let the user break out of the loop */
		if (ctrlc() || had_ctrlc()) {
		    clear_ctrlc();

		    break;
		}
		cnt++;
	    } while (res < 0);
#if (defined CONFIG_EMMC) || (defined CONFIG_CMD_UFS)
	    if (strstr(aufile[i], "ext4") || strstr(aufile[i], "ufs")){
		pos += layout.len;
		remain = aufile_size[i] - pos;
		debug("nsect:%d,remain:%u,pos:%lld\n",nsect,remain,pos);
	    }

	    if ((segment == nsect) && (remain > SECTION_SIZE)){
		extra += 1;
	    }
#else
	    pos += section_size;
#endif

	}
    }

    if (uboot_updated == 1) {
	return 1;
    } else if (updatefile_found == 1) {
	return 0;
    } else {
        return -1;
    }
}
#endif


#define ENV_MAX_LEN    (CONFIG_MAX_SIZE / 2)

/*
 * pick up env form config file and set env
 * fail:return -1;
 * ok:  return 0;
 */
static int env_pick_up(const char* envname, const char *buffer)
{
    char *str, *str_s, *str_e;
    char env[ENV_MAX_LEN];

    str = strstr(buffer, envname);
    if (!str) {
        goto env_err;
    }

    str_s = strchr(str, '\'');
    if (!str_s) {
        goto env_err;
    }

    str_e = strchr(++str_s, '\'');
    if(!str_e) {
        goto env_err;
    }

    if ((unsigned long)(str_e - str_s) > ENV_MAX_LEN) {
        printf("ERROR:%s too long!\n", envname);
        goto err;
    }

    strncpy(env, str_s, (size_t)(str_e - str_s));
    env[(size_t)(str_e - str_s)] = '\0';
    setenv((char *)envname, env);

    return 0;

env_err:
    printf("ERROR:%s not found!\n", envname);
err:
    return -1;
}

/*
 * pick up bootargs and bootcmd from config file and save env
 * fail:return -1;
 * ok:  return 0;
 */
static int get_env_from_config(void)
{
    char *aufile = AU_CONFIG;
    int ret;
    long sz = file_fat_read(aufile, (void *)LOAD_ADDR, CONFIG_MAX_SIZE);

    if (sz <= 0) {
        printf("ERROR:%s not found!\n", aufile);
        goto err;
    }

    ret = env_pick_up("bootargs", (char *)LOAD_ADDR);
    if (ret < 0) {
        goto err;
    }

    ret = env_pick_up("bootcmd", (char *)LOAD_ADDR);
    if (ret < 0) {
        goto err;
    }

    /*(void)env_pick_up("minibootargs", (char *)LOAD_ADDR);*/

    return 0;
err:
    return -1;
}

struct mtd_part_s {
    char * str;
    int unit;
    struct mtd_part_s *next;
};

/*
 * insert node to list
 */
struct mtd_part_s * list_insert(struct mtd_part_s *head, struct mtd_part_s *node)
{
    struct mtd_part_s *p = head;

    if (!head) {
        head = node;
    }

    while(p) {
        if(p->next == NULL) {
            p->next = node;
            break;
        }
        p = p->next;
    }

    return head;
}

/*
 * sort list by str
 */
struct mtd_part_s *mtd_list_sort(struct mtd_part_s *head)
{
    int flag = 0;
    struct mtd_part_s *p, *pt1, *pt2;

    if (!head) {
        return NULL;
    }

    if (head->next == NULL) {
        return head;
    }

    do {
        flag = 0;

        if ((unsigned long)(head->str) >= (unsigned long)(head->next->str)) {
            pt1 = head->next->next;
            pt2 = head->next;
            pt2->next = head;
            head->next = pt1;
            head = pt2;
        }

        for (p = head; p->next->next != NULL; p = p->next) {
            if((unsigned long)(p->next->str) >= (unsigned long)(p->next->next->str)) {
                pt1 = p->next->next->next;
                pt2 = p->next->next;
                pt2->next = p->next;
                p->next->next = pt1;
                p->next = pt2;
                flag = 1;
            }
        }

    } while (flag);

    return head;
}

/*
 * get mtd parttion info list from env string
 */
struct mtd_part_s *get_mtd_parts(char *env)
{
    char *str = env;
    struct mtd_part_s * part_p;
    struct mtd_part_s * head = NULL;

    str = env;
    while ((str = strstr(str, "M("))) {
        part_p = malloc(sizeof(struct mtd_part_s));
        if(part_p == NULL) {
            return NULL;
        }
        part_p->str = str;
        part_p->unit = 1024 * 1024;
        part_p->next = NULL;
        head = list_insert(head, part_p);
        str++;
    }

    str = env;
    while ((str = strstr(str, "m("))) {
        part_p = malloc(sizeof(struct mtd_part_s));
        if (part_p == NULL) {
            return NULL;
        }
        part_p->str = str;
        part_p->unit = 1024 * 1024;
        part_p->next = NULL;
        head = list_insert(head, part_p);
        str++;
    }

    str = env;
    while ((str = strstr(str, "K("))) {
        part_p = malloc(sizeof(struct mtd_part_s));
        if (part_p == NULL) {
            return NULL;
        }
        part_p->str = str;
        part_p->unit = 1024;
        part_p->next = NULL;
        head = list_insert(head, part_p);
        str++;
    }

    str = env;
    while ((str = strstr(str, "k("))) {
        part_p = malloc(sizeof(struct mtd_part_s));
        if (part_p == NULL) {
            return NULL;
        }
        part_p->str = str;
        part_p->unit = 1024;
        part_p->next = NULL;
        head = list_insert(head, part_p);
        str++;
    }

    head = mtd_list_sort(head);

    return head;
}

int bootargs_analyze(void)
{
    char *str;
    char *str_0;
    char *str_num;
    int i;
    int j;
    int k;
    int num;
    unsigned long size;
    long start = 0;
    struct mtd_part_s *part_p = NULL;
    char env[ENV_MAX_LEN];
    char *envp = getenv("bootargs");

    if (strlen(envp) > ENV_MAX_LEN - 1) {
        printf("ERROR:bootargs is too long!\n");
        return -1;
    }

    strcpy(env, envp);

    str = env;
    str_0 = env;
    i = 0;
    part_p = get_mtd_parts(env);

    while (part_p) {
        if (i >= AU_MAXFILES) {
            printf("ERROR:Num of partition is more than %0d!\n", AU_MAXFILES);
            break;
        }
        str = part_p->str;

        str_num = str - 1;
        j = 0;
        size = 0;

        while (*str_num >= '0' && *str_num <= '9') {
            k = j;
            num = *str_num - '0';
            while (k) {
                num *= 10;
                k--;
            }
            size += num;
            j++;
            str_num--;
        }
        ausize[i] = size * part_p->unit;
        aufl_layout[i].start = start;
        aufl_layout[i].end = start + ausize[i] - 1;
        start += ausize[i];

        str += 2;
        str_0 = strstr(str, ")");
        strncpy(aufile_table[i], str, (unsigned long)(str_0 - str));
        aufile_table[i][str_0 - str] = '\0';
        aufile[i] = &(aufile_table[i][0]);
        printf("\n[%0d]=%-16s start=0x%08lx end=0x%08lx size=0x%08lx", i, aufile_table[i],
               (unsigned long)(aufl_layout[i].start),
               (unsigned long)(aufl_layout[i].end),
               (unsigned long)ausize[i]);
        i++;
        part_p = part_p->next;
    }

    if (i == 0) {
        printf("ERROR:Can't find valid partition info!\n");
        return -1;
    }

    return 0;
}

#ifdef CONFIG_EMMC
unsigned int dos_start_lba = 0;
#ifdef CONFIG_HISI_MC
int target_dev = 0;  // 0:emmc 1:sd
#else
int target_dev = 1;  // 0:emmc 1:sd
#endif
int target_paratition = -1;  //p1~p32, -1:default to last paratition

/*
 * get mtd parttion info list from env string in order
 */
struct mtd_part_s *get_mtd_parts_inorder(char *env, int *total_paratition)
{
    char *str = env;
    struct mtd_part_s * part_p;
    struct mtd_part_s * head = NULL;
    int num = 0;

    str = env;
    while ((str = strstr(str, "("))) {
        printf("get_mtd_parts_inorder str=%s\n", str);
        part_p = malloc(sizeof(struct mtd_part_s));
        if (part_p == NULL) {
            return NULL;
        }
        if (('M' == *(str - 1)) || ('m' == *(str - 1))) {
            part_p->unit = 1024 * 1024;
        } else if(('K' == *(str - 1)) || ('k' == *(str - 1))) {
            part_p->unit = 1024;
        } else {
            part_p->unit = 1;
        }
        part_p->str = str;
        part_p->next = NULL;
        head = list_insert(head, part_p);
        str++;
        num++;
    }

    head = mtd_list_sort(head);
    *total_paratition = num;
    return head;
}


int bootargs_getend(void)
{
    char *str;
    char *str_0;
    char *str_num;
    int i;
    int j;
    int k;
    int num;
    unsigned long size;
    long start = 0;
    char filename[NAME_MAX_LEN] = {0};
    int total_paratition = 0;
    int cur_target_paratition = 0;
    struct mtd_part_s *part_p = NULL;
    char env[ENV_MAX_LEN];
    char *envp = getenv("bootargs");

    if (strlen(envp) > ENV_MAX_LEN - 1) {
        printf("ERROR:bootargs is too long!\n");
        return -1;
    }

    strcpy(env, envp);

    str = env;
    str_0 = env;
    i = 0;
    part_p = get_mtd_parts_inorder(env, &total_paratition);
    if (target_paratition == -1) {
        cur_target_paratition = total_paratition;
    } else {
        cur_target_paratition = target_paratition;
    }

    printf("total_paratition=%d\n", total_paratition);

    while (part_p) {
        if (i >= AU_MAXFILES) {
            printf("ERROR:Num of partition is more than %0d!\n", AU_MAXFILES);
            break;
        }
        str = part_p->str;

        str_num = str - 2;
        j = 0;
        size = 0;

        while (*str_num >= '0' && *str_num <= '9') {
            k = j;
            num = *str_num - '0';
            while (k) {
                num *= 10;
                k--;
            }
            size += num;
            j++;
            str_num--;
        }
        start += size * part_p->unit;
        str += 1;
        str_0 = strstr(str, ")");
        strncpy(filename, str, (unsigned long)(str_0 - str));
        filename[str_0 - str] = '\0';

        printf("\n[%0d]=%-16s start=0x%08lx", i, filename, start);
        i++;
        part_p = part_p->next;
        if (i >= cur_target_paratition) {
            break;
        }
    }

    if (i == 0) {
        printf("ERROR:Can't find valid partition info!\n");
        dos_start_lba = 0;
        return -1;
    }

    if (target_dev == 0) { //emmc device
        dos_start_lba = start / 512;
    } else { //sd device
        dos_start_lba = 0;
    }

    printf("\n dos_start_lba = %d\n", dos_start_lba);

    return 0;
}
#endif


/*
 * This is called by board_init() after the hardware has been set up
 * and is usable.
 * return -1, otherwise it will return 0;
 */
int do_auto_update(void)
{
    struct blk_desc *stor_dev;
    int old_ctrlc;
    int j;
    int state = -1;
    int dev;
    int updateflg = 0;

    au_stor_curr_dev = -1;
    for (j = 0; j < MAX_UPDATE_INTF; j++) {
        if ((unsigned long)s_intf[j].name[0] != 0) {
            au_stor_curr_dev = s_intf[j].init();
            if (-1 == au_stor_curr_dev) {
                debug("%s storage device not found!\n",
                      s_intf[j].name);
                continue;
            }

            dev = 0;
            if (strncmp("mmc", s_intf[j].name, sizeof("mmc")) == 0) {
#ifdef CONFIG_EMMC
                dev = target_dev;
#else
                dev = 0;
#endif
            }
            debug("device name %s!\n", s_intf[j].name);
            stor_dev = blk_get_dev(s_intf[j].name, dev);
            if (stor_dev == NULL) {
                debug("Unknow device type!\n");
                continue;
            }

#ifdef CONFIG_EMMC
            if (target_dev == 0) {   //emmc upgrade
                bootargs_getend();
                part_init(stor_dev); //re-do the entry->test(dev_desc) to set the dev_desc->part_type to PART_TYPE_DOS
            }
#endif

            if (fat_register_device(stor_dev, 1) != 0) {
                debug("Unable to use %s %d:%d for fatls\n",
                      s_intf[j].name,
                      au_stor_curr_dev,
                      1);
                continue;
            }

            if (file_fat_detectfs() != 0) {
                debug("file_fat_detectfs failed\n");
                continue;
            }

            unsigned int reg = readl((void *)(SYS_CTRL_REG_BASE + REG_SYSSTAT));
#if (defined CONFIG_TARGET_HI3519AV100)||(defined CONFIG_TARGET_HI3556AV100)
            if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_SPI_NAND) {
                boot_medium_type = BOOT_FROM_NAND;
                update_intf_p = &update_intf[UPDATE_MEDIUM_NAND];
            } else if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_SPI) {
                boot_medium_type = BOOT_FROM_SPI;
                update_intf_p = &update_intf[UPDATE_MEDIUM_SPINOR];
            } else if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_NAND) {
                boot_medium_type = BOOT_FROM_NAND;
                update_intf_p = &update_intf[UPDATE_MEDIUM_NAND];
            } else if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_EMMC) {
                boot_medium_type = BOOT_FROM_EMMC;
                update_intf_p = &update_intf[UPDATE_MEDIUM_EMMC];
            }
#else
            if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_SPI) {
                /*
                 * SFC_DEVICE_MODE:0:SPINOR 1:SPINAND
                 */
                if (GET_SPI_DEVICE_TYPE(reg)) {
                    boot_medium_type = BOOT_FROM_NAND;
                    update_intf_p = &update_intf[UPDATE_MEDIUM_NAND];
                } else {
                    boot_medium_type = BOOT_FROM_SPI;
                    update_intf_p = &update_intf[UPDATE_MEDIUM_SPINOR];
                }
            } else if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_NAND) {
                boot_medium_type = BOOT_FROM_NAND;
                update_intf_p = &update_intf[UPDATE_MEDIUM_NAND];
            } else if (GET_SYS_BOOT_MODE(reg) == BOOT_FROM_EMMC) {
                boot_medium_type = BOOT_FROM_EMMC;
                update_intf_p = &update_intf[UPDATE_MEDIUM_EMMC];
            }

#endif
            update_intf_p->init();

            if (get_env_from_config()) {
                printf("Warning:no config!\n");
                printf("Try to use old env!\n");
            }

            if (bootargs_analyze()) {
                printf("ERROR:bootargs analyze fail!\n");
                continue;
            }

            /*
             * make sure that we see CTRL-C
             * and save the old state
             */
            old_ctrlc = disable_ctrlc(0);

            state = update_to_flash();

            /* restore the old state */
            disable_ctrlc(old_ctrlc);

            s_intf[j].exit();

            /*
             * no update file found
             */
            if (-1 == state) {
                continue;
            }
            /*
             * update files have been found on current medium,
             * so just break here
             */
            updateflg = 1;
            break;
        }
    }

    /*
     * If u-boot has been updated, it's better to save environment to flash
     */
    if (state == 1) {
        /*env_crc_update();*/
        saveenv();
    }

    if (updateflg == 1) {
        return 0;
    } else {
        return -1;
    }
}

#endif /* CONFIG_AUTO_UPDATE */
