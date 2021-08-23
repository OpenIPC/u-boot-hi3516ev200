/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "hi_type.h"
#include "hdmi_reg_video_path.h"
#include "hdmi_product_define.h"

/* Define the struct pointor of the module video_path_reg */
volatile S_video_path_reg_REGS_TYPE *gopvideo_path_regAllReg = NULL;

int HDMI_TX_S_video_path_reg_REGS_TYPE_Init(HI_CHAR *pcAddr)
{
    pcAddr = (HI_CHAR *)HDMI_IO_MAP(HDMI_TX_BASE_ADDR, 4);

    gopvideo_path_regAllReg = (volatile S_video_path_reg_REGS_TYPE*)(pcAddr + HDMI_TX_BASE_ADDR_AIDEO);

    return HI_SUCCESS;
}

int HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit(void)
{
    if (gopvideo_path_regAllReg) {
        gopvideo_path_regAllReg = HI_NULL;
    }

    return HI_SUCCESS;
}

int HDMI_DATA_ALIGN_CTRL_reg_vert_cbcr_selSet(unsigned int ureg_vert_cbcr_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_vert_cbcr_sel = ureg_vert_cbcr_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstSet(unsigned int ureg_fifo_manu_rst)
{
    HI_U32 *pu32RegAddr = NULL;

    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_fifo_manu_rst = ureg_fifo_manu_rst;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_MULTI_CSC_CTRL_reg_csc_modeGet
 * Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_mode
 * Input       : unsigned int ureg_csc_mode: 8 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_MULTI_CSC_CTRL_reg_csc_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_mode;
}

/******************************************************************************
 * Function    : HDMI_DITHER_CONFIG_dither_modeSet
 * Description : Set the value of the member DITHER_CONFIG.dither_mode
 * Input       : unsigned int udither_mode: 2 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_DITHER_CONFIG_dither_modeSet(unsigned int udither_mode)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_mode = udither_mode;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);

    return HI_SUCCESS;
}



/******************************************************************************
 * Function    : HDMI_DITHER_CONFIG_dither_rnd_bypSet
 * Description : Set the value of the member DITHER_CONFIG.dither_rnd_byp
 * Input       : unsigned int udither_rnd_byp: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_DITHER_CONFIG_dither_rnd_bypSet(unsigned int udither_rnd_byp)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_rnd_byp = udither_rnd_byp;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_MULTI_CSC_CTRL_reg_csc_modeSet
 * Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_mode
 * Input       : unsigned int ureg_csc_mode: 8 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(unsigned int ureg_csc_mode)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_mode = ureg_csc_mode;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}



/******************************************************************************
 * Function    : HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet
 * Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_saturate_en
 * Input       : unsigned int ureg_csc_saturate_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(unsigned int ureg_csc_saturate_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_saturate_en = ureg_csc_saturate_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_MULTI_CSC_CTRL_reg_csc_enSet
 * Description : Set the value of the member MULTI_CSC_CTRL.reg_csc_en
 * Input       : unsigned int ureg_csc_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_MULTI_CSC_CTRL_reg_csc_enSet(unsigned int ureg_csc_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_en = ureg_csc_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet
 * Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_vert_en
 * Input       : unsigned int ureg_dwsm_vert_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(unsigned int ureg_dwsm_vert_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_vert_en = ureg_dwsm_vert_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet
 * Description : Set the value of the member VIDEO_DWSM_CTRL.reg_dwsm_hori_en
 * Input       : unsigned int ureg_dwsm_hori_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(unsigned int ureg_dwsm_hori_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_hori_en = ureg_dwsm_hori_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(unsigned int ureg_hori_filter_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_hori_filter_en = ureg_hori_filter_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet
 * Description : Set the value of the member DATA_ALIGN_CTRL.reg_pxl_div_en
 * Input       : unsigned int ureg_pxl_div_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(unsigned int ureg_pxl_div_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_pxl_div_en = ureg_pxl_div_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet
 * Description : Set the value of the member DATA_ALIGN_CTRL.reg_demux_420_en
 * Input       : unsigned int ureg_demux_420_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(unsigned int ureg_demux_420_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_demux_420_en = ureg_demux_420_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_inver_sync
 * Input       : unsigned int ureg_inver_sync: 4 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(unsigned int ureg_inver_sync)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_inver_sync = ureg_inver_sync;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}



/******************************************************************************
 * Function    : HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_syncmask_en
 * Input       : unsigned int ureg_syncmask_en: 4 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(unsigned int ureg_syncmask_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_syncmask_en = ureg_syncmask_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cr_sel
 * Input       : unsigned int ureg_vmux_cr_sel: 3 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(unsigned int ureg_vmux_cr_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cr_sel = ureg_vmux_cr_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_cb_sel
 * Input       : unsigned int ureg_vmux_cb_sel: 3 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(unsigned int ureg_vmux_cb_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cb_sel = ureg_vmux_cb_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_vmux_y_sel
 * Input       : unsigned int ureg_vmux_y_sel: 3 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(unsigned int ureg_vmux_y_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_y_sel = ureg_vmux_y_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.tmds_pack_mode
 * Input       : unsigned int utmds_pack_mode: 3 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(unsigned int utmds_pack_mode)
{
    HI_U32 *pu32RegAddr = NULL;

    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.tmds_pack_mode = utmds_pack_mode;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet
 * Description : Set the value of the member TX_PACK_FIFO_CTRL.tmds_pack_mode
 * Input       : unsigned int utmds_pack_mode: 2 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.tmds_pack_mode;
}

/******************************************************************************
 * Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enSet
 * Description : Set the value of the member VIDEO_DMUX_CTRL.reg_fifo_auto_rst_en
 * Input       : unsigned int ureg_fifo_auto_rst_en: 3 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enSet(unsigned int ureg_fifo_auto_rst_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_fifo_auto_rst_en = ureg_fifo_auto_rst_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdSet
 * Description : Set the value of the member TCLK_LOWER_THRESHOLD.reg_tcnt_lower_threshold
 * Input       : unsigned int ureg_tcnt_lower_threshold: 18 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdSet(unsigned int ureg_tcnt_lower_threshold)
{
    HI_U32 *pu32RegAddr = NULL;

    U_TCLK_LOWER_THRESHOLD o_tclk_lower_threshold;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TCLK_LOWER_THRESHOLD.u32);
    o_tclk_lower_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    o_tclk_lower_threshold.bits.reg_tcnt_lower_threshold = ureg_tcnt_lower_threshold;
    HDMITXRegWrite(pu32RegAddr, o_tclk_lower_threshold.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdSet
 * Description : Set the value of the member TCLK_LOWER_THRESHOLD.reg_tcnt_upper_threshold
 * Input       : unsigned int ureg_tcnt_upper_threshold: 18 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdSet(unsigned int ureg_tcnt_upper_threshold)
{
    HI_U32 *pu32RegAddr = NULL;

    U_TCLK_UPPER_THRESHOLD o_tclk_upper_threshold;
    pu32RegAddr = (HI_U32*)&(gopvideo_path_regAllReg->TCLK_LOWER_THRESHOLD.u32);
    o_tclk_upper_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    o_tclk_upper_threshold.bits.reg_tcnt_upper_threshold = ureg_tcnt_upper_threshold;
    HDMITXRegWrite(pu32RegAddr, o_tclk_upper_threshold.u32);

    return HI_SUCCESS;
}


