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
#include "hdmi_reg_tx.h"
#include "hdmi_product_define.h"

/* Define the struct pointor of the module tx_hdmi_reg */
volatile S_tx_hdmi_reg_REGS_TYPE *goptx_hdmi_regAllReg = NULL;


int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init(HI_CHAR *pcAddr)
{
    pcAddr = (HI_CHAR *)HDMI_IO_MAP(HDMI_TX_BASE_ADDR, 4);

    goptx_hdmi_regAllReg = (volatile S_tx_hdmi_reg_REGS_TYPE *)(pcAddr + HDMI_TX_BASE_ADDR_HDMITX);

    return HI_SUCCESS;
}

int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_hdmi_regAllReg) {
        goptx_hdmi_regAllReg = HI_NULL;
    }

    return HI_SUCCESS;
}

int HDMI_AVI_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_PKT_HEADER o_avi_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_PKT_HEADER.u32);
    o_avi_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_pkt_header.bits.avi_pkt_hb2 = uhb2;
    o_avi_pkt_header.bits.avi_pkt_hb1 = uhb1;
    o_avi_pkt_header.bits.avi_pkt_hb0 = uhb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_pkt_header.u32);
    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb0,unsigned int uavi_pkt0_pb1,unsigned int uavi_pkt0_pb2,unsigned int uavi_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_L o_avi_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_L.u32);
    o_avi_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb3 = uavi_pkt0_pb3;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb2 = uavi_pkt0_pb2;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb1 = uavi_pkt0_pb1;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb0 = uavi_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt0_l.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb4,unsigned int uavi_pkt0_pb5,unsigned int uavi_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_H o_avi_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_H.u32);
    o_avi_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb6 = uavi_pkt0_pb6;
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb5 = uavi_pkt0_pb5;
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb4 = uavi_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt0_h.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb0,unsigned int uavi_pkt1_pb1,unsigned int uavi_pkt1_pb2,unsigned int uavi_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_L o_avi_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_L.u32);
    o_avi_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb3 = uavi_pkt1_pb3;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb2 = uavi_pkt1_pb2;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb1 = uavi_pkt1_pb1;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb0 = uavi_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt1_l.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb4,unsigned int uavi_pkt1_pb5,unsigned int uavi_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_H o_avi_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_H.u32);
    o_avi_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb6 = uavi_pkt1_pb6;
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb5 = uavi_pkt1_pb5;
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb4 = uavi_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt1_h.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb0,unsigned int uavi_pkt2_pb1,unsigned int uavi_pkt2_pb2,unsigned int uavi_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_L o_avi_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_L.u32);
    o_avi_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb3 = uavi_pkt2_pb3;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb2 = uavi_pkt2_pb2;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb1 = uavi_pkt2_pb1;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb0 = uavi_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt2_l.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb4,unsigned int uavi_pkt2_pb5,unsigned int uavi_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_H o_avi_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_H.u32);
    o_avi_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb6 = uavi_pkt2_pb6;
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb5 = uavi_pkt2_pb5;
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb4 = uavi_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt2_h.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb0,unsigned int uavi_pkt3_pb1,unsigned int uavi_pkt3_pb2,unsigned int uavi_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_L o_avi_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_L.u32);
    o_avi_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb3 = uavi_pkt3_pb3;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb2 = uavi_pkt3_pb2;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb1 = uavi_pkt3_pb1;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb0 = uavi_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt3_l.u32);

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb4,unsigned int uavi_pkt3_pb5,unsigned int uavi_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_H o_avi_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_H.u32);
    o_avi_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb6 = uavi_pkt3_pb6;
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb5 = uavi_pkt3_pb5;
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb4 = uavi_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt3_h.u32);

    return HI_SUCCESS;
}

/*************************************************************************************/
/********************************  Vsif Infoframe set ********************************/
/*************************************************************************************/
int HDMI_VSIF_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_PKT_HEADER o_vsif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_PKT_HEADER.u32);
    o_vsif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_pkt_header.bits.vsif_pkt_hb2= uhb2;
    o_vsif_pkt_header.bits.vsif_pkt_hb1= uhb1;
    o_vsif_pkt_header.bits.vsif_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_pkt_header.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb0,unsigned int uvsif_pkt0_pb1,unsigned int uvsif_pkt0_pb2,unsigned int uvsif_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_L o_vsif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_L.u32);
    o_vsif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb3 = uvsif_pkt0_pb3;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb2 = uvsif_pkt0_pb2;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb1 = uvsif_pkt0_pb1;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb0 = uvsif_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt0_l.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb4,unsigned int uvsif_pkt0_pb5,unsigned int uvsif_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_H o_vsif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_H.u32);
    o_vsif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb6 = uvsif_pkt0_pb6;
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb5 = uvsif_pkt0_pb5;
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb4 = uvsif_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt0_h.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb0,unsigned int uvsif_pkt1_pb1,unsigned int uvsif_pkt1_pb2,unsigned int uvsif_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_L o_vsif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_L.u32);
    o_vsif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb3 = uvsif_pkt1_pb3;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb2 = uvsif_pkt1_pb2;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb1 = uvsif_pkt1_pb1;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb0 = uvsif_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt1_l.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb4,unsigned int uvsif_pkt1_pb5,unsigned int uvsif_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_H o_vsif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_H.u32);
    o_vsif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb6 = uvsif_pkt1_pb6;
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb5 = uvsif_pkt1_pb5;
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb4 = uvsif_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt1_h.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb0,unsigned int uvsif_pkt2_pb1,unsigned int uvsif_pkt2_pb2,unsigned int uvsif_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_L o_vsif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_L.u32);
    o_vsif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb3 = uvsif_pkt2_pb3;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb2 = uvsif_pkt2_pb2;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb1 = uvsif_pkt2_pb1;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb0 = uvsif_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt2_l.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb4,unsigned int uvsif_pkt2_pb5,unsigned int uvsif_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_H o_vsif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_H.u32);
    o_vsif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb6 = uvsif_pkt2_pb6;
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb5 = uvsif_pkt2_pb5;
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb4 = uvsif_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt2_h.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb0,unsigned int uvsif_pkt3_pb1,unsigned int uvsif_pkt3_pb2,unsigned int uvsif_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_L o_vsif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_L.u32);
    o_vsif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb3 = uvsif_pkt3_pb3;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb2 = uvsif_pkt3_pb2;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb1 = uvsif_pkt3_pb1;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb0 = uvsif_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt3_l.u32);

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb4,unsigned int uvsif_pkt3_pb5,unsigned int uvsif_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_H o_vsif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_H.u32);
    o_vsif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb6 = uvsif_pkt3_pb6;
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb5 = uvsif_pkt3_pb5;
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb4 = uvsif_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt3_h.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet
 * Description : Set the value of the member CEA_AVI_CFG.cea_avi_rpt_en
 * Input       : unsigned int ucea_avi_rpt_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(unsigned int ucea_avi_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_avi_cfg.bits.cea_avi_rpt_en = ucea_avi_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_avi_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_AVI_CFG_cea_avi_enSet
 * Description : Set the value of the member CEA_AVI_CFG.cea_avi_en
 * Input       : unsigned int ucea_avi_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_AVI_CFG_cea_avi_enSet(unsigned int ucea_avi_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_avi_cfg.bits.cea_avi_en = ucea_avi_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_avi_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_GEN_CFG_cea_gen_enSet
 * Description : Set the value of the member CEA_GEN_CFG.cea_gen_en
 * Input       : unsigned int ucea_gen_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_GEN_CFG_cea_gen_enSet(unsigned int ucea_gen_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN_CFG o_cea_gen_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN_CFG.u32);
    o_cea_gen_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen_cfg.bits.cea_gen_en = ucea_gen_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet
 * Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
 * Input       : unsigned int cea_cp_rpt_cnt: 8 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(unsigned int ucea_cp_rpt_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_rpt_cnt = ucea_cp_rpt_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);

    return HI_SUCCESS;
}


/******************************************************************************
 * Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_enSet
 * Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
 * Input       : unsigned int ucea_cp_rpt_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(unsigned int ucea_cp_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_rpt_en = ucea_cp_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_CP_CFG_cea_cp_enSet
 * Description : Set the value of the member CEA_CP_CFG.cea_cp_en
 * Input       : unsigned int ucea_cp_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_CP_CFG_cea_cp_enSet(unsigned int ucea_cp_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_en = ucea_cp_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet
 * Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_rpt_en
 * Input       : unsigned int ucea_vsif_rpt_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(unsigned int ucea_vsif_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_vsif_cfg.bits.cea_vsif_rpt_en = ucea_vsif_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_VSIF_CFG_cea_vsif_enSet
 * Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_en
 * Input       : unsigned int ucea_vsif_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_VSIF_CFG_cea_vsif_enSet(unsigned int ucea_vsif_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_vsif_cfg.bits.cea_vsif_en = ucea_vsif_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_AVMIXER_CONFIG_dc_pkt_enSet
 * Description : Set the value of the member AVMIXER_CONFIG.dc_pkt_en
 * Input       : unsigned int udc_pkt_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_AVMIXER_CONFIG_dc_pkt_enSet(unsigned int udc_pkt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.dc_pkt_en = udc_pkt_en;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_AVMIXER_CONFIG_hdmi_modeSet
 * Description : Set the value of the member AVMIXER_CONFIG.hdmi_mode
 * Input       : unsigned int uhdmi_mode: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_AVMIXER_CONFIG_hdmi_modeSet(unsigned int uhdmi_mode)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.hdmi_mode = uhdmi_mode;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet
 * Description : Set the value of the member CP_PKT_AVMUTE.cp_clr_avmute
 * Input       : unsigned int ucp_clr_avmute: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(unsigned int ucp_clr_avmute)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    o_cp_pkt_avmute.bits.cp_clr_avmute = ucp_clr_avmute;
    HDMITXRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet
 * Description : Set the value of the member CP_PKT_AVMUTE.cp_set_avmute
 * Input       : unsigned int ucp_set_avmute: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(unsigned int ucp_set_avmute)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    o_cp_pkt_avmute.bits.cp_set_avmute = ucp_set_avmute;
    HDMITXRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_HDMI_ENC_CTRL_enc_bypassSet
 * Description : Set the value of the member HDMI_ENC_CTRL.enc_bypass
 * Input       : unsigned int uenc_bypass: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_HDMI_ENC_CTRL_enc_bypassSet(unsigned int uenc_bypass)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_bypass = uenc_bypass;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_HDMI_ENC_CTRL_enc_scr_onSet
 * Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_on
 * Input       : unsigned int uenc_scr_on: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_HDMI_ENC_CTRL_enc_scr_onSet(unsigned int uenc_scr_on)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_scr_on = uenc_scr_on;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet
 * Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi2_on
 * Input       : unsigned int uenc_hdmi2_on: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(unsigned int uenc_hdmi2_on)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_hdmi2_on = uenc_hdmi2_on;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_CEA_AVI_CFG_cea_avi_enGet
 * Description : Set the value of the member CEA_AVI_CFG.cea_avi_en
 * Input       : unsigned int ucea_avi_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_AVI_CFG_cea_avi_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_avi_cfg.bits.cea_avi_en;
}

/******************************************************************************
 * Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_enGet
 * Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
 * Input       : unsigned int ucea_cp_rpt_en: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CEA_CP_CFG_cea_cp_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_cp_cfg.bits.cea_cp_rpt_en;
}

/******************************************************************************
 * Function    : HDMI_AVMIXER_CONFIG_null_pkt_enSet
 * Description : Set the value of the member AVMIXER_CONFIG.null_pkt_en
 * Input       : unsigned int unull_pkt_en_vs_high: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_AVMIXER_CONFIG_null_pkt_enSet(unsigned int unull_pkt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.null_pkt_en = unull_pkt_en;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);

    return HI_SUCCESS;
}


/******************************************************************************
 * Function    : HDMI_AVMIXER_CONFIG_hdmi_modeGet
 * Description : Set the value of the member AVMIXER_CONFIG.hdmi_mode
 * Input       : unsigned int uhdmi_mode: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_AVMIXER_CONFIG_hdmi_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.hdmi_mode;
}

/******************************************************************************
 * Function    : HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet
 * Description : Set the value of the member CP_PKT_AVMUTE.cp_set_avmute
 * Input       : unsigned int ucp_set_avmute: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cp_pkt_avmute.bits.cp_set_avmute;
}

/******************************************************************************
 * Function    : HDMI_HDMI_ENC_CTRL_enc_scr_onGet
 * Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_on
 * Input       : unsigned int uenc_scr_on: 1 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_HDMI_ENC_CTRL_enc_scr_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_scr_on;
}

/******************************************************************************
 * Function    : HDMI_CTL_TYPE_CONFIG_ctl_type_configSet
 * Description : Set the value of the member CTL_TYPE_CONFIG.ctl_type_config
 * Input       : unsigned int uctl_type_config: 2 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_CTL_TYPE_CONFIG_ctl_type_configSet(unsigned int uctl_type_config)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CTL_TYPE_CONFIG o_ctl_type_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CTL_TYPE_CONFIG.u32);
    o_ctl_type_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_ctl_type_config.bits.ctl_type_config = uctl_type_config;
    HDMITXRegWrite(pu32RegAddr, o_ctl_type_config.u32);

    return HI_SUCCESS;
}

/******************************************************************************
 * Function    : HDMI_HTOTAL_HW_htotal_hwGet
 * Description : Set the value of the member HTOTAL_HW_.htotal_hw
 * Input       : unsigned int uhtotal_hw: 15 bits
 * Return      : int: 0-Error, 1-Success
******************************************************************************/
int HDMI_HTOTAL_HW_htotal_hwGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HTOTAL_HW o_htotal_hw;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HTOTAL_HW.u32);
    o_htotal_hw.u32 = HDMITXRegRead(pu32RegAddr);
    return o_htotal_hw.bits.htotal_hw;
}


