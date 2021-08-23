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
#include "hdmi_hal_intf.h"

#include "hdmi_reg_crg.h"
#include "hdmi_product_define.h"

#include "boot_hdmi_intf.h"
#include "ubi_uboot.h"
#include "malloc.h"

typedef HI_VOID* HDMI_HANDLE;


#define INTF_NULL_CHK(p) do{\
    if(HI_NULL == p) {\
        HDMI_ERR("%s is null pointer!return fail.\n",#p);\
        return HI_FAILURE;\
    }\
}while(0)

#define INTF_NULL_CHK_NO_RET(p) do{\
    if(HI_NULL == p) {\
        HDMI_ERR("%s is null pointer!return fail.\n",#p);\
        return;\
    }\
}while(0)


#define INTF_INIT_CHK(pstHalCfg) do{\
    INTF_NULL_CHK(pstHalCfg);\
    if(HI_TRUE != pstHalCfg->bInit) {\
        HDMI_ERR("hal intf module didn't init!return fail.\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define INTF_INIT_CHK_NO_RET(pstHalCfg) do{\
    INTF_NULL_CHK_NO_RET(pstHalCfg);\
    if(HI_TRUE != pstHalCfg->bInit) {\
        HDMI_ERR("hal intf module didn't init!return fail.\n");\
        return;\
    }\
}while(0)

#define HDMI_UINDEPTH_2_KINDEPTH(uindepth, kindepth) \
do{\
    if (HDMI_VIDEO_BITDEPTH_8 == uindepth) {\
        kindepth = HDMI_DEEP_COLOR_24BIT;\
    } else if (HDMI_VIDEO_BITDEPTH_10 == uindepth) {\
        kindepth = HDMI_DEEP_COLOR_30BIT;\
    } else if (HDMI_VIDEO_BITDEPTH_12 == uindepth)  {\
        kindepth = HDMI_DEEP_COLOR_36BIT;\
    } else if (HDMI_VIDEO_BITDEPTH_16 == uindepth)  {\
        kindepth = HDMI_DEEP_COLOR_48BIT;\
    } else if (HDMI_VIDEO_BITDEPTH_OFF == uindepth) {\
        kindepth = HDMI_DEEP_COLOR_OFF;\
    } else {\
        kindepth = HDMI_DEEP_COLOR_OFF;\
    }\
}while(0)

#define HALTRAINSTATUS_2_INTFTRAINSTATUS(HalTrainStatus, IntfTrainStatus) \
do {     \
    if (HalTrainStatus == FRL_TRAIN_FAIL) {\
        IntfTrainStatus = HDMI_FRL_TRAIN_FAIL;\
    } else if (HalTrainStatus == FRL_TRAIN_SUCCESS) {\
        IntfTrainStatus = HDMI_FRL_TRAIN_SUCCESS;\
    } else if (HalTrainStatus == FRL_TRAIN_BUSY) {\
        IntfTrainStatus = HDMI_FRL_TRAIN_BUSY;\
    }\
}while(0)

#define HALTRAINFAIL_2_INTFTRAINFAIL(HalTrainFail, IntfTrainFail) \
do {      \
    if (HalTrainFail == FRL_TRAIN_FAIL_RES_FLTTIMEOUT) {\
        IntfTrainFail = HDMI_FRL_TRAIN_FAIL_FLTTIMEOUT;\
    } else if (HalTrainFail == FRL_TRAIN_FAIL_RES_FLTSTEPTIMEOUT) {\
        IntfTrainFail = HDMI_FRL_TRAIN_FAIL_FLTSTEPTIMEOUT;\
    } else if (HalTrainFail == FRL_TRAIN_FAIL_RES_RATECHANGE) {\
        IntfTrainFail = HDMI_FRL_TRAIN_FAIL_RATECHANGE;\
    } else if (HalTrainFail == FRL_TRAIN_FAIL_RES_FFECHANGE) {\
        IntfTrainFail = HDMI_FRL_TRAIN_FAIL_FFECHANGE;\
    } else if (HalTrainFail == FRL_TRAIN_FAIL_RES_NORMAL) {\
        IntfTrainFail = HDMI_FRL_TRAIN_FAIL_NORMAL;\
    }\
}while(0)

#define HALTRAINPTN_2_INTFTRAINPTN(HalTrainPtn, IntfTrainPTN) \
do {     \
    if (HalTrainPtn == FRL_TRAIN_PATTERN_NONE) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_NONE;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP1) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP1;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP2) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP2;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP3) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP3;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP4) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP4;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP5) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP5;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP6) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP6;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP7) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP7;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_LP8) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_LP8;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_0E) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_0E;\
    } else if (HalTrainPtn == FRL_TRAIN_PATTERN_0F) {\
        IntfTrainPTN = HDMI_FRL_TRAIN_PATTERN_0F;\
    }\
}while(0)

static HDMI_HAL_CFG_S s_stHalCfg[HDMI_DEVICE_ID_BUTT] ;

static HDMI_HAL_CFG_S *IntfHalInfoPtrGet(HDMI_DEVICE_ID_E enHdmi)
{
    if (enHdmi < HDMI_DEVICE_ID_BUTT) {
        return &s_stHalCfg[enHdmi];
    }
    return HI_NULL;
}

static HI_S32 IntfTxCapabilityInit(HDMI_DEVICE_ID_E enHdmiId)
{
    HDMI_HAL_CFG_S *pstHalCfg = IntfHalInfoPtrGet(enHdmiId);

    INTF_NULL_CHK(pstHalCfg);

    /* Tx Capability */
    pstHalCfg->stTxCapability.bTxHdmi_14      = HI_TRUE;
    pstHalCfg->stTxCapability.bTxHdmi_20      = HI_TRUE;

    pstHalCfg->stTxCapability.bTxHdmi_21      = HI_TRUE;

    pstHalCfg->stTxCapability.bTxHdcp_14      = HI_TRUE;
    pstHalCfg->stTxCapability.bTxHdcp_22      = HI_TRUE;

    pstHalCfg->stTxCapability.bTxRGB444       = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444     = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422     = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR420     = HI_TRUE;

    pstHalCfg->stTxCapability.bTxDeepClr10Bit = HI_TRUE;
    pstHalCfg->stTxCapability.bTxDeepClr12Bit = HI_TRUE;
    pstHalCfg->stTxCapability.bTxDeepClr16Bit = HI_FALSE;

    pstHalCfg->stTxCapability.bTxRGB_YCbCR444 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444_422 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422_420 = HI_TRUE;

    pstHalCfg->stTxCapability.bTxYCBCR420_422 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422_444 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444_RGB = HI_TRUE;

    pstHalCfg->stTxCapability.bTxScdc         = HI_TRUE;
    pstHalCfg->stTxCapability.u32TxMaxTmdsClk = 600; /* in MHz */

    return HI_SUCCESS;
}

HI_VOID HAL_HDMI_HardwareInit(struct hiHDMI_HAL_S *pstHdmiHal)
{
    DRV_HDMI_ProdCrgInit();
    return;
}

HI_VOID HAL_HDMI_TmdsModeSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TMDS_MODE_E enTmdsMode)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    HAL_HDMI_CtrlTmdsModeSet(pstHdmiHal->stHalCtx.u32HdmiID,enTmdsMode);
    return;
}

HI_VOID HAL_HDMI_AvMuteSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAvMute)
{
    HDMI_AVMUTE_CFG_S stAvmuteCfg = {0};

    INTF_NULL_CHK_NO_RET(pstHdmiHal);

    HDMI_MEMSET(&stAvmuteCfg,0,sizeof(HDMI_AVMUTE_CFG_S));
    HAL_HDMI_CtrlAvmuteSet(pstHdmiHal->stHalCtx.u32HdmiID,&stAvmuteCfg);

    return;
}

HI_VOID HAL_HDMI_CscParamSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
    /* colorspace(in & out), Quantization, CONVERSION_STD */
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstVideoCfg);

    HAL_HDMI_CtrlCscSet(pstHdmiHal->stHalCtx.u32HdmiID, pstVideoCfg);

    pstHdmiHal->stHalCtx.stVideoCfg.enInColorSpace  = pstVideoCfg->enInColorSpace;
    pstHdmiHal->stHalCtx.stVideoCfg.enOutColorSpace = pstVideoCfg->enOutColorSpace;
    pstHdmiHal->stHalCtx.stVideoCfg.eOutCscQuantization = pstVideoCfg->eOutCscQuantization;
    pstHdmiHal->stHalCtx.stVideoCfg.enConvStd = pstVideoCfg->enConvStd;

    return;
}


HI_VOID HAL_HDMI_InfoframeSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8InBuffer)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pu8InBuffer);

    HAL_HDMI_CtrlInfoFrameDataSet(pstHdmiHal->stHalCtx.u32HdmiID,enInfoFrameId,pu8InBuffer);
    return;
}


HI_VOID HAL_HDMI_InfoframeEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    HAL_HDMI_CtrlInfoFrameEnSet(pstHdmiHal->stHalCtx.u32HdmiID,enInfoFrameId,bEnable);
    return;
}

HI_S32 HAL_HDMI_VideoPathSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    HDMI_VIDEO_PATH_S       stVideoPath = {0};
    HDMI_HAL_CFG_S          *pstHalCfg = HI_NULL;
    HDMI_PHY_TMDS_CFG_S     stPhyTmds = {0};

    HDMI_MEMSET(&stPhyTmds, 0, sizeof(HDMI_PHY_TMDS_CFG_S));

    INTF_NULL_CHK(pstHdmiHal);
    INTF_NULL_CHK(pstVideoCfg);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK(pstHalCfg);
    pstHalCfg->u32TmdsClk = pstVideoCfg->u32TmdsClk;

    stVideoPath.enInColorSpace              = pstVideoCfg->enInColorSpace;
    stVideoPath.enOutColorSpace             = pstVideoCfg->enOutColorSpace;

    HDMI_UINDEPTH_2_KINDEPTH(pstVideoCfg->enInBitDepth, stVideoPath.enInDeepColor);
    stVideoPath.enOutDeepColor              = pstVideoCfg->enDeepColor;

    stVideoPath.stOutHvSyncPol.bDeInver     = pstVideoCfg->bDEPol;
    stVideoPath.stOutHvSyncPol.bHPolInver   = pstVideoCfg->bHSyncPol;
    stVideoPath.stOutHvSyncPol.bVPolInver   = pstVideoCfg->bVSyncPol;

    switch (pstVideoCfg->enConvStd) {
        case HDMI_CONV_STD_BT_601:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT601;
            break;
        case HDMI_CONV_STD_BT_709:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT709;
            break;
        case HDMI_CONV_STD_BT_2020_non_const_luminous:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT2020;
            break;
        case HDMI_CONV_STD_BT_2020_const_luminous:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT2020_CONST;
            break;

        default:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT709;
            break;

    }
    stVideoPath.enInColormetry = stVideoPath.enOutColormetry;
    stVideoPath.enInQuantization    = pstVideoCfg->eOutCscQuantization; /* should set according to the sink;should sync 98cv200 */
    stVideoPath.enOutQuantization   = pstVideoCfg->eOutCscQuantization;
    stVideoPath.u32InPixelClk       = pstVideoCfg->u32PixelClk;
    stVideoPath.u32OutTmdsClk       = pstVideoCfg->u32TmdsClk;  /* video path no use */
    stVideoPath.enTimming           = pstVideoCfg->enTimming;

    s32Ret |= HAL_HDMI_CtrlVideoPathSet(pstHdmiHal->stHalCtx.u32HdmiID,&stVideoPath);

    /* PHY config */
    stPhyTmds.bEmiEnable    = pstVideoCfg->bEmiEnable;
    stPhyTmds.enDeepColor   = pstVideoCfg->enDeepColor;
    stPhyTmds.u32PixelClk   = pstVideoCfg->u32PixelClk;
    stPhyTmds.u32TmdsClk    = pstVideoCfg->u32TmdsClk;
    stPhyTmds.enModeCfg     = HDMI_PHY_MODE_CFG_TMDS;
    stPhyTmds.enTraceLen    = HDMI_TRACE_LEN_1;
    s32Ret |= HAL_HDMI_PhyTmdsSet(&stPhyTmds);

    return s32Ret;
}

HI_VOID HAL_HDMI_CtrlReset(struct hiHDMI_HAL_S *pstHdmiHal)
{
    HI_BOOL bOutput = HI_FALSE;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);

    HAL_HDMI_PhyOeGet(&bOutput);

    if (HI_FALSE == bOutput) {
        HDMI_INFO("oe enable, do not reset!\n");
        HAL_HDMI_CtrlDataReset(pstHdmiHal->stHalCtx.u32HdmiID);
    }
}

HI_VOID HAL_HDMI_PhyOutputEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);

#ifndef HDMI_FPGA_SUPPORT
    HAL_HDMI_PhyOeSet(bEnable);
#endif
    return;
}

HI_VOID HAL_HDMI_PhyPowerEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_BOOL bOldEable = HI_FALSE;

    HAL_HDMI_PhyPowerGet(&bOldEable);
    if(bEnable != bOldEable) {
        HAL_HDMI_PhyPowerSet(bEnable);
    }
#endif
    return;
}

HI_VOID HAL_HDMI_PhySet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_PHY_CFG_S *pstPhyCfg)
{
#ifndef HDMI_FPGA_SUPPORT
    HDMI_PHY_TMDS_CFG_S stCfg = {0};

    INTF_NULL_CHK_NO_RET(pstPhyCfg);

    HDMI_MEMCPY(&stCfg, pstPhyCfg, sizeof(HDMI_PHY_TMDS_CFG_S));

    HAL_HDMI_PhyTmdsSet(&stCfg);
#endif
    return;
}

HI_VOID HAL_HDMI_TxCapabilityGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TX_CAPABILITY_S *pstTxCapability)
{
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstTxCapability);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK_NO_RET(pstHalCfg);
    INTF_INIT_CHK_NO_RET(pstHalCfg);
    HDMI_MEMCPY(pstTxCapability,&pstHalCfg->stTxCapability,sizeof(HDMI_TX_CAPABILITY_S));
    return;
}

HI_U32 *HAL_HDMI_BaseAddrGet(struct hiHDMI_HAL_S *pstHdmiHal)
{
    HI_U32 *pu32BaseAddr = HI_NULL;

    pu32BaseAddr = (HI_U32 *)HDMI_CTRL_BASE_ADDR;

    return pu32BaseAddr;
}

#ifdef HDMI_SCDC_SUPPORT

HI_VOID HAL_HDMI_ScdcStatusSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus)
{
    SCDC_ATTR_S stScdcAttr = {0};

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstScdcStatus);
    stScdcAttr.bSinkReadQuest       = HI_FALSE;
    stScdcAttr.bSinkScramble        = pstScdcStatus->bSinkScrambleOn;
    stScdcAttr.bSrcScramble         = pstScdcStatus->bSourceScrambleOn;
    stScdcAttr.bTmdsClkRatio40x     = pstScdcStatus->u8TmdsBitClkRatio==40 ? HI_TRUE : HI_FALSE;
    stScdcAttr.u32ScrambleInterval  = pstScdcStatus->u32ScrambleInterval;
    stScdcAttr.u32ScrambleTimeout   = pstScdcStatus->u32ScrambleTimeout;
    HAL_HDMI_ScdcAttrSet(pstHdmiHal->stHalCtx.u32HdmiID, &stScdcAttr);
    return;
}

HI_VOID HAL_HDMI_ScdcStatusGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus)
{
    SCDC_ATTR_S stScdcAttr = {0};

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstScdcStatus);
    HAL_HDMI_ScdcAttrGet(pstHdmiHal->stHalCtx.u32HdmiID, &stScdcAttr);
    pstScdcStatus->bSinkReadQuest       = HI_FALSE;
    pstScdcStatus->bSinkScrambleOn      = stScdcAttr.bSinkScramble;
    pstScdcStatus->bSourceScrambleOn    = stScdcAttr.bSrcScramble;
    pstScdcStatus->u8TmdsBitClkRatio    = stScdcAttr.bTmdsClkRatio40x ? 40 : 10;
    pstScdcStatus->u32ScrambleInterval  = stScdcAttr.u32ScrambleInterval;
    pstScdcStatus->u32ScrambleTimeout   = stScdcAttr.u32ScrambleTimeout;

    return;
}

HI_VOID HAL_HDMI_ScdcConfig(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_CONFIG_S* pstScdcConfig)
{
    return;
}

HI_S32 HAL_HDMI_ScdcProcess(struct hiHDMI_HAL_S *pstHdmiHal,
                                SCDC_CMD_E enScdcCmd, HI_VOID* pData)
{
    INTF_NULL_CHK(pstHdmiHal);
    INTF_NULL_CHK(pData);

    return HAL_HDMI_ScdcCtrl(pstHdmiHal->stHalCtx.u32HdmiID,
                                 enScdcCmd, pData);
}
#endif

/*******************************define_start**********************************************************
Function:
int HAL_HDMI_Init(HMDI_CALLBACK_FUNC pCallback, HDMI_HAL_S** pstHdmiHalCtrl)
* Description:  open HDMI HAL
* Input:        pCallback: event callback
* Output:       **pstHdmiHalCtrl: hal global struct pointer
* Return:       HI_SUCCESS / ERR CODE
* Others:       NONE
************************************end****************************************************************/
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle)
{
    HDMI_HAL_S  *pstHdmiHal = HI_NULL;
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    pstHdmiHal = (HDMI_HAL_S *)HDMI_VMALLOC(sizeof(HDMI_HAL_S));
    if (pstHdmiHal == HI_NULL) {
        HDMI_ERR("Alloc HDMI_HAL_S struct memory fail\n");
        return HI_FAILURE;
    }

    HDMI_MEMSET(pstHdmiHal, 0, sizeof(HDMI_HAL_S));

    if (pstHalInit) {
        pstHdmiHal->stHalCtx.pCallback  = pstHalInit->pfnEventCallBack;
        pstHdmiHal->stHalCtx.hHdmiDev   = pstHalInit->pvEventData;
        pstHdmiHal->stHalCtx.u32HdmiID  = pstHalInit->u32HdmiDevId;
        pstHdmiHal->stHalCtx.pcBaseAddr = pstHalInit->pcBaseAddr;
        pstHdmiHal->stHalCtx.pcPhyAddr  = pstHalInit->pcPhyAddr;
    } else {
        HDMI_INFO("pstHalInit null! open in boot!\n");
    }

    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    if (pstHalCfg == HI_NULL) {
        HDMI_VFREE(pstHdmiHal);
        HDMI_ERR("pstHalCfg null,fail!\n");
        return HI_FAILURE;
    }

    IntfTxCapabilityInit(pstHdmiHal->stHalCtx.u32HdmiID);

#ifndef HDMI_FPGA_SUPPORT
    HAL_HDMI_PhyInit(pstHalInit);
#endif

    HAL_HDMI_CtrlInit(pstHdmiHal->stHalCtx.u32HdmiID, pstHalInit);
    HAL_HDMI_DdcInit(pstHdmiHal->stHalCtx.u32HdmiID);

#ifdef HDMI_SCDC_SUPPORT
    HAL_HDMI_ScdcInit(pstHdmiHal->stHalCtx.u32HdmiID);
#endif

    pstHdmiHal->HAL_HDMI_HardwareInit            = HAL_HDMI_HardwareInit;
    pstHdmiHal->HAL_HDMI_AvMuteSet               = HAL_HDMI_AvMuteSet;
    pstHdmiHal->HAL_HDMI_TmdsModeSet             = HAL_HDMI_TmdsModeSet;
    pstHdmiHal->HAL_HDMI_InfoframeSet            = HAL_HDMI_InfoframeSet;
    pstHdmiHal->HAL_HDMI_InfoframeEnableSet      = HAL_HDMI_InfoframeEnableSet;
    pstHdmiHal->HAL_HDMI_VideoPathSet            = HAL_HDMI_VideoPathSet;
    pstHdmiHal->HAL_HDMI_PhyPowerEnableSet       = HAL_HDMI_PhyPowerEnableSet;
    pstHdmiHal->HAL_HDMI_PhyOutputEnableSet      = HAL_HDMI_PhyOutputEnableSet;
    pstHdmiHal->HAL_HDMI_TxCapabilityGet         = HAL_HDMI_TxCapabilityGet;
    pstHdmiHal->HAL_HDMI_CscParamSet             = HAL_HDMI_CscParamSet;
    pstHdmiHal->HAL_HDMI_PhySet                  = HAL_HDMI_PhySet;
    pstHdmiHal->HAL_HDMI_CtrlReset               = HAL_HDMI_CtrlReset;

#ifdef HDMI_SCDC_SUPPORT
    pstHdmiHal->HAL_HDMI_ScdcConfig              = HAL_HDMI_ScdcConfig;
    pstHdmiHal->HAL_HDMI_ScdcStatusGet           = HAL_HDMI_ScdcStatusGet;
    pstHdmiHal->HAL_HDMI_ScdcStatusSet           = HAL_HDMI_ScdcStatusSet;
    pstHdmiHal->HAL_HDMI_ScdcProcess             = HAL_HDMI_ScdcProcess;
#endif

    *pstHalHandle = pstHdmiHal;
    pstHalCfg->bInit = HI_TRUE;

    return HI_SUCCESS;
}

/*******************************define_start**********************************************************
Function:
void HAL_HDMI_Deinit(HDMI_HAL_S* pstHdmiHal)
* Description:  free HDMI HAL
* Input:        *pstHdmiHal:hal global structure
* Output:       none
* Return:       none
* Others:       none
************************************end****************************************************************/
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal)
{
    HDMI_HAL_CFG_S *pstHalCfg = (HDMI_HAL_CFG_S *)HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK_NO_RET(pstHalCfg);

#ifdef HDMI_SCDC_SUPPORT
    HAL_HDMI_ScdcDeinit(pstHdmiHal->stHalCtx.u32HdmiID);
#endif
    HAL_HDMI_DdcDeInit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_CtrlDeinit(pstHdmiHal->stHalCtx.u32HdmiID);

#ifndef HDMI_FPGA_SUPPORT
    HAL_HDMI_PhyDeInit();
#endif

    REG_HDMI_CRG_reg_DeInit();

    pstHdmiHal->stHalCtx.pcBaseAddr = HI_NULL;
    pstHdmiHal->stHalCtx.pcPhyAddr  = HI_NULL;
    HDMI_MEMSET(pstHdmiHal, 0, sizeof(HDMI_HAL_S));
    HDMI_VFREE(pstHdmiHal);

    pstHalCfg->bInit = HI_FALSE;

    return;
}

