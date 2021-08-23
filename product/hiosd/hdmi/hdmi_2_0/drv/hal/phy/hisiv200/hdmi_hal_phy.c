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
#include "hdmi_hal_phy.h"
#include "hdmi_reg_tx.h"
#include "hdmi_reg_dphy.h"
#include "hdmi_reg_crg.h"
#include "hdmi_reg_ctrl.h"
#include "hdmi_osal.h"
#include "hdmi_product_define.h"

#define PHY_NULL_CHK(p) \
do{\
    if(HI_NULL == p) {\
        HDMI_ERR("%s is null pointer!return fail.\n",#p);\
        return HI_FAILURE;\
    }\
}while(0)

#define PHY_NULL_CHK_NO_RET(p) \
do{\
    if(HI_NULL == p) {\
        HDMI_ERR("%s is null pointer!return fail.\n",#p); \
        return; \
    }\
}while(0)

#define PHY_RET_CHK(ret) \
do{\
    if(HI_FAILURE == ret) {\
        HDMI_ERR("Line: %d ret is failure\n", __LINE__);\
        return HI_FAILURE;\
    }\
}while(0)

#define PHY_INIT_CHK do{\
    if(HI_TRUE != s_stHDMIPhyInfo.bInit) {\
        HDMI_ERR("phy intf module didn't init!return fail.\n");\
        return HI_FAILURE;\
    }\
}while(0)


#define PHY_ARRAY_SIZEOF(a)  (sizeof(a)/sizeof(a[0]))
#define PCB_TRACE_BOUNDARY   (40)   /* 4inch */

/******************************************************************************/
/* APHY_CS DEFINE                                                             */
/******************************************************************************/
#define APHY_CS_0               0x1
#define APHY_CS_1               0x2
#define APHY_CS_2               0x4
#define APHY_CS_3               0x8
#define APHY_CS_4               0x10
#define APHY_CS_012             0x7
#define APHY_CS_4567            0xf0
#define APHY_CS_8               0x100
#define APHY_CS_9               0x200
/******************************************************************************/
/* CLOCK_SET DEFINE                                                           */
/******************************************************************************/
#define PHY_CPIS_VAL            (0x07)
#define PHY_MPLL20CLK_EN        (1<<2)
#define PHY_DATACLK_EN          (1<<3)
#define PHY_DCCCLK_EN           (1<<4)
#define PHY_VP_OUTEN            (1<<5)
#define PHY_VP_DIVNEN           (1<<7)
#define PHY_TMDS_OUTEN           0x14
#define PHY_CAS_CON             (0x02<<5)
#define PHY_ZCAL                (0x02<<5)

#define PHY_CPZS_BIT             3
#define PHY_VP_MODE              5
#define PHY_MAINEN_BIT           1
#define PHY_TXTERM_BIT           7
#define PHY_ITERM                4

volatile HI_U32 *gophdmi21tx_dphy_regAllReg = HI_NULL;
/******************************************************************************/
/* enum                                                             */
/******************************************************************************/
typedef enum {
    APHY_OFFSET_0 = 0x0,
    APHY_OFFSET_1,
    APHY_OFFSET_2,
    APHY_OFFSET_3,
    APHY_OFFSET_4,
    APHY_OFFSET_5,
    APHY_OFFSET_6,
    APHY_OFFSET_7,
    APHY_OFFSET_8,
    APHY_OFFSET_9,
    APHY_OFFSET_A,
    APHY_OFFSET_B,
    APHY_OFFSET_C,
    APHY_OFFSET_D,
    APHY_OFFSET_E,
    APHY_OFFSET_F,
    APHY_OFFSET_BUTT,
}APHY_OFFSET_ADDR_E;

/******************************************************************************/
/* Data structure                                                             */
/******************************************************************************/
typedef struct {
    HI_U32      u32ClkMin;
    HI_U32      u32ClkMax;
}PHY_CLK_RANGE_S;

typedef struct {
    HI_U32      u32pcntMin;
    HI_U32      u32pcntMax;
}PHY_PKI_RANGE_S;

typedef struct {
    HI_U8       u8PllDivm;        /* T2_PLLDIVM[3:0] */
    HI_U8       u8Fracn;          /* T2_FRACN[7:0] */
    HI_U32      u32Fracx;         /* T2_FRACX[23:0] */
}PHY_PLL_REF_CLK_RANGE_S;

typedef struct {
    HI_U32     u32VaLue1;         /* TXFFE Value1[31:0] */
    HI_U32     u32VaLue2;         /* TxFFE value2[31:0] */
    HI_U32     u32VaLue3;         /* TxFFE value3[31:0] */
}PHY_TXFFE_RANGE_S;

typedef struct {
    PHY_CLK_RANGE_S     stPhyTmdsClkRange;
    PHY_HWSPEC_CFG_S    stPhyHWSpecCfg;
}PHY_HWSPEC_S;

typedef struct {
    PHY_CLK_RANGE_S     stPhyClkRange;
    HI_U8               u8SeekValue;
}PHY_CLK_RANG_VALUE_S;

typedef struct {
    PHY_PKI_RANGE_S         stPhyPkiRange;
    HI_U8                   u8SeekValue;
}PHY_PKI_RANG_VALUE_S;

typedef struct {
    PHY_PLL_REF_CLK_RANGE_S  stPhyPllRefClkRange;
    HI_U8                    u8SeekValue;
}PHY_PLL_REF_CLK_RANG_VALUE_S;

typedef struct {
    HI_U8       u8Pki;
    HI_U8       u8p_fig_divm;
    HI_U8       u8p_fig_divp;
    HI_U32      u32p_fig_div;
    HI_U32      u32pcont1target;
    HI_U32      u32ptmdscntval;
}PHY_FCG_PARA_S;

typedef struct {
    HI_U8   u8ModT;
    HI_U8   u8ModD;
    HI_U8   u8SscgMode;
    HI_U16  u16ModN;
}PHY_FDIV_PARA_S;

typedef struct {
    HI_U32              u32IRefCnt;
    HI_U32              u32IRefCnt_len;
    PHY_FCON_MODE_E     enFconMode;
}PHY_FCON_PARA_S;

typedef struct {
    HI_U8               u8RefClkDiv;
    HI_U8               u8VpDivnsel;
    HI_U8               u8Cpzs;
    HI_U8               u8TmdsDivnsel;
    HI_U8               u8VpMode;
    HI_U8               u8FcdStep;
    HDMI_PHY_TMDS_CFG_S stTmdsCfg;
}PHY_CLK_SET_S;

typedef struct {
    HI_U8       u8MVal;
    HI_U8       u8NVal;
    HI_U32      u32PllRefClk;
    HI_U32      u32MnValue;
}PHY_MNX_S;

typedef struct {
    HI_U32              u32refdiv;
    HI_U32              u32fbdiv;
    HI_U32              u32pstdiv1;
    HI_U32              u32pstdiv2;
}MPLL_CRG_DIVN_S;

typedef struct {
    HDMI_COLORSPACE_E   enColorSpace;
    HDMI_DEEP_COLOR_E   enDeepColor;
    MPLL_CRG_DIVN_S     stMpllCrgDiv;
}MPLL_CRG_CFG_S;

/******************************************************************************/
/* Private value                                                              */
/******************************************************************************/
#ifndef HDMI_FPGA_SUPPORT
static PHY_MNX_S       s_stMnxGet;
#endif
static HDMI_PHY_INFO_S s_stHDMIPhyInfo;

#define PHY_FFE_MAX_LANE    4

/* x--lane,y--TXFFE */
PHY_TXFFE_RANGE_S  s_stPhyHWTXFFEValue[PHY_FFE_MAX_LANE][FRL_TXFFE_MODE_BUTT] = {
    {{0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}},
    {{0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}},
    {{0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}},
    {{0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}, {0xc, 0x0, 0x0}},
};

PHY_CLK_RANG_VALUE_S s_stPhyHWFcdStepSet[]= {
    {{     0,37500      }, 4},
    {{ 37500,75000      }, 3},
    {{ 75000,150000     }, 2},
    {{150000,300000     }, 1},
    {{300000,600000     }, 0}
};

PHY_CLK_RANG_VALUE_S s_stPhyHWRefClkDiv[]= {
    {{      0,37500     }, 0},
    {{  37500,75000     }, 1},
    {{  75000,150000    }, 2},
    {{ 150000,300000    }, 3},
    {{ 300000,600000    }, 4},
};

PHY_CLK_RANG_VALUE_S s_stPhyHWVpDivnsel[]= {
    {{     0,37500      }, 4},
    {{ 37500,75000      }, 3},
    {{ 75000,150000     }, 2},
    {{150000,300000     }, 1},
    {{300000,600000     }, 0}
};

/* HDMI 2.1 pll ref clk Frequency division factor */
PHY_PLL_REF_CLK_RANG_VALUE_S s_stPhyHWPllRefClk[]= {
    {{0xb,0x5,0x0       }, 0x1},
    {{0xb,0x5,0x0       }, 0x2},
    {{0x7,0x3,0x555555  }, 0x3},
    {{0x9,0x4,0x2AAAAA  }, 0x4},
    {{0xb,0x5,0x0       }, 0x5},
};

PHY_CLK_RANG_VALUE_S  s_stPhyHWTmdsDivnselTab[]= {
/****|TmdsclkRange |*|DivnselValue|**************
 *****|   Min  Max  |*|DivnselValue|**************/
    {{300000,600000     }, 0},
    {{150000,300000     }, 1},
    {{ 75000,150000     }, 2},
    {{ 37500,75000      }, 3},
    {{     0,37500      }, 4},
};

PHY_PKI_RANG_VALUE_S s_stPhyHWCntTarget[]={
    {{0     ,1023       },0xC},
    {{1024  ,2047       },0xB},
    {{2048  ,4095       },0xA},
    {{4096  ,8191       },0x9},
    {{8192  ,16383      },0x8},
};

/* for 1 shot 1, 25000<= pixelclk < 80000 */
MPLL_CRG_CFG_S s_stMpllDivnselTab0[]={
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_24BIT, {0x1, 0x100, 0x4, 0x2} },
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_30BIT, {0x1, 0x118, 0x7, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_24BIT, {0x1, 0x100, 0x4, 0x2} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_30BIT, {0x1, 0x118, 0x7, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_24BIT, {0x1, 0x100, 0x4, 0x2} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_OFF,   {0x1, 0x100, 0x4, 0x2} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_24BIT, {0x1, 0x100, 0x4, 0x4} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_30BIT, {0x1, 0x118, 0x7, 0x2} },
};

/* for 1 shot 1, 80000<= pixelclk <= 100000 */
MPLL_CRG_CFG_S s_stMpllDivnselTab1[]={
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_24BIT, {0x1, 0x060, 0x3, 0x1} },
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_30BIT, {0x1, 0x078, 0x3, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_24BIT, {0x1, 0x060, 0x3, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_30BIT, {0x1, 0x078, 0x3, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_24BIT, {0x1, 0x060, 0x3, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_OFF,   {0x1, 0x060, 0x3, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_24BIT, {0x1, 0x060, 0x6, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_30BIT, {0x1, 0x078, 0x6, 0x1} },
};

/* for 1 shot 1, 100000<= pixelclk <= 200000 */
MPLL_CRG_CFG_S s_stMpllDivnselTab2[]={
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_24BIT, {0x1, 0x040, 0x2, 0x1} },
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_30BIT, {0x1, 0x050, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_24BIT, {0x1, 0x040, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_30BIT, {0x1, 0x050, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_24BIT, {0x1, 0x040, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_OFF  , {0x1, 0x040, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_24BIT, {0x1, 0x060, 0x4, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_30BIT, {0x1, 0x078, 0x4, 0x1} },
};

/* for 1 shot 1, 200000 <= pixelclk < 600000 */
MPLL_CRG_CFG_S s_stMpllDivnselTab3[]={
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x1, 0x1} },
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_OFF,   {0x1, 0x020, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x2, 0x1} },
};

/* for 1 shot 2, 600000 <= pixelclk < 2376000 */
MPLL_CRG_CFG_S s_stMpllDivnselTab4[]={
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x2, 0x1} },
    {HDMI_COLORSPACE_RGB,      HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr444, HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr422, HDMI_DEEP_COLOR_OFF,   {0x1, 0x020, 0x2, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_24BIT, {0x1, 0x020, 0x1, 0x1} },
    {HDMI_COLORSPACE_YCbCr420, HDMI_DEEP_COLOR_30BIT, {0x1, 0x028, 0x1, 0x1} },
};


/******************************************************************************/
/* Private interface                                                          */
/******************************************************************************/

#ifndef HDMI_FPGA_SUPPORT
/* manual to configurate APHY register */
static HI_VOID PhyHWWriteStb1Byte(HI_U32 u32CS, APHY_OFFSET_ADDR_E enAphyOffset, HI_U32 u32Wdata)
{
    /* CS reset */
    HDMI_PHY_CSEN_stb_cs_enSet(0x00);
    /* WR reset */
    HDMI_PHY_WR_stb_wenSet(0);
    /* WDATA set */
    HDMI_PHY_WDATA_stb_wdataSet(u32Wdata);
    /* ADDR set */
    HDMI_PHY_ADDR_stb_addrSet(enAphyOffset);
    /* delay time */
    HDMI_UDELAY(1);
    /* WR set */
    HDMI_PHY_WR_stb_wenSet(1);
    /* CS set */
    HDMI_PHY_CSEN_stb_cs_enSet(u32CS);
    /* delay time */
    HDMI_UDELAY(1);
    /* CS reset */
    HDMI_PHY_CSEN_stb_cs_enSet(0x00);
    /* WR reset */
    HDMI_PHY_WR_stb_wenSet(00);

    return ;
}

static HI_VOID PhyHWReadStb1Byte(HI_U32 u32CS,
    APHY_OFFSET_ADDR_E enAphyOffset, HI_U8 *pu8Rdata)
{
    PHY_NULL_CHK_NO_RET(pu8Rdata);

    /* CS reset */
    HDMI_PHY_CSEN_stb_cs_enSet(0x00);
    /* WR reset */
    HDMI_PHY_WR_stb_wenSet(0);
    /* ADDR set */
    HDMI_PHY_ADDR_stb_addrSet(enAphyOffset);
    /* delay time */
    HDMI_UDELAY(1);
    /* CS set */
    HDMI_PHY_CSEN_stb_cs_enSet(u32CS);
    /* delay time */
    HDMI_UDELAY(1);
    /* RDATA read */
    *pu8Rdata = (HI_U8)HDMI_PHY_RDATA_stb_rdataGet();
    /* CS reset */
    HDMI_PHY_CSEN_stb_cs_enSet(0x00);

    return ;
}

static HI_VOID PhyHWWriteStb(HI_U32 u32CS, APHY_OFFSET_ADDR_E enAphyOffset,
                             HI_U8 u8Msb, HI_U8 u8Lsb, HI_U32 u32Wdata)
{
    HI_U8 u8Rdata = 0;
    HI_U8 u8MaskValue = 0;
    HI_U8 u8WriteValue = 0;

    if((u8Msb == 0x7) && (u8Lsb == 0x0)) {
        PhyHWWriteStb1Byte(u32CS, enAphyOffset, u32Wdata);
    } else {
        if(u32CS == (APHY_CS_012 | APHY_CS_3)) {
            PhyHWReadStb1Byte(APHY_CS_0, enAphyOffset, &u8Rdata);
        } else if(u32CS == APHY_CS_4567) {
            PhyHWReadStb1Byte(APHY_CS_4, enAphyOffset, &u8Rdata);
        } else {
            PhyHWReadStb1Byte(u32CS, enAphyOffset, &u8Rdata);
        }

        u8MaskValue = (0xff >> (0x7 - u8Msb)) & (0xff << u8Lsb);
        u8WriteValue = (~u8MaskValue & u8Rdata) + (u8MaskValue & (u32Wdata << u8Lsb));
        PhyHWWriteStb1Byte(u32CS, enAphyOffset, u8WriteValue);
    }

    return ;
}

static HI_S32 PhyHWRefClkDivset(HI_U32 u32PixelClk)
{
    HI_U32 u32RefClkDiv = 0;

    if(u32PixelClk < 37500) {
        u32RefClkDiv = 0x0;
    } else if(u32PixelClk >= 37500 && u32PixelClk < 75000) {
        u32RefClkDiv = 0x1;
    } else if(u32PixelClk >= 75000 && u32PixelClk < 150000) {
        u32RefClkDiv = 0x2;
    } else if(u32PixelClk >= 150000 && u32PixelClk < 300000) {
        u32RefClkDiv = 0x3;
    } else if(u32PixelClk >= 300000 && u32PixelClk < 600000) {
        u32RefClkDiv = 0x4;
    }

    PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_2, u32RefClkDiv);

    return HI_SUCCESS;
}

static HI_S32 PhyHWInitset(HDMI_PHY_TMDS_CFG_S *pstHdmiSpecCfg)
{
    PHY_NULL_CHK(pstHdmiSpecCfg);

    HDMI_INFO("enModeCfg = %d\n", pstHdmiSpecCfg->enModeCfg);
    if (HDMI_PHY_MODE_CFG_TMDS == pstHdmiSpecCfg->enModeCfg) {
        /*  DDC QEC Settings */
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_0, 0x20);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_1, 0x20);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_2, 0x20);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_3, 0x20);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_4, 0x10);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_5, 0x10);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_6, 0x10);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_7, 0x10);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_8, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_4567, APHY_OFFSET_9, 0x00);

        /* #PLL Settings */
#if defined(HDMI_PRODUCT_HI3559A)
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_0, 0x7f); /* ff->7f */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_1, 0x31);
        PhyHWRefClkDivset(pstHdmiSpecCfg->u32PixelClk);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_3, 0x78); /* 1f */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_4, 0x02);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_5, 0x13);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_6, 0x4c);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_7, 0x46);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_8, 0x84); /* 94 */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_9, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_A, 0x06); /* 00 */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_B, 0xE0); /* 00 */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_C, 0x10); /* 00 */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_D, 0xf0); /* e7 */
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_E, 0x73); /* 00 */
#elif defined(HDMI_PRODUCT_HI3519A)
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_0, 0xff);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_1, 0x30);
        PhyHWRefClkDivset(pstHdmiSpecCfg->u32PixelClk);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_3, 0x01);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_4, 0x02);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_5, 0x50);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_6, 0x1e);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_7, 0x14);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_8, 0x84);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_9, 0x90);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_9, 0x80);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_A, 0x03);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_B, 0xE0);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_C, 0x10);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_D, 0xff);
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_E, 0x01);
#endif

        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_0, 0x07);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_1, 0xa0);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_2, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_3, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_4, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_5, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_6, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_7, 0xa0); /* 00 */
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_8, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_9, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_A, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_B, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_C, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_D, 0x00);
        PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_E, 0x00);
    }
    return HI_SUCCESS;
}

static HI_S32 PhyHWInputClockCheck(HI_U32 *pu32PixelClk)
{
    HI_U32 u32RegRetValue = 0;
    HI_U32 u32InputClk = 0;

    PHY_NULL_CHK(pu32PixelClk);

    /* select pixel clock */
    HDMI_CLKSET_p_fd_clk_selSet(0x0);
    HDMI_CLKSET_p_refclk_selSet(0x0);
    HDMI_CLKSET_p_ctmanSet(0x0);
    HDMI_CLKSET_p_modclk_selSet(0x1);
    HDMI_CLKSET_p_fdivclk_selSet(0x0);
    HDMI_CLKSET_mod_div_valSet(0x0);

    /* set lock_val and lock_cnt */
    HDMI_FDSRCPARAM_src_lock_valSet(0x04);
    HDMI_FDSRCPARAM_src_lock_cntSet(0x02);
    HDMI_FDSRCPARAM_src_enableSet(0x0);
    HDMI_FDSRCPARAM_fdsrcparam_unusedSet(0x0);

    /* set frequency options */
    HDMI_FDSRCFREQ_src_cnt_optSet(0x1);
    HDMI_FDSRCFREQ_fdsrcfreq_unused_1Set(0);
    HDMI_FDSRCFREQ_src_freq_optSet(0);
    HDMI_FDSRCFREQ_fdsrcfreq_unused_2Set(0);
    HDMI_FDSRCFREQ_src_freq_extSet(0);

    /* FD enable */
    HDMI_FDSRCPARAM_src_lock_valSet(0x4);
    HDMI_FDSRCPARAM_src_lock_cntSet(0x2);
    HDMI_FDSRCPARAM_src_enableSet(0x1);
    HDMI_FDSRCPARAM_fdsrcparam_unusedSet(0x0);

    /* delay time */
    HDMI_MSLEEP(1);

    /* read status and result */
    u32RegRetValue = HDMI_FDSRCRES_src_det_statGet();
    u32InputClk = HDMI_FDSRCRES_src_cnt_outGet();
    HDMI_INFO("input clock = : %d  kHz\n", u32InputClk);

    if ((u32RegRetValue & 0xF) == 0xF) {
        HDMI_INFO(">> input clock quality : stable,\n");
        *pu32PixelClk = u32InputClk;
    } else {
        HDMI_INFO(">> Warning! input clock is unstable! \n");
        *pu32PixelClk = u32InputClk;
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID PhyHWResetRelease(HI_VOID)
{
    /* phy bus hardware reset and release */
    REG_HDMI_CRG_hdmitx_phy_bus_srst_req_Set(1);
    HDMI_UDELAY(1);
    REG_HDMI_CRG_hdmitx_phy_bus_srst_req_Set(0);

    /* STB_RESET reset (0x11160010) */
    HDMI_RESETN_resetnSet(0x0);

    /* SPPDHTG2 Software Reset 0x11160310 */
    HDMI_SWRESET_sw_reset_mod_clockSet(0x0);
    HDMI_SWRESET_sw_reset_tmds_clockSet(0x0);
    HDMI_SWRESET_sw_reset_mpll_clockSet(0x0);
    HDMI_SWRESET_sw_reset_nco_clockSet(0x0);
    HDMI_SWRESET_sw_reset_fd_clockSet(0x0);
    HDMI_SWRESET_sw_reset_mod_and_mpll_clockSet(0x0);
    HDMI_SWRESET_sw_reset_mod_and_nco_clockSet(0x0);
    HDMI_SWRESET_sw_reset_mod_and_fd_clockSet(0x0);
    HDMI_SWRESET_sw_reset_hsfifo_clockSet(0x0);
    HDMI_SWRESET_sw_reset_txfifo_clockSet(0x0);
    HDMI_SWRESET_sw_reset_data_clockSet(0x0);
    HDMI_SWRESET_sw_reset_hs_clockSet(0x0);
    HDMI_SWRESET_sw_reset_pllref_clockSet(0x0);
    HDMI_SWRESET_sw_reset_dac_clockSet(0x0);
    HDMI_SWRESET_dac_clock_gatSet(0x0);
    HDMI_SWRESET_sw_reset_frl_clockSet(0x0);
    HDMI_SWRESET_up_sample_fifo_clock_swrstSet(0x0);
    HDMI_SWRESET_swreset_unusedSet(0x0);
    HDMI_SWRESET_global_resetSet(1);

    /* STB_RESET reset release (0x11160010) */
    HDMI_RESETN_resetnSet(0x1);

    /* SPPDHTG2 Software Reset Release 0x11160310 */
    HDMI_SWRESET_global_resetSet(0);

    /* DAC CLOCK enable 0x11160310 */
    HDMI_SWRESET_dac_clock_gatSet(1);

    /* select mod clk */
    HDMI_CLKSET_p_fd_clk_selSet(0x0);
    HDMI_CLKSET_p_refclk_selSet(0x0);
    HDMI_CLKSET_p_ctmanSet(0x0);
    HDMI_CLKSET_p_modclk_selSet(0x1);
    HDMI_CLKSET_p_fdivclk_selSet(0x0);
    HDMI_CLKSET_mod_div_valSet(0x0);

    /* select 24MHz clock as reference clock */
    HDMI_REFCLKSEL_i_ref_clk_selSet(1);

    /* strobe auto m mode set */
    HDMI_STBOPT_req_lengthSet(0x0);
    HDMI_STBOPT_stb_delay2Set(0x2);
    HDMI_STBOPT_stb_delay1Set(0x2);
    HDMI_STBOPT_stb_delay0Set(0x2);
    HDMI_STBOPT_stb_acc_selSet(0x0);
    HDMI_STBOPT_stb_cs_selSet(0x0);

    return ;
}

/*********obtain  PLL vco frequency ***********/
static HI_S32 PhyHWDivnselGet(HI_U32 u32TmdsClk, HI_U8 *pu8TmdsDivn)
{
    HI_U32  i = 0;
    PHY_CLK_RANG_VALUE_S *pstPhyTmdsDivnsel = HI_NULL;

    PHY_NULL_CHK(pu8TmdsDivn);

    for( pstPhyTmdsDivnsel = &s_stPhyHWTmdsDivnselTab[0];
        (pstPhyTmdsDivnsel && (i<PHY_ARRAY_SIZEOF(s_stPhyHWTmdsDivnselTab)));
        pstPhyTmdsDivnsel++, i++) {
        if( (u32TmdsClk >= pstPhyTmdsDivnsel->stPhyClkRange.u32ClkMin) \
            && (u32TmdsClk < pstPhyTmdsDivnsel->stPhyClkRange.u32ClkMax) ) {
            *pu8TmdsDivn = pstPhyTmdsDivnsel->u8SeekValue;
            return HI_SUCCESS;
        }
    }

    HDMI_ERR("can't find param, u32TmdsClk:%u\n", u32TmdsClk);

    return HI_FAILURE;
}

static HI_U32 PhyHWPow(HI_U32 u32BaseNum, HI_U32 u32IndexNum)
{
    HI_U8    i = 0;
    HI_U32   u32RetVal = 1;

    for(i = 0; i < u32IndexNum; i++) {
        u32RetVal = u32RetVal * u32BaseNum;
    }

    return u32RetVal;
}

static HI_S32 PhyHWPllVcoFreGet(HI_U32 u32TmdsClk, HI_U32 *pu32PllVcoFre)
{
    HI_U8 u8TmdsDivnsel = 0;

    PHY_NULL_CHK(pu32PllVcoFre);

    if(HI_SUCCESS != PhyHWDivnselGet(u32TmdsClk, &u8TmdsDivnsel)) {
        return HI_FAILURE;
    }

    HDMI_INFO("u8TmdsDivnsel = %x \n", u8TmdsDivnsel);
    *pu32PllVcoFre = u32TmdsClk * 5 * PhyHWPow(2, u8TmdsDivnsel);
    HDMI_INFO("u32TmdsClk = %d, u32PllVcoFre=%d\n", u32TmdsClk, *pu32PllVcoFre);

    return HI_SUCCESS;
}

/******************************************************
 **u32TmdsClk > 25000 && u32TmdsClk < 100000*************
 ******************************************************/
static HI_VOID PHYSpaceCfg100M(HDMI_TRACE_LEN_E enTraceLen)
{
#if defined(HDMI_PRODUCT_HI3559A)
    /* #Data Drv Settings */
    PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
    PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
    PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
    PhyHWWriteStb1Byte(0x007, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
    PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x05, 0x1D);
    PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x08, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x09, 0x00);
    PhyHWWriteStb1Byte(0x007, 0xA , 0x00);
    PhyHWWriteStb1Byte(0x007, 0xB , 0x50);
    /* #CLK Drv Settings */
    PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
    PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
    PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
    PhyHWWriteStb1Byte(0x008, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
    PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x05, 0x22);
    PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
    PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
    PhyHWWriteStb1Byte(0x008, 0xA , 0x00);
    PhyHWWriteStb1Byte(0x008, 0xB , 0x50);

#elif defined(HDMI_PRODUCT_HI3519A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0A);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB , 0x20);
        /* CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0A);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB , 0x20);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x09);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB , 0x20);
        PhyHWWriteStb1Byte(0x100, 0xC , 0x12);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x09);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x47);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB , 0x20);
    } else {
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0A);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB , 0x20);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0A);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xA , 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB , 0x20);
    }
#endif
    return ;
}

/******************************************************
 **u32TmdsClk < 165000 && u32TmdsClk > 100000*************
 ******************************************************/
static HI_VOID PHYSpaceCfg165M(HDMI_TRACE_LEN_E enTraceLen)
{
#if defined(HDMI_PRODUCT_HI3559A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* #Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x007, 0x05, 0x26);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        /* #CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x008, 0x05, 0x24);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x00, 0xE7);
        PhyHWWriteStb1Byte(0x007, 0x01, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x02, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x007, 0x05, 0x2C);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x10);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x4A);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        /* #CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x008, 0x05, 0x24);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
    }
#elif defined(HDMI_PRODUCT_HI3519A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* #Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x00, 0xE7); /* 87->E7 */
        PhyHWWriteStb1Byte(0x007, 0x01, 0xBE); /* 3E->BE */
        PhyHWWriteStb1Byte(0x007, 0x02, 0xBE); /* 3E->BE */
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0C); /* 0A->0C */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x25); /* 00->25 */
        PhyHWWriteStb1Byte(0x007, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x81); /* 80->81 */
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        /* CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B); /* 0A->0B */
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x81); /* 80->81 */
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x00, 0xE7);
        PhyHWWriteStb1Byte(0x007, 0x01, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x02, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0C);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x25);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x81);
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x81);
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    } else {
        PhyHWWriteStb1Byte(0x007, 0x00, 0xE7); /* 87->E7 */
        PhyHWWriteStb1Byte(0x007, 0x01, 0xBE); /* 3E->BE */
        PhyHWWriteStb1Byte(0x007, 0x02, 0xBE); /* 3E->BE */
        PhyHWWriteStb1Byte(0x007, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0C); /* 0A->0C */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x25); /* 00->25 */
        PhyHWWriteStb1Byte(0x007, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x81); /* 80->81 */
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B); /* 0A->0B */
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x81); /* 80->81 */
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    }
#endif
    return ;
}

/******************************************************
 *****u32TmdsClk >= 165000 && u32TmdsClk < 340000******
 ******************************************************/
static HI_VOID PHYSpaceCfg165M_340M(HDMI_TRACE_LEN_E enTraceLen)
{
#if defined(HDMI_PRODUCT_HI3559A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* #Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x007, 0x05, 0x3D); /* MAIN_ICAL<4:0> : Data Swing Control */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00); /* POST1_ICAL<4:0> : Pre Emphasis Strength Control */
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x50); /* TXTERMEN <5:0> : Termination Impedance Control. 1~31: 3.2k~200ohm, 32~63:200~68ohm */
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x007, 0x0B, 0x50);

        /* #CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x008, 0x05, 0x24);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x00, 0xE7);
        PhyHWWriteStb1Byte(0x007, 0x01, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x02, 0xBE);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x007, 0x05, 0x3F); /* MAIN_ICAL<4:0> : Data Swing Control */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00); /* POST1_ICAL<4:0> : Pre Emphasis Strength Control */
        PhyHWWriteStb1Byte(0x007, 0x07, 0x05);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x55); /* TXTERMEN <5:0> : Termination Impedance Control. 1~31: 3.2k~200ohm, 32~63:200~68ohm */
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x007, 0x0B, 0x5F);

        /* #CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0xDF); /* FF->DF,from gaoyi,20180314 */
        PhyHWWriteStb1Byte(0x008, 0x05, 0x24);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);

    }
#elif defined(HDMI_PRODUCT_HI3519A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xE7); /* C7->E7 */
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x10); /* 0F->10 */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x50); /* 55->50 */
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        PhyHWWriteStb1Byte(0x100, 0xC,  0x0B);
        /* CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07); /* C7->07 */
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B); /* 0F->0B */
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48); /* 55->48 */
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xF7);
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x0F);
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x51);
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        PhyHWWriteStb1Byte(0x100, 0xC,  0x0E);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B);
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    } else {
        PhyHWWriteStb1Byte(0x007, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x03, 0xE7); /* C7->E7 */
        PhyHWWriteStb1Byte(0x007, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x05, 0x10); /* 0F->10 */
        PhyHWWriteStb1Byte(0x007, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x08, 0x50); /* 55->50 */
        PhyHWWriteStb1Byte(0x007, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x007, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x007, 0xB,  0x20);
        PhyHWWriteStb1Byte(0x100, 0xC,  0x0B);
        PhyHWWriteStb1Byte(0x008, 0x00, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x01, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x02, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x03, 0x07); /* C7->07 */
        PhyHWWriteStb1Byte(0x008, 0x04, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x05, 0x0B); /* 0F->0B */
        PhyHWWriteStb1Byte(0x008, 0x06, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x07, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x08, 0x48); /* 55->48 */
        PhyHWWriteStb1Byte(0x008, 0x09, 0x80);
        PhyHWWriteStb1Byte(0x008, 0xA,  0x00);
        PhyHWWriteStb1Byte(0x008, 0xB,  0x20);
    }
#endif
    return ;
}

/******************************************************
 *****u32TmdsClk >= 340000   && u32TmdsClk < 600000****
 ******************************************************/
static HI_VOID PHYSpaceCfg340M_600M(HDMI_TRACE_LEN_E enTraceLen)
{
#if defined(HDMI_PRODUCT_HI3559A)

    PhyHWWriteStb1Byte(0x007, 0x0, 0x87);
    PhyHWWriteStb1Byte(0x007, 0x1, 0x7E);
    PhyHWWriteStb1Byte(0x007, 0x2, 0x7E);
    PhyHWWriteStb1Byte(0x007, 0x3, 0xDF); /* FF->DF,from gaoyi,20180314 */
    PhyHWWriteStb1Byte(0x007, 0x4, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x5, 0x3E);
    PhyHWWriteStb1Byte(0x007, 0x6, 0x0B);
    PhyHWWriteStb1Byte(0x007, 0x7, 0x00);
    PhyHWWriteStb1Byte(0x007, 0x8, 0x70);
    PhyHWWriteStb1Byte(0x007, 0x9, 0x81);
    PhyHWWriteStb1Byte(0x007, 0xB, 0x5F);
    PhyHWWriteStb1Byte(0x100, 0xC, 0x12);

    /* CLK Drv Settings */
    PhyHWWriteStb1Byte(0x008, 0x0, 0x87);
    PhyHWWriteStb1Byte(0x008, 0x1, 0x3E);
    PhyHWWriteStb1Byte(0x008, 0x2, 0x3E);
    PhyHWWriteStb1Byte(0x008, 0x3, 0xDF); /* FF->DF,from gaoyi,20180314 */
    PhyHWWriteStb1Byte(0x008, 0x4, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x5, 0x22);
    PhyHWWriteStb1Byte(0x008, 0x6, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x7, 0x00);
    PhyHWWriteStb1Byte(0x008, 0x8, 0x44);
    PhyHWWriteStb1Byte(0x008, 0x9, 0x81);
    PhyHWWriteStb1Byte(0x008, 0xB, 0x50);

#elif defined(HDMI_PRODUCT_HI3519A)
    if (enTraceLen == HDMI_TRACE_LEN_0) {
        /* Data Drv Settings */
        PhyHWWriteStb1Byte(0x007, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x1, 0x7E); /* 3E->7E */
        PhyHWWriteStb1Byte(0x007, 0x2, 0x7E); /* 3E->7E */
        PhyHWWriteStb1Byte(0x007, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x5, 0x16); /* 14->16 */
        PhyHWWriteStb1Byte(0x007, 0x6, 0x08); /* 00->08 */
        PhyHWWriteStb1Byte(0x007, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x8, 0x70);
        PhyHWWriteStb1Byte(0x007, 0x9, 0x81);
        PhyHWWriteStb1Byte(0x007, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB, 0x20);
        /* CLK Drv Settings */
        PhyHWWriteStb1Byte(0x008, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x1, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x2, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x5, 0x0B); /* 14->0B */
        PhyHWWriteStb1Byte(0x008, 0x6, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x8, 0x48); /* 70->48 */
        PhyHWWriteStb1Byte(0x008, 0x9, 0x80); /* 81->80 */
        PhyHWWriteStb1Byte(0x008, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB, 0x20);
    } else if (enTraceLen == HDMI_TRACE_LEN_1) {
        PhyHWWriteStb1Byte(0x007, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x1, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x2, 0x3E);
        PhyHWWriteStb1Byte(0x007, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x5, 0x16);
        PhyHWWriteStb1Byte(0x007, 0x6, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x8, 0x70);
        PhyHWWriteStb1Byte(0x007, 0x9, 0x81);
        PhyHWWriteStb1Byte(0x007, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB, 0x20);
        PhyHWWriteStb1Byte(0x008, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x1, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x2, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x5, 0x0B);
        PhyHWWriteStb1Byte(0x008, 0x6, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x8, 0x48);
        PhyHWWriteStb1Byte(0x008, 0x9, 0x80);
        PhyHWWriteStb1Byte(0x008, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB, 0x20);
    } else {
        PhyHWWriteStb1Byte(0x007, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x007, 0x1, 0x7E); /* 3E->7E */
        PhyHWWriteStb1Byte(0x007, 0x2, 0x7E); /* 3E->7E */
        PhyHWWriteStb1Byte(0x007, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x007, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x5, 0x16); /* 14->16 */
        PhyHWWriteStb1Byte(0x007, 0x6, 0x08); /* 00->08 */
        PhyHWWriteStb1Byte(0x007, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x007, 0x8, 0x70);
        PhyHWWriteStb1Byte(0x007, 0x9, 0x81);
        PhyHWWriteStb1Byte(0x007, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x007, 0xB, 0x20);
        PhyHWWriteStb1Byte(0x008, 0x0, 0x87);
        PhyHWWriteStb1Byte(0x008, 0x1, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x2, 0x3E);
        PhyHWWriteStb1Byte(0x008, 0x3, 0x07);
        PhyHWWriteStb1Byte(0x008, 0x4, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x5, 0x0B); /* 14->0B */
        PhyHWWriteStb1Byte(0x008, 0x6, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x7, 0x00);
        PhyHWWriteStb1Byte(0x008, 0x8, 0x48); /* 70->48 */
        PhyHWWriteStb1Byte(0x008, 0x9, 0x80); /* 81->80 */
        PhyHWWriteStb1Byte(0x008, 0xA, 0x00);
        PhyHWWriteStb1Byte(0x008, 0xB, 0x20);
    }
#endif
    return ;
}
#endif

#ifndef HDMI_FPGA_SUPPORT
static HI_S32 PhyHWFconCfgParaGet(PHY_FCON_PARA_S* pstFconMode, HI_U32 u32TmdsClk,
                                                HDMI_PHY_TMDS_CFG_S *pstTmdsCfg)
{
    HI_U32 u32PllVcoFre = 0;

    PHY_NULL_CHK(pstFconMode);
    PHY_NULL_CHK(pstTmdsCfg);

    if(pstTmdsCfg->enModeCfg == HDMI_PHY_MODE_CFG_TMDS) {
        if(HI_SUCCESS != PhyHWPllVcoFreGet(u32TmdsClk, &u32PllVcoFre)) {
            return HI_FAILURE;
        }

        /***************************
             PLL_VCO_FRE(MHz)
        I_REF_CNT = ----------------- *  16(constant)*IRefCnt_len(FCCNTR0[31:24])
                24 * 20
        ***************************/
        pstFconMode->u32IRefCnt = (u32PllVcoFre * 2 * (pstFconMode->u32IRefCnt_len) / (1000 * 20 * 3));
    }

    HDMI_INFO("ModeCfg: %d, I_REF_CNT: %x\n", pstTmdsCfg->enModeCfg, pstFconMode->u32IRefCnt);

    return HI_SUCCESS;
}

static HI_VOID PhyHWSpecTmdsSet(HDMI_PHY_TMDS_CFG_S *pstSpecCfg)
{
    PHY_NULL_CHK_NO_RET(pstSpecCfg);

    HDMI_INFO("u32TmdsClk=%d, trace:%d \n", pstSpecCfg->u32TmdsClk, pstSpecCfg->enTraceLen);
    if(pstSpecCfg->u32TmdsClk < 100000) {
        PHYSpaceCfg100M(pstSpecCfg->enTraceLen);
    } else if(pstSpecCfg->u32TmdsClk >= 100000 && pstSpecCfg->u32TmdsClk < 165000) {
        PHYSpaceCfg165M(pstSpecCfg->enTraceLen);
    } else if(pstSpecCfg->u32TmdsClk >= 165000 && pstSpecCfg->u32TmdsClk < 340000) {
        PHYSpaceCfg165M_340M(pstSpecCfg->enTraceLen);
    } else if(pstSpecCfg->u32TmdsClk >= 340000 && pstSpecCfg->u32TmdsClk < 600000) {
        PHYSpaceCfg340M_600M(pstSpecCfg->enTraceLen);
    }

    return ;
}

static HI_VOID PhyHWClockTmdsSet(PHY_CLK_SET_S *pstPhyClkSet)
{
    PHY_NULL_CHK_NO_RET(pstPhyClkSet);

    PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_2, pstPhyClkSet->u8RefClkDiv & PHY_CPIS_VAL                  );
    PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_1, 0xA0                                                      );
    PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_7, (pstPhyClkSet->u8TmdsDivnsel &(0x07))|(PHY_TMDS_OUTEN<<3 ));
    PhyHWWriteStb1Byte(APHY_CS_9, APHY_OFFSET_E, 0x00                                                      );

    HDMI_FCDSTEPSET_p_divn_h20Set(pstPhyClkSet->u8FcdStep & (0x07));
    HDMI_FCDSTEPSET_fcdstepset_unusedSet(0x0);
    HDMI_FCDSTEPSET_up_sampler_ratio_selSet(0x0);
    HDMI_FDIVMANUAL_i_manual_enSet(0xe);
    HDMI_INFO("s_stMnxGet: M:%d, N:%d\n", s_stMnxGet.u8MVal, s_stMnxGet.u8NVal);
    HDMI_FDIVMANUAL_i_mdivSet(s_stMnxGet.u8MVal);
    HDMI_FDIVSTAT1_i_fdiv_inSet(((HI_U32)s_stMnxGet.u8NVal) << 24);
    HDMI_HDMI_MODE_reg_hdmi_mode_enSet((pstPhyClkSet->stTmdsCfg.u32TmdsClk > 340000) ? 0x1 : 0x0);
    HDMI_INFO("u32TmdsClk: %d\n", pstPhyClkSet->stTmdsCfg.u32TmdsClk);

    /* FDIV init */
    HDMI_FDIVSET0_initSet(0x0);
    HDMI_FDIVSET0_en_ctrlSet(0x0);
    HDMI_FDIVSET0_en_modSet(0x0);
    HDMI_FDIVSET0_en_sdmSet(0x0);

    HDMI_FDIVSET0_initSet(0x1);
    /*  delay 1us */
    HDMI_UDELAY(1);

    HDMI_FDIVSET0_initSet(0x0);
    HDMI_FDIVSET0_en_ctrlSet(0x0);
    HDMI_FDIVSET0_en_modSet(0x0);
    HDMI_FDIVSET0_en_sdmSet(0x0);

    return ;
}
#endif

#ifndef HDMI_FPGA_SUPPORT
static HI_S32 PhyHWFCON(PHY_FCON_PARA_S *pstFconMode)
{
    HI_U32 u32FconResult[4] = {0};
    HI_U32 u32Count = 0;

    PHY_NULL_CHK(pstFconMode);

    if(pstFconMode->enFconMode == PHY_FCON_MODE_AUTO) {
        HDMI_CTSET0_i_enableSet(1);
        HDMI_CTSET0_i_runSet(0);
        HDMI_CTSET0_ctset0_unusedSet(0);

        HDMI_FCCNTR0_i_vco_st_wait_lenSet(0x50);
        HDMI_FCCNTR0_i_vco_end_wait_lenSet(0x50);
        HDMI_FCCNTR0_i_ref_cnt_lenSet(pstFconMode->u32IRefCnt_len);

        HDMI_FCOPT_i_ct_selSet(0x1);
        HDMI_FCOPT_i_clkdet_selSet(0x1);
        HDMI_FCOPT_i_ct_modeSet(2);
        HDMI_FCOPT_fcopt_unused_1Set(0);
        HDMI_FCOPT_i_ct_enSet(0x1);
        HDMI_FCOPT_fcopt_unused_2Set(0);
        HDMI_FCOPT_i_ct_idx_selSet(0);
        HDMI_FCOPT_fcopt_unusedSet(0);

        HDMI_FCCNTR1_i_ref_cntSet(pstFconMode->u32IRefCnt & (0xffff));
        HDMI_INFO("stFconMode.u32IRefCnt = %x\n", pstFconMode->u32IRefCnt);

        HDMI_FCCONTINSET1_p_contin_upd_rateSet(0x0000960);
        HDMI_FCCONTINSET1_p_contin_upd_timeSet(0x0);

        /* FCCONRINSET2, fcon max value : 0x3FF, min value : 0x5 2017.3.21updataed */
        HDMI_FCCONTINSET2_p_contin_upd_th_dnSet(0x005);
        HDMI_FCCONTINSET2_fccontinset2_unusedSet(0);
        HDMI_FCCONTINSET2_p_contin_upd_th_upSet(0x3ff);

        /* FCCONRINSET0, [0] enable, [2] LKVDET polarity */
        HDMI_FCCONTINSET0_p_contin_upd_enSet(1);
        HDMI_FCCONTINSET0_p_contin_upd_optSet(0);
        HDMI_FCCONTINSET0_p_contin_upd_polSet(1);
        HDMI_FCCONTINSET0_fccontinset0_unusedSet(0);
        HDMI_FCCONTINSET0_p_contin_upd_stepSet(0x2);

        /* CTSET0, [0] i_enable, [1] run */
        HDMI_CTSET0_i_enableSet(1);
        HDMI_CTSET0_i_runSet(1);
        HDMI_CTSET0_ctset0_unusedSet(0);
        HDMI_MSLEEP(2);

        while(u32Count <= 3) {
            u32FconResult[0] = HDMI_FCSTAT_clk_okGet();
            u32FconResult[1] = HDMI_FCSTAT_busyGet();
            u32FconResult[2] = HDMI_FCSTAT_doneGet();
            u32FconResult[3] = HDMI_FCSTAT_errorGet();

            if (0 == u32FconResult[1])
            {
                break;
            }

            HDMI_MSLEEP(2);
            u32Count++;
        }

        if (u32FconResult[3] == 0 && u32FconResult[2] == 1 &&
            u32FconResult[1] == 0 && u32FconResult[0] == 1) {
            HDMI_INFO("FCON is done\n");
        } else if(u32FconResult[3] == 1) {
            HDMI_INFO("FCON is failed\n");
            return HI_FAILURE;
        }
    } else {
        HDMI_FCOPT_i_ct_selSet(0x0);
        HDMI_FCOPT_i_clkdet_selSet(0x0);
        HDMI_FCOPT_i_ct_modeSet(0x0);
        HDMI_FCOPT_fcopt_unused_1Set(0);
        HDMI_FCOPT_i_ct_enSet(0x0);
        HDMI_FCOPT_fcopt_unused_2Set(0);
        HDMI_FCOPT_i_ct_idx_selSet(0);

        HDMI_FCOPT_fcopt_unusedSet(0);
        HDMI_FCCONTINSET0_p_contin_upd_enSet(0x0);
        HDMI_FCCONTINSET0_p_contin_upd_optSet(0x0);
        HDMI_FCCONTINSET0_p_contin_upd_polSet(0x0);
        HDMI_FCCONTINSET0_fccontinset0_unusedSet(0);
        HDMI_FCCONTINSET0_p_contin_upd_stepSet(0);
        HDMI_CTSET1_i_mpll_fconSet(0x022);
        HDMI_CTSET1_i_mpll_ctlckSet(0x1);
        HDMI_CTSET1_ctset1_unusedSet(0x0);
    }

    return HI_SUCCESS;
}

static HI_S32 PhyHWPostSetup(HDMI_PHY_TMDS_CFG_S *pstTmdsCfg)
{
    PHY_NULL_CHK(pstTmdsCfg);

    /* HS FIFO enable, HS LINK external */
    HDMI_HSSET_p_hssetSet(3);

    /* transmitter FIFO and up sampler enable */
    HDMI_TXFIFOSET0_p_pr_en_h20Set(1);
    HDMI_TXFIFOSET0_p_enable_h20Set(1);
    HDMI_TXFIFOSET0_txfifoset0_unusedSet(0);

    /* loop divider close */
    PhyHWWriteStb(APHY_CS_8, APHY_OFFSET_D, 5, 5, 0x0);
    /* loop divider open */
    PhyHWWriteStb(APHY_CS_8, APHY_OFFSET_D, 5, 5, 0x1);
    HDMI_MSLEEP(5);

    /* HS input data selection */
    HDMI_TXDATAOUTSEL_p_ch_out_selSet(0x0);
    /* PHY data serializer reset/set */
    PhyHWWriteStb(APHY_CS_0, APHY_OFFSET_0, 1, 1, 0x0);
    PhyHWWriteStb(APHY_CS_1, APHY_OFFSET_0, 1, 1, 0x0);
    PhyHWWriteStb(APHY_CS_2, APHY_OFFSET_0, 1, 1, 0x0);
    PhyHWWriteStb(APHY_CS_3, APHY_OFFSET_0, 1, 1, 0x0);

    HDMI_MSLEEP(1);

    PhyHWWriteStb(APHY_CS_0, APHY_OFFSET_0, 1, 1, 0x1);
    PhyHWWriteStb(APHY_CS_1, APHY_OFFSET_0, 1, 1, 0x1);
    PhyHWWriteStb(APHY_CS_2, APHY_OFFSET_0, 1, 1, 0x1);
    PhyHWWriteStb(APHY_CS_3, APHY_OFFSET_0, 1, 1, 0x1);

    return HI_SUCCESS;
}

static HI_S32 PhyHWPhyHWClkRangValueGet(HI_U32 u32TmdsClk, HI_U8 u8Size,
                                        PHY_CLK_RANG_VALUE_S *pstPhyRangSel, HI_U8 *pu8SeekValue)
{
    HI_U32 i = 0;
    PHY_CLK_RANG_VALUE_S *pstPhyRang = HI_NULL;

    PHY_NULL_CHK( pstPhyRangSel );
    PHY_NULL_CHK( pu8SeekValue );

    for(pstPhyRang = &pstPhyRangSel[0];
       (pstPhyRang && (i<u8Size));
        pstPhyRang ++ ,i++) {
        if( (u32TmdsClk >= pstPhyRang->stPhyClkRange.u32ClkMin)  \
            && (u32TmdsClk < pstPhyRang->stPhyClkRange.u32ClkMax) ) {
            *pu8SeekValue = pstPhyRang->u8SeekValue;

            return HI_SUCCESS;
        }
    }

    HDMI_ERR("can't find param,u32TmdsClk:%d,i=%d\n", u32TmdsClk, i);

    return HI_FAILURE;
}

static HI_S32 PhyHWFractionalMnxGet(HI_U32 u32TmdsClk, HI_U32 u32PixelClk, HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_U8   k             = 0;
    HI_U8   u8MValue      = 0;
    HI_U32  u32MnValue    = 0;
    HI_U8   u8SeekValue   = 0;
    HI_U8   u8Size        = 0;
    HI_U32  u32PllRefClk  = 0;
    HI_U8   u8TmdsDivnsel = 0;

    /* HDMI 2.0 Configure pll feedback coefficient M ,N ,X */
    u8Size = PHY_ARRAY_SIZEOF(s_stPhyHWRefClkDiv);
    /* Determine the reference clock division factor */
    if(HI_SUCCESS != PhyHWPhyHWClkRangValueGet(u32PixelClk, u8Size, &s_stPhyHWRefClkDiv[0], &u8SeekValue)) {
        return HI_FAILURE;
    }

    /* obtain  TMDS_DIVNSEL */
    if(HI_SUCCESS != PhyHWDivnselGet(u32TmdsClk, &u8TmdsDivnsel)) {
        return HI_FAILURE;
    }

    HDMI_INFO("u32PixelClk: %d, u8SeekValue: %d-%d\n", u32PixelClk, u8SeekValue, PhyHWPow(2, u8SeekValue));
    u32PllRefClk = u32PixelClk / PhyHWPow(2, u8SeekValue);
    s_stMnxGet.u32PllRefClk = u32PllRefClk;
    HDMI_INFO("u32PllRefClk(%d), u8TmdsDivnsel(%d),enDeepColor(%d) \n", s_stMnxGet.u32PllRefClk, u8TmdsDivnsel, enDeepColor);

    switch(enDeepColor) {
        case HDMI_DEEP_COLOR_30BIT:
            u32MnValue = 5 * PhyHWPow(2, u8TmdsDivnsel) * PhyHWPow(2, u8SeekValue) * 5 / 4;
            break;
        case HDMI_DEEP_COLOR_36BIT:
            u32MnValue = 5 * PhyHWPow(2, u8TmdsDivnsel) * PhyHWPow(2, u8SeekValue) * 3 / 2;
            break;
        default :
            u32MnValue = 5 * PhyHWPow(2, u8TmdsDivnsel) * PhyHWPow(2, u8SeekValue);
            break;
    }

    HDMI_INFO("u32MnValue = %d \n", u32MnValue);
    s_stMnxGet.u32MnValue = u32MnValue;

    /* calculate N value */
    s_stMnxGet.u8NVal = (HI_U8)(u32MnValue % 10);
    if(0 == s_stMnxGet.u8NVal) {
        k = 2;
        s_stMnxGet.u8NVal = 10;
    }  else{
        k = 1;
    }

    /* calculate M value */
    u8MValue = (HI_U8)(u32MnValue / 10);
    s_stMnxGet.u8MVal = u8MValue - k;

    HDMI_INFO("Get Mnx M:%x, N:%d\n", s_stMnxGet.u8MVal, s_stMnxGet.u8NVal);

    return HI_SUCCESS;

}

/*  PHY clock configuration */
static HI_S32 PhyHWClockSet(PHY_CLK_SET_S *pstPhyClkSet)
{
    PHY_NULL_CHK(pstPhyClkSet);

    switch(pstPhyClkSet->stTmdsCfg.enModeCfg) {
        case HDMI_PHY_MODE_CFG_TMDS:
            PhyHWClockTmdsSet(pstPhyClkSet);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 PhyHWSpecSet(HDMI_PHY_TMDS_CFG_S *pstSpecCfg)
{
    PHY_NULL_CHK(pstSpecCfg);

    switch(pstSpecCfg->enModeCfg) {
        case HDMI_PHY_MODE_CFG_TMDS:
            PhyHWSpecTmdsSet(pstSpecCfg);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 HAL_HDMI_ClkSetParaGet(PHY_CLK_SET_S *pstPhyClkSet)
{
    HI_U8 u8Size = 0;

    PHY_NULL_CHK(pstPhyClkSet);
    /* get p_divn_h20 */
    if(pstPhyClkSet->stTmdsCfg.enModeCfg == HDMI_PHY_MODE_CFG_TMDS) {
        u8Size = PHY_ARRAY_SIZEOF(s_stPhyHWFcdStepSet);

        if(HI_SUCCESS != PhyHWPhyHWClkRangValueGet(pstPhyClkSet->stTmdsCfg.u32TmdsClk, u8Size,  \
                                        &s_stPhyHWFcdStepSet[0], &pstPhyClkSet->u8FcdStep)) {
            return HI_FAILURE;
        }
    }

    HDMI_INFO("Get u8FcdStep = %x\n", pstPhyClkSet->u8FcdStep);

    /* REFCLKDIV (TMDS) */
    if(pstPhyClkSet->stTmdsCfg.enModeCfg == HDMI_PHY_MODE_CFG_TMDS) {
        u8Size = PHY_ARRAY_SIZEOF(s_stPhyHWRefClkDiv);
        if(HI_SUCCESS != PhyHWPhyHWClkRangValueGet(pstPhyClkSet->stTmdsCfg.u32PixelClk, u8Size,  \
                                                   &s_stPhyHWRefClkDiv[0], &pstPhyClkSet->u8RefClkDiv))
        {
            return HI_FAILURE;
        }
    }
    /* FRL */
    else if(pstPhyClkSet->stTmdsCfg.enModeCfg == HDMI_PHY_MODE_CFG_FRL) {
        pstPhyClkSet->u8RefClkDiv = 0;
    }

    HDMI_INFO("Get u8RefClkDiv = %x\n", pstPhyClkSet->u8RefClkDiv);

    /* TMDS_DIVNSEL */
    if(pstPhyClkSet->stTmdsCfg.enModeCfg == HDMI_PHY_MODE_CFG_TMDS) {
        /* TMDS */
        u8Size = PHY_ARRAY_SIZEOF(s_stPhyHWTmdsDivnselTab);

        if(HI_SUCCESS != PhyHWPhyHWClkRangValueGet(pstPhyClkSet->stTmdsCfg.u32TmdsClk, u8Size,  \
                                                   &s_stPhyHWTmdsDivnselTab[0], &pstPhyClkSet->u8TmdsDivnsel))
        {
            return HI_FAILURE;
        }

        /* shut down FCG */
        PhyHWWriteStb1Byte(0x100, 0xB, 0x00);
        HDMI_FCGSET_p_fcg_enSet(0x0);
        HDMI_FCGSET_p_fcg_dlf_enSet(0x0);
        HDMI_FCGSET_p_fcg_dither_enSet(0x0);
        HDMI_FCGSET_p_fcg_lock_enSet(0x0);

    }

    HDMI_INFO("Get u8TmdsDivnsel = %x\n", pstPhyClkSet->u8TmdsDivnsel);

    return HI_SUCCESS;
}

#endif
/******************************************************************************/
/***************             Public interface            **********************/
/******************************************************************************/
HI_S32 HAL_HDMI_PhyInit(HDMI_HAL_INIT_S *pstHalInit)
{
    if(!s_stHDMIPhyInfo.bInit) {
        HDMI_HDMITX_PHY_reg_Init(HI_NULL);

        s_stHDMIPhyInfo.bInit = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyReset(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT
    PhyHWResetRelease();
#endif

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyDeInit(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT
    /* the base addr of the register  map */
    HDMI_HDMITX_PHY_reg_DeInit();
#endif

    s_stHDMIPhyInfo.bInit = HI_FALSE;

    return HI_SUCCESS;
}

/* aphy power */
HI_S32 HAL_HDMI_PhyPowerGet(HI_BOOL *pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    PHY_NULL_CHK(pbEnable);
    /* obtain power's status:0 reset; 1:set */
    if(0x00 == HDMI_RESETN_resetnGet()) {
        *pbEnable = HI_FALSE;
    } else {
        *pbEnable = HI_TRUE;
    }
#endif

    return HI_SUCCESS;
}

/* aphy power */
HI_S32 HAL_HDMI_PhyPowerSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    /* PHY MODE power on  or off */
    HDMI_RESETN_resetnSet(bEnable);
#endif

    s_stHDMIPhyInfo.bPowerEnable  = bEnable;

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyOeSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_U8 u8PhyOeStat = 0;

    u8PhyOeStat = (HI_TRUE == bEnable) ? 0x01 : 0x00;
    /* CH0-3 DRIVER OE */
    PhyHWWriteStb(0x01, 0x03, 0x00, 0x00, u8PhyOeStat);
    PhyHWWriteStb(0x02, 0x03, 0x00, 0x00, u8PhyOeStat);
    PhyHWWriteStb(0x04, 0x03, 0x00, 0x00, u8PhyOeStat);
    PhyHWWriteStb(0x08, 0x03, 0x00, 0x00, u8PhyOeStat);
#endif

    s_stHDMIPhyInfo.bOeEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyOeGet(HI_BOOL *pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    /* get oe's status */
    HI_U8 u8OeD0Status = 0;
    HI_U8 u8OeD1Status = 0;
    HI_U8 u8OeD2Status = 0;
    HI_U8 u8OeClkStatus = 0;

    PHY_NULL_CHK(pbEnable);

    if(0 == HDMI_RESETN_resetnGet()) {
        HDMI_INFO("phy is reset now, OE disable.\n");
        *pbEnable = HI_FALSE;
        return HI_SUCCESS;
    }

    PhyHWReadStb1Byte(0x1, 0x03, &u8OeD0Status);
    PhyHWReadStb1Byte(0x2, 0x03, &u8OeD1Status);
    PhyHWReadStb1Byte(0x4, 0x03, &u8OeD2Status);
    PhyHWReadStb1Byte(0x8, 0x03, &u8OeClkStatus);

    if(   (u8OeD0Status & 0x01)
       && (u8OeD1Status & 0x01)
       && (u8OeD2Status & 0x01)
       && (u8OeClkStatus & 0x01)) {
        *pbEnable = HI_TRUE;
    } else {
        *pbEnable = HI_FALSE;
    }
#endif

    return HI_SUCCESS;
}

/* for video path */
HI_S32 HAL_HDMI_PhyTmdsSet(HDMI_PHY_TMDS_CFG_S *pstTmdsCfg)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_U32             u32PixelClk  = 0;
    PHY_FCON_PARA_S    stFconMode   = {0};
    HDMI_PHY_SSC_CFG_S stHdmiSscCfg = {0};
    PHY_CLK_SET_S      stPhyClkSet  = {0};

    PHY_NULL_CHK(pstTmdsCfg);

    if( (pstTmdsCfg->enModeCfg == HDMI_PHY_MODE_CFG_TMDS) ||
        (pstTmdsCfg->enModeCfg == HDMI_PHY_MODE_CFG_FRL)) {
        HDMI_MEMSET(&stFconMode, 0, sizeof(PHY_FCON_PARA_S));
        HDMI_MEMSET(&stHdmiSscCfg, 0, sizeof(HDMI_PHY_SSC_CFG_S));
        HDMI_MEMSET(&stPhyClkSet, 0, sizeof(PHY_CLK_SET_S));

        stFconMode.u32IRefCnt_len = 128;

        HDMI_INFO("TMDS mode: %d\n",   pstTmdsCfg->enModeCfg);
        HDMI_INFO("u32PixelClk: %d\n", pstTmdsCfg->u32PixelClk);
        HDMI_INFO("u32TmdsClk: %d\n",  pstTmdsCfg->u32TmdsClk);
        HDMI_INFO("enDeepColor: %d\n", pstTmdsCfg->enDeepColor);
        HDMI_INFO("enTraceLen: %d\n",  pstTmdsCfg->enTraceLen);

        /* Reset:clear all the aphy register */
        PhyHWResetRelease();

        /* input clock check:to configurate dphy Source clock detection module */
        if(HI_SUCCESS != PhyHWInputClockCheck(&u32PixelClk))
        {
            HDMI_INFO("input clock unstable\n");
        }

        /* inital: finish aphy, dphy configuration, configurate the register no change with standard */
        PhyHWInitset(pstTmdsCfg);

        stPhyClkSet.stTmdsCfg.enModeCfg   = pstTmdsCfg->enModeCfg;
        stPhyClkSet.stTmdsCfg.enDeepColor = pstTmdsCfg->enDeepColor;
        stPhyClkSet.stTmdsCfg.u32PixelClk = pstTmdsCfg->u32PixelClk;
        stPhyClkSet.stTmdsCfg.u32TmdsClk  = pstTmdsCfg->u32TmdsClk;

        /* calculate pLL parameter */
        if(HI_SUCCESS != HAL_HDMI_ClkSetParaGet(&stPhyClkSet))
        {
            HDMI_INFO("Init para get fail\n");
            return HI_FAILURE;
        }

        if(HDMI_PHY_MODE_CFG_TMDS == pstTmdsCfg->enModeCfg)
        {
            /* pll Feedback clock divider */
            if(HI_SUCCESS != PhyHWFractionalMnxGet(stPhyClkSet.stTmdsCfg.u32TmdsClk,
                                               stPhyClkSet.stTmdsCfg.u32PixelClk, stPhyClkSet.stTmdsCfg.enDeepColor)) {
                HDMI_INFO("MNX get fail\n");
                return HI_FAILURE;
            }
        }

        /* inital:PHY clock configuration */
        PhyHWClockSet(&stPhyClkSet);

        stFconMode.enFconMode = PHY_FCON_MODE_AUTO;

        /* calculate FCON */
        PhyHWFconCfgParaGet(&stFconMode, stPhyClkSet.stTmdsCfg.u32TmdsClk, pstTmdsCfg);
        /* PLL configuration (FCON configuration) */
        if(HI_SUCCESS != PhyHWFCON(&stFconMode))
        {
            HDMI_INFO("Fcon is failed and You have to restart FCON\n");
        }

        stHdmiSscCfg.stHDMIPhySsc.bSscEnable = 0;
        /* calculate Spread spectrum */
        if(HDMI_PHY_MODE_CFG_TMDS == pstTmdsCfg->enModeCfg)
        {
            HAL_HDMI_PhySscSet(&stHdmiSscCfg);
        }

        /* inital:index */
        PhyHWSpecSet(pstTmdsCfg);

        /* data path enabl e */
        PhyHWPostSetup(pstTmdsCfg);

        /* Then disable output and enable by user. */
        HAL_HDMI_PhyOeSet(HI_FALSE);

        /* status info set */
        HDMI_MEMCPY(&s_stHDMIPhyInfo.stTmdsCfg, pstTmdsCfg, sizeof(HDMI_PHY_TMDS_CFG_S));
    }
#endif

    return HI_SUCCESS;
}

#ifndef HDMI_FPGA_SUPPORT
HI_S32 HAL_HDMI_PhySscSet(HDMI_PHY_SSC_CFG_S * pstHdmiSscCfg)
{
    HI_U16 u16ModD     = 0;
    HI_U16 u16ModN     = 0;
    HI_U32 u32ModDN    = 0;
    HI_U32 u32SscFreq  = 0;
    HI_U32 u32SscAmptd = 0;
    HI_U64 u64ModD     = 0;
    HI_U32 u32Rem      = 0;

    PHY_NULL_CHK(pstHdmiSscCfg);

    if(HI_FALSE == pstHdmiSscCfg->stHDMIPhySsc.bSscEnable) {
    }

    u32SscAmptd = pstHdmiSscCfg->stHDMIPhySsc.stPhySscCfg.u32SscAmptd;
    u32SscFreq = pstHdmiSscCfg->stHDMIPhySsc.stPhySscCfg.u32SscFreq / 1000;
    if (u32SscFreq == 0) {
        return HI_FAILURE;
    }

    if(HI_SUCCESS != PhyHWFractionalMnxGet(pstHdmiSscCfg->u32TmdsClk,
        pstHdmiSscCfg->u32PixClk, pstHdmiSscCfg->enDeepColor)) {
        HDMI_INFO("MNX get fail\n");
    }

    u32ModDN = (HI_U32)HDMI_DO_DIVU64(((HI_U64)s_stMnxGet.u32PllRefClk * 2500), u32SscFreq);

    if(5000 <= u32ModDN % 10000) {
        u16ModN = (HI_U16)(u32ModDN / 10000 + 1);
    } else {
        u16ModN = (HI_U16)(u32ModDN / 10000);
    }

    if (u16ModN == 0) {
        return HI_FAILURE;
    }
    u64ModD = (HI_U64)HDMI_DO_DIVU64(((HI_U64)PhyHWPow(2, 24) * s_stMnxGet.u32MnValue * u32SscAmptd), u16ModN);

    HDMI_DIVU64_REM(u64ModD, 100000, &u32Rem);

    if(50000 <= u32Rem) {
        u16ModD = (HI_U16)(HDMI_DO_DIVU64(u64ModD, 100000) + 1);
    } else {
        u16ModD = (HI_U16)HDMI_DO_DIVU64(u64ModD, 100000);
    }

     HDMI_INFO("u16ModN = %x, u16ModD = %x \n", u16ModN, u16ModD);

    if(pstHdmiSscCfg->stHDMIPhySsc.bSscEnable) {
#if defined(HDMI_PRODUCT_HI3559A)
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_D, 0xFD);
#else
        PhyHWWriteStb1Byte(APHY_CS_8, APHY_OFFSET_D, 0xFF);
#endif

        /* MOD_N MOD_T */
        HDMI_FDIVSET3_mod_lenSet(0x0);
        HDMI_FDIVSET3_mod_tSet(0x1);
        HDMI_FDIVSET3_mod_nSet(u16ModN);
        /* MOD_D */
        HDMI_FDIVSET4_mod_dSet(u16ModD);
        /* FDIV init */
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(0);
        HDMI_FDIVSET0_en_modSet(0);
        HDMI_FDIVSET0_en_sdmSet(0);
        /* FDIV init */
        HDMI_FDIVSET0_initSet(1);
        HDMI_UDELAY(1); /* 150 nsec  */
        /* FDIV control */
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(0);
        HDMI_FDIVSET0_en_modSet(0);
        HDMI_FDIVSET0_en_sdmSet(0);
        HDMI_UDELAY(1); /* 150 nsec  */
        /* FDIV control */
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(1);
        HDMI_FDIVSET0_en_modSet(0);
        HDMI_FDIVSET0_en_sdmSet(0);
        /* FDIV control */
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(1);
        HDMI_FDIVSET0_en_modSet(1);
        HDMI_FDIVSET0_en_sdmSet(0);
        /* FDIV control */
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(1);
        HDMI_FDIVSET0_en_modSet(1);
        HDMI_FDIVSET0_en_sdmSet(1);
     }
     else
     {
        /* FDIV init */
        HDMI_FDIVSET0_initSet(1);
        HDMI_UDELAY(1);
        HDMI_FDIVSET0_initSet(0);
        HDMI_FDIVSET0_en_ctrlSet(0);
        HDMI_FDIVSET0_en_modSet(0);
     }

    return HI_SUCCESS;
}
#endif

HI_S32 HAL_HDMI_PhySscGet(HI_BOOL * pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    PHY_NULL_CHK(pbEnable);

    if(   HDMI_FDIVSET0_en_modGet()
       && HDMI_FDIVSET0_en_ctrlGet()
       && HDMI_FDIVSET0_en_sdmGet() ) {
        *pbEnable = HI_TRUE;
    } else {
        *pbEnable = HI_FALSE;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyInfoGet(HDMI_PHY_INFO_S *pstPhyStatus)
{
    PHY_NULL_CHK(pstPhyStatus);

    /* status information obtain */
    HDMI_MEMCPY(pstPhyStatus, &s_stHDMIPhyInfo, sizeof(HDMI_PHY_INFO_S));

    return HI_SUCCESS;
}
