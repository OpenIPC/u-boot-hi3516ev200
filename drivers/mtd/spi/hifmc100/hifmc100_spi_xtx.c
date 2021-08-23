/*
 * The Flash Memory Controller v100 Device Driver for hisilicon
 *
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
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

/*****************************************************************************/
#define XTX_READ_SR_H 	(0x35)
#define XTX_READ_SR_L   (0x05)

/****************************************************************************/
/*
   enable QE bit if QUAD read write is supported by xtx's flash
*/
static int spi_xtx_qe_enable(struct hifmc_spi *spi)
{
    unsigned char status_h,status_l, op;
    unsigned int regval;
	unsigned short reg;
    const char *str[] = {"Disable", "Enable"};
    struct hifmc_host *host = (struct hifmc_host *)spi->host;

    op = spi_is_quad(spi);

    FMC_PR(QE_DBG, "\t* Start SPI Nor xtx %s Quad.\n", str[op]);

    status_h = spi_general_get_flash_register(spi, XTX_READ_SR_H);

    FMC_PR(QE_DBG, "\t  Read Status Register-h[%#x]%#x\n", XTX_READ_SR_H,
           status_h);
    if (op == SPI_NOR_GET_QE_BY_CR(status_h)) {
        FMC_PR(QE_DBG, "\t* Quad was %s status:%#x\n", str[op], status_h);
        goto QE_END;
    }

    spi->driver->write_enable(spi);
	status_l = spi_general_get_flash_register(spi, XTX_READ_SR_L);

    if (op) {
        status_h |= SPI_NOR_CR_QE_MASK;
    } else {
        status_h &= ~SPI_NOR_CR_QE_MASK;
    }
	reg = (unsigned short)status_h << 8 | status_l;
    writew(reg, host->iobase);
    FMC_PR(QE_DBG, "\t  Write IO[%p]%#x\n", host->iobase,
           *(unsigned short *)host->iobase);

   	
    regval = FMC_CMD_CMD1(SPI_CMD_WRSR);
    hifmc_write(host, FMC_CMD, regval);
    FMC_PR(QE_DBG, "\t  Set CMD[%#x]%#x\n", FMC_CMD, regval);

    regval = OP_CFG_FM_CS(spi->chipselect) | OP_CFG_OEN_EN;
    hifmc_write(host, FMC_OP_CFG, regval);
    FMC_PR(QE_DBG, "\t  Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

    regval = FMC_DATA_NUM_CNT(sizeof(unsigned short));
    hifmc_write(host, FMC_DATA_NUM, regval);
    FMC_PR(QE_DBG, "\t  Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, regval);

    regval = FMC_OP_CMD1_EN(ENABLE)
             | FMC_OP_WRITE_DATA_EN(ENABLE)
             | FMC_OP_REG_OP_START;
    hifmc_write(host, FMC_OP, regval);
    FMC_PR(QE_DBG, "\t  Set OP[%#x]%#x\n", FMC_OP, regval);

    FMC_CMD_WAIT_CPU_FINISH(host);

    /* wait the flash have switched quad mode success */
    spi->driver->wait_ready(spi);

    status_h = spi_general_get_flash_register(spi, XTX_READ_SR_H);
    FMC_PR(QE_DBG, "\t  Read Status Register-h[%#x]:%#x\n",
           XTX_READ_SR_H, status_h);
    if (op == SPI_NOR_GET_QE_BY_CR(status_h)) {
        FMC_PR(QE_DBG, "\t  %s Quad success. status_h:%#x\n",
               str[op], status_h);
    } else {
        DB_MSG("Error: %s Quad failed! reg:%#x\n", str[op],
               status_h);
	}
QE_END:
	return status_h;	

}

