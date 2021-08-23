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

    gopvideo_path_regAllReg = (volatile S_video_path_reg_REGS_TYPE *)(pcAddr + HDMI_TX_BASE_ADDR_AIDEO);

    return HI_SUCCESS;
}

int HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit(void)
{
    if (gopvideo_path_regAllReg) {
        gopvideo_path_regAllReg = HI_NULL;
    }

    return HI_SUCCESS;
}

int HDMI_MULTI_CSC_CTRL_reg_csc_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_multi_csc_ctrl.bits.reg_csc_mode;
}

int HDMI_DITHER_CONFIG_dither_modeSet(unsigned int udither_mode)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_mode = udither_mode;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);

    return HI_SUCCESS;
}

int HDMI_DITHER_CONFIG_dither_rnd_bypSet(unsigned int udither_rnd_byp)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DITHER_CONFIG o_dither_config;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->DITHER_CONFIG.u32);
    o_dither_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_dither_config.bits.dither_rnd_byp = udither_rnd_byp;
    HDMITXRegWrite(pu32RegAddr, o_dither_config.u32);

    return HI_SUCCESS;
}

int HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(unsigned int ureg_csc_mode)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_mode = ureg_csc_mode;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(unsigned int ureg_csc_saturate_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_saturate_en = ureg_csc_saturate_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_MULTI_CSC_CTRL_reg_csc_enSet(unsigned int ureg_csc_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_MULTI_CSC_CTRL o_multi_csc_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->MULTI_CSC_CTRL.u32);
    o_multi_csc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_multi_csc_ctrl.bits.reg_csc_en = ureg_csc_en;
    HDMITXRegWrite(pu32RegAddr, o_multi_csc_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet(unsigned int ureg_dwsm_vert_byp)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_vert_byp = ureg_dwsm_vert_byp;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(unsigned int ureg_dwsm_vert_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_vert_en = ureg_dwsm_vert_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(unsigned int ureg_hori_filter_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_hori_filter_en = ureg_hori_filter_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(unsigned int ureg_dwsm_hori_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DWSM_CTRL o_video_dwsm_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DWSM_CTRL.u32);
    o_video_dwsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dwsm_ctrl.bits.reg_dwsm_hori_en = ureg_dwsm_hori_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dwsm_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(unsigned int ureg_pxl_div_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_pxl_div_en = ureg_pxl_div_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(unsigned int ureg_demux_420_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DATA_ALIGN_CTRL o_data_align_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->DATA_ALIGN_CTRL.u32);
    o_data_align_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_data_align_ctrl.bits.reg_demux_420_en = ureg_demux_420_en;
    HDMITXRegWrite(pu32RegAddr, o_data_align_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(unsigned int ureg_inver_sync)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_inver_sync = ureg_inver_sync;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(unsigned int ureg_syncmask_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_syncmask_en = ureg_syncmask_en;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(unsigned int ureg_vmux_cr_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cr_sel = ureg_vmux_cr_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(unsigned int ureg_vmux_cb_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_cb_sel = ureg_vmux_cb_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(unsigned int ureg_vmux_y_sel)
{
    HI_U32 *pu32RegAddr = NULL;

    U_VIDEO_DMUX_CTRL o_video_dmux_ctrl;
    pu32RegAddr = (HI_U32 *)&(gopvideo_path_regAllReg->VIDEO_DMUX_CTRL.u32);
    o_video_dmux_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_dmux_ctrl.bits.reg_vmux_y_sel = ureg_vmux_y_sel;
    HDMITXRegWrite(pu32RegAddr, o_video_dmux_ctrl.u32);

    return HI_SUCCESS;
}

