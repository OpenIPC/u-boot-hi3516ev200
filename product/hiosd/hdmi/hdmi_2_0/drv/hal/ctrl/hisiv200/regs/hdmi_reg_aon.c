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
#include "hdmi_reg_aon.h"
#include "hdmi_product_define.h"

/* Define the struct pointor of the module tx_aon_reg */
volatile S_tx_aon_reg_REGS_TYPE *goptx_aon_regAllReg = NULL;

int HDMI_TX_S_tx_aon_reg_REGS_TYPE_Init(HI_CHAR *pcAddr)
{
    pcAddr = (HI_CHAR *)HDMI_IO_MAP(HDMI_TX_BASE_ADDR, 4);

    goptx_aon_regAllReg = (volatile S_tx_aon_reg_REGS_TYPE *)(pcAddr + (HDMI_TX_BASE_ADDR_AON));

    return HI_SUCCESS;
}

int HDMI_TX_S_tx_aon_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_aon_regAllReg) {
        goptx_aon_regAllReg = HI_NULL;
    }

    return HI_SUCCESS;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MST_CTRL_dcc_man_enSet
 * Description : Set the value of the member DDC_MST_CTRL.dcc_man_en
 * Input       : unsigned int udcc_man_en: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MST_CTRL_dcc_man_enSet(unsigned int udcc_man_en)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MST_CTRL o_ddc_mst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_ctrl.bits.dcc_man_en = udcc_man_en;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32);

    return HI_SUCCESS;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MAN_CTRL_ddc_sda_oenSet
 * Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_oen
 * Input       : unsigned int uddc_sda_oen: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MAN_CTRL_ddc_sda_oenSet(unsigned int uddc_sda_oen)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_sda_oen = uddc_sda_oen;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);

    return HI_SUCCESS;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MAN_CTRL_ddc_scl_oenSet
 * Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_oen
 * Input       : unsigned int uddc_scl_oen: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MAN_CTRL_ddc_scl_oenSet(unsigned int uddc_scl_oen)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_man_ctrl.bits.ddc_scl_oen = uddc_scl_oen;
    HDMITXRegWrite(pu32RegAddr, o_ddc_man_ctrl.u32);

    return HI_SUCCESS;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MST_STATE_ddc_i2c_no_ackGet
 * Description : Set the value of the member DDC_MST_STATE.ddc_i2c_no_ack
 * Input       : unsigned int uddc_i2c_no_ack: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MST_STATE_ddc_i2c_no_ackGet(void)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_i2c_no_ack;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MST_STATE_ddc_i2c_bus_lowGet
 * Description : Set the value of the member DDC_MST_STATE.ddc_i2c_bus_low
 * Input       : unsigned int uddc_i2c_bus_low: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MST_STATE_ddc_i2c_bus_lowGet(void)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MST_STATE o_ddc_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_STATE.u32);
    o_ddc_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_state.bits.ddc_i2c_bus_low;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MAN_CTRL_ddc_sda_stGet
 * Description : Set the value of the member DDC_MAN_CTRL.ddc_sda_st
 * Input       : unsigned int uddc_sda_st: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MAN_CTRL_ddc_sda_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_sda_st;
}

/*******************************************************************************
 * Function    : HDMI_DDC_MAN_CTRL_ddc_scl_stGet
 * Description : Set the value of the member DDC_MAN_CTRL.ddc_scl_st
 * Input       : unsigned int uddc_scl_st: 1 bits
 * Return      : int: 0-Error, 1-Success
*******************************************************************************/
int HDMI_DDC_MAN_CTRL_ddc_scl_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;

    U_DDC_MAN_CTRL o_ddc_man_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MAN_CTRL.u32);
    o_ddc_man_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_man_ctrl.bits.ddc_scl_st;
}

int HDMI_DDC_MST_CTRL_ddc_speed_cntSet(unsigned int uddc_speed_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_CTRL o_ddc_mst_ctrl;

    pu32RegAddr = (HI_U32*)&(goptx_aon_regAllReg->DDC_MST_CTRL.u32);
    o_ddc_mst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_ctrl.bits.ddc_speed_cnt = uddc_speed_cnt;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_ctrl.u32);

    return HI_SUCCESS;
}


