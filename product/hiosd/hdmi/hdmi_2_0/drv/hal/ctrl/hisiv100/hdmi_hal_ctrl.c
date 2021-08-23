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

#include "hdmi_reg_tx.h"
#include "hdmi_reg_video_path.h"
#include "hdmi_reg_aon.h"
#include "hdmi_reg_ctrl.h"

#include "hdmi_hal_ddc.h"
#include "hdmi_hal_ctrl.h"
#include "hdmi_hal_intf.h"

#include "hdmi_reg_crg.h"
#include "hdmi_product_define.h"

#define CTRL_SET_BIT(var,bit)   do{var |= 1 << bit;}while(0)
#define CTRL_CLR_BIT(var,bit)   do{var &= ~(1 << bit);}while(0)
#define IS_BIT_SET(var,bit)     ({(var) & (0x1<<bit) ? HI_TRUE : HI_FALSE ;})

#define CTRL_NULL_CHK(p) do{\
    if(HI_NULL == p) {\
        HDMI_WARN("null pointer!\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define CTRL_INIT_CHK(pstCtrlInfo) do{\
    CTRL_NULL_CHK(pstCtrlInfo);\
    if(pstCtrlInfo->bInit != HI_TRUE) {\
        HDMI_WARN("hdmi ctrl module didn't init!\n");\
        return HI_FAILURE;\
    }\
}while(0)

/********************** VIDEO DEFINE *********************************/

#define CTRL_CHANNEL0_Y                 0x0
#define CTRL_CHANNEL0_Y422              0x3
#define CTRL_CHANNEL1_Cb                0x1
#define CTRL_CHANNEL1_Y422              0x4
#define CTRL_CHANNEL2_Cr                0x2
#define CTRL_CHANNEL2_Y422              0x3

#define CTRL_COLORMETRY_OUT_MASK        0xfc
#define CTRL_COLORMETRY_OUT_BIT         (4-4)
#define CTRL_COLORMETRY_IN_MASK         0xcf
#define CTRL_COLORMETRY_IN_BIT          (8-4)
#define CTRL_COLORMETRY_MASK            0x3

#define CTRL_RGB_OUT_BIT                (7-4)
#define CTRL_RGB_IN_BIT                 (11-4)

#define CTRL_QUANTIZAION_OUT_BIT        (6-4)
#define CTRL_QUANTIZAION_IN_BIT         (10-4)

#define CTRL_SYCN_POL_V_BIT             (25-25)
#define CTRL_SYCN_POL_H_BIT             (26-25)
#define CTRL_SYCN_POL_DE_BIT            (28-25)

#define CTRL_BLACK_Y_Cb_Cr              0x000000

#define CTRL_BLACK_DATA_YUV_CR      (0x80<<2)
#define CTRL_BLACK_DATA_YUV_Y       (0x10<<2)
#define CTRL_BLACK_DATA_YUV_CB      (0x80<<2)

#define CTRL_BLACK_DATA_RGB_R       (0x10<<2)
#define CTRL_BLACK_DATA_RGB_G       (0x10<<2)
#define CTRL_BLACK_DATA_RGB_B       (0x10<<2)


typedef struct {
    HI_BOOL             bVerDownSampBypass;
    HI_BOOL             bVerDownSampEnable;
    HI_BOOL             bHorDownSampEnable;
    HI_BOOL             bDemuxY420Enalbe;
    HI_BOOL             bPxlDivEnable;
    HI_BOOL             bHorFilterEnable;
}HDMI_VIDOE_DOWNSAMP_S;


/********************** AUDIO DEFINE *********************************/

#define CTRL_AUDIO_INVALID_CFG          0xff
#define CTRL_AUDIO_INVALID_RATE         0xffffffff




typedef enum{

    HDMI_AUDIO_FIFO_MAP_SD0 = 0x0,
    HDMI_AUDIO_FIFO_MAP_SD1 = 0x1,
    HDMI_AUDIO_FIFO_MAP_SD2 = 0x2,
    HDMI_AUDIO_FIFO_MAP_SD3 = 0x3,
    HDMI_AUDIO_FIFO_MAP_BUTT,

}HDMI_AUDIO_FIFO_MAP_E;


typedef enum{
    HDMI_AUDIO_I2S_SD_NONE  = 0x0,
    HDMI_AUDIO_I2S_SD_0     = 0x1,
    HDMI_AUDIO_I2S_SD_2     = 0x2,
    HDMI_AUDIO_I2S_SD_3     = 0x4,
    HDMI_AUDIO_I2S_SD_4     = 0x8,
    HDMI_AUDIO_I2S_SD_ALL   = 0xf,
}HDMI_AUDIO_I2S_SD_E;

typedef enum {
    HDMI_AUDIO_HBRA_MASK_ALL    = 0x0,
    HDMI_AUDIO_HBRA_MASK_NONE   = 0xf,

}CTRL_I2S_HBRA_MASK_E;



typedef enum {
    HDMI_AUDIO_CLK_ACCURACY_LEVEL2 = 0x0,
    HDMI_AUDIO_CLK_ACCURACY_LEVEL1 ,
    HDMI_AUDIO_CLK_ACCURACY_LEVEL3 ,
    HDMI_AUDIO_CLK_ACCURACY_NOTMATCH
}HDMI_AUDIO_CLK_ACCURACY_E;

typedef struct {
    HI_BOOL                 bTest;
    CTRL_I2S_HBRA_MASK_E    enHbraMask;
}CTRL_FIFO_CTRL_S;

typedef struct {
    /* cfg0 */
    HDMI_AUDIO_CLK_ACCURACY_E   enClkAccuracy ;
    HI_BOOL                     bProfessionalApp ;
    HI_BOOL                     bCompress;
    HI_U32                      u32SampRate;

    /* cfg1 */
    HI_U32                      u32OrgRate;
    HI_U32                      u32OutBitLength;

    HDMI_AUDIO_FIFO_MAP_E       enFifoMap;
}CTRL_I2S_STATUS_S;

typedef struct {
    HI_BOOL     bChannelLRSwap;
    HI_U32      u32InBitLength;
    HI_BOOL     bVbitCompress;
    HI_BOOL     bDataDirMsbFirst;
    HI_BOOL     bJustifyWsRight;
    HI_BOOL     bPolarityWsMsbLeft;
    HI_BOOL     bSckShiftOne;
    HI_BOOL     bHbraEnable;

}CTRL_I2S_CTRL_S;

typedef struct {
    HI_U32      u32BitValue;
    HI_U32      u32BitReg;
}HDMI_AUDIO_SAMP_BIT_S;


typedef struct {
    HI_U32      u32RateValue;
    HI_U32      u32RateReg;
    HI_U32      u32RateOrgReg;
}HDMI_I2S_SAMP_RATE_S;

typedef struct {
    HI_U32      u32RateValue;
    HI_U32      u32RateReg;
}HDMI_SPDIF_SAMP_RATE_S;

static HDMI_CTRL_INFO_S    s_stCtrlInfo[HDMI_DEVICE_ID_BUTT];

static HDMI_CTRL_INFO_S *CtrlInfoPtrGet(HDMI_DEVICE_ID_E enHdmi)
{
    if (enHdmi < HDMI_DEVICE_ID_BUTT) {
        return &s_stCtrlInfo[enHdmi];
    }
    return HI_NULL;
}

static HI_VOID CtrlAvmuteSet(HDMI_AVMUTE_CFG_S *pstAvmuteCfg)
{
    if(!pstAvmuteCfg) {
        HDMI_ERR("pstAvmuteCfg null!\n");
        return ;
    }

    HDMI_CEA_CP_CFG_cea_cp_enSet(HI_FALSE);

    /* set GCP set mute & clr mute  */
    HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(pstAvmuteCfg->bMuteSet);
    HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(pstAvmuteCfg->bMuteClr);

    /* set GCP avmute funtion enable,sent loop  */
    HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(pstAvmuteCfg->u32RptCnt);
    HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(pstAvmuteCfg->bMuteRptEn);

    HDMI_CEA_CP_CFG_cea_cp_enSet(pstAvmuteCfg->bMutePkgEn);

    return ;
}


static HI_S32 CtrlAvmuteGet(HI_BOOL *pbEnable)
{

    CTRL_NULL_CHK(pbEnable);

    *pbEnable = HI_FALSE;

    if(HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet()
    || HDMI_CEA_CP_CFG_cea_cp_rpt_enGet()) {
        if (HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet()) {
            *pbEnable = HI_TRUE;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID CtrlVideoPathDeepClrSet(HI_BOOL bEnalbeGcp,HDMI_DEEP_COLOR_E enDeepColorBit)
{
    HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(enDeepColorBit);
    HDMI_AVMIXER_CONFIG_dc_pkt_enSet(bEnalbeGcp);
    return ;
}

static HI_S32 CtrlVideoPathPolaritySet(HDMI_SYNC_POL_S *pstOutSyncPol)
{
    HI_U32  u32SyncPolCfg = 0;

    CTRL_NULL_CHK(pstOutSyncPol);

    if (pstOutSyncPol->bHPolInver) {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_H_BIT);
    } else {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_H_BIT);
    }

    if (pstOutSyncPol->bVPolInver) {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_V_BIT);
    } else {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_V_BIT);
    }

    if (pstOutSyncPol->bDeInver) {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_DE_BIT);
    } else {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_DE_BIT);
    }

    HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(u32SyncPolCfg);
    HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(HI_FALSE);

    return HI_SUCCESS;

}

#if defined(HDMI_PRODUCT_HI3519A)
static HI_VOID CtrlVideoPathLowPowerSet(HI_BOOL bEnable)
{
    return ;
}
#endif

static HI_VOID CtrlVideoPathDitherSet(HI_BOOL bEnable,HDMI_VIDEO_DITHER_E enDitherMode)
{
    HDMI_DITHER_CONFIG_dither_rnd_bypSet((!bEnable));
    HDMI_DITHER_CONFIG_dither_modeSet(enDitherMode);
    return ;
}

static HI_VOID CtrlVideoColorRgbSet(HI_BOOL bRgbIn,HI_BOOL bRgbOut)
{
    HI_U32  u32RegValue  = 0;

    u32RegValue = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    if (bRgbIn) {
        CTRL_SET_BIT(u32RegValue,CTRL_RGB_IN_BIT);
    } else {
        CTRL_CLR_BIT(u32RegValue,CTRL_RGB_IN_BIT);
    }

    if (bRgbOut) {
        CTRL_SET_BIT(u32RegValue,CTRL_RGB_OUT_BIT);
    } else {
        CTRL_CLR_BIT(u32RegValue,CTRL_RGB_OUT_BIT);
    }
    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32RegValue);

    return ;
}

static HI_VOID CtrlVideoColorCscSet(HI_BOOL bCscEnable)
{
    HDMI_MULTI_CSC_CTRL_reg_csc_enSet(bCscEnable);
    return ;
}

static HI_VOID CtrlVideoColorYCbCr422Set(HI_BOOL bYCbCr422)
{
    if (bYCbCr422) {
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(CTRL_CHANNEL0_Y422);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(CTRL_CHANNEL1_Y422);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(CTRL_CHANNEL2_Y422);
    } else {
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(CTRL_CHANNEL0_Y);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(CTRL_CHANNEL1_Cb);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(CTRL_CHANNEL2_Cr);
    }

    return ;
}

static HI_VOID CtrlVideoColorYCbCr420Set(HI_BOOL bYCbCr420)
{
    HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(bYCbCr420);
    HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(bYCbCr420);

    return ;
}


static HI_VOID CtrlVideoColorDwsmVertSet(HI_BOOL bDwsmVert)
{
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet(!bDwsmVert);
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(bDwsmVert);

    return ;
}


static HI_VOID CtrlVideoColorDwsmHoriSet(HI_BOOL bDwsmHori)
{
    HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(bDwsmHori);
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(bDwsmHori);

    return ;
}


static HI_VOID CtrlVideoPathColormetrySet(HDMI_COLORMETRY_E enInColormetry,HDMI_COLORMETRY_E enOutColormetry)
{
    HI_U32 u32CscMode = 0;

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    enOutColormetry = (HI_U32)enOutColormetry & CTRL_COLORMETRY_MASK;
    u32CscMode &= CTRL_COLORMETRY_OUT_MASK;
    u32CscMode |= enOutColormetry << CTRL_COLORMETRY_OUT_BIT;

    enInColormetry = (HI_U32)enInColormetry & CTRL_COLORMETRY_MASK;
    u32CscMode &= CTRL_COLORMETRY_IN_MASK;
    u32CscMode |= (HI_U32)enInColormetry << CTRL_COLORMETRY_IN_BIT;

    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32CscMode);

    return ;
}

static HI_VOID CtrlVideoPathColorSpaceSet(HDMI_COLORSPACE_E enInColorSpace,HDMI_COLORSPACE_E enOutColorSpace)
{
    HI_BOOL     bInRgb          = HI_FALSE;
    HI_BOOL     bOutRgb         = HI_FALSE;
    HI_BOOL     bCscEnalbe      = HI_FALSE;
    HI_BOOL     bY422Enalbe     = HI_FALSE;
    HI_BOOL     bY420Enalbe     = HI_FALSE;
    HI_BOOL     bDwsmHoriEnalbe = HI_FALSE;
    HI_BOOL     bDwsmVertEnalbe = HI_FALSE;

    if (enInColorSpace == HDMI_COLORSPACE_YCbCr444 ) {
        switch(enOutColorSpace) {
            case HDMI_COLORSPACE_YCbCr422:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_TRUE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;

            case HDMI_COLORSPACE_YCbCr420:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_TRUE;
                bDwsmVertEnalbe = HI_TRUE;
                break;

            case HDMI_COLORSPACE_RGB:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_TRUE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;

            case HDMI_COLORSPACE_YCbCr444:
            default:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;

        }
    } else if (enInColorSpace == HDMI_COLORSPACE_RGB ) {

        switch(enOutColorSpace) {
            case HDMI_COLORSPACE_YCbCr444:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;

            case HDMI_COLORSPACE_YCbCr422:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_TRUE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;

            case HDMI_COLORSPACE_YCbCr420:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_TRUE;
                bDwsmVertEnalbe = HI_TRUE;
                break;

            case HDMI_COLORSPACE_RGB:
            default:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_TRUE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
                break;


        }
    /* bypass */
    } else if (enInColorSpace == HDMI_COLORSPACE_YCbCr422) {
        bInRgb          = HI_FALSE;
        bOutRgb         = HI_FALSE;
        bCscEnalbe      = HI_FALSE;
        bY422Enalbe     = HI_FALSE;
        bDwsmHoriEnalbe = HI_FALSE;
        bY420Enalbe     = HI_FALSE;
        bDwsmVertEnalbe = HI_FALSE;
    } else {
        HDMI_WARN("un-expected enInColorSpace=%u\n",enInColorSpace);
    }

    HDMI_INFO(  "csc set:\n"
                "bInRgb=%d,bOutRgb=%d,bCscEnalbe=%d \n"
                "bY422Enalbe=%d,bDwsmHoriEnalbe=%d \n"
                "bY420Enalbe=%d,bDwsmVertEnalbe=%d \n",
                bInRgb,bOutRgb,bCscEnalbe,
                bY422Enalbe,bDwsmHoriEnalbe,
                bY420Enalbe,bDwsmVertEnalbe);

    CtrlVideoColorRgbSet(bInRgb,bOutRgb);
    CtrlVideoColorYCbCr422Set(bY422Enalbe);
    CtrlVideoColorDwsmHoriSet(bDwsmHoriEnalbe);
    CtrlVideoColorYCbCr420Set(bY420Enalbe);
    CtrlVideoColorDwsmVertSet(bDwsmVertEnalbe);
    CtrlVideoColorCscSet(bCscEnalbe);
    return ;
}

static HI_VOID CtrlVideoPathQuantizationSet(HDMI_QUANTIZATION_RANGE enInQuantization,HDMI_QUANTIZATION_RANGE enOutQuantization)
{
    HI_U32 u32CscMode = 0;

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    if (HDMI_QUANTIZATION_RANGE_LIMITED != enInQuantization) {
        CTRL_SET_BIT(u32CscMode,CTRL_QUANTIZAION_IN_BIT);
    } else {
        CTRL_CLR_BIT(u32CscMode,CTRL_QUANTIZAION_IN_BIT);
    }

    if (HDMI_QUANTIZATION_RANGE_LIMITED != enOutQuantization) {
        CTRL_SET_BIT(u32CscMode,CTRL_QUANTIZAION_OUT_BIT);
    } else {
        CTRL_CLR_BIT(u32CscMode,CTRL_QUANTIZAION_OUT_BIT);
    }

    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32CscMode);
    HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(HI_TRUE);

    return ;
}

static HI_S32 CtrlVendorInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(bEnable);
    HDMI_CEA_VSIF_CFG_cea_vsif_enSet(bEnable);

    return HI_SUCCESS;
}

static HI_S32 CtrlVendorInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++) {
        CTRL_NULL_CHK(pu8Tmp);
    }

    /* HB config */
    HDMI_VSIF_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    /* pkt0 config */
    HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    /* pkt1 config */
    HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    /* pkt2 config */
    HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    /* pkt3 config */
    HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlAviInfoFrameEnSet(HI_BOOL bEnable)
{
    /* Repeat Enable */
    HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(bEnable);
    /* InfoFrame Enable */
    HDMI_CEA_AVI_CFG_cea_avi_enSet(bEnable);

    return HI_SUCCESS;
}

static HI_S32 CtrlAviInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++) {
        CTRL_NULL_CHK(pu8Tmp);
    }

    /* HB config */
    HDMI_AVI_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlTmdsModeSet(HDMI_TMDS_MODE_E enTmdsMode)
{
    switch(enTmdsMode) {
        case HDMI_TMDS_MODE_DVI:
            HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_FALSE);
#if defined(HDMI_PRODUCT_HI3519A)
            /* in case of HDMI_TMDS_MODE_DVI,hdmi_dvi_sel should be set 0, */
            /* eess_mode_en shoud be set the default value */
            HDMI_AVMIXER_CONFIG_hdmi_dvi_selSet(HI_FALSE);
            HDMI_AVMIXER_CONFIG_eess_mode_enSet(HI_TRUE);
#endif

            break;
        case HDMI_TMDS_MODE_HDMI_1_4:
            HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_TRUE);
            HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(HI_FALSE);
#if defined(HDMI_PRODUCT_HI3519A)
            /* in case of HDMI_TMDS_HDMI_1_4,hdmi_dvi_sel should be set 1, */
            /* eess_mode_en shoud be set 1 */
            HDMI_AVMIXER_CONFIG_hdmi_dvi_selSet(HI_TRUE);
            HDMI_AVMIXER_CONFIG_eess_mode_enSet(HI_TRUE);
#endif
            break;
        case HDMI_TMDS_MODE_HDMI_2_0:
            HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_TRUE);
            HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(HI_TRUE);
            HDMI_HDMI_ENC_CTRL_enc_bypassSet(HI_FALSE);
#if defined(HDMI_PRODUCT_HI3519A)
            /* in case of HDMI_TMDS_MODE_HDMI_2_0,hdmi_dvi_sel should be set 1, */
            /* eess_mode_en shoud be set 1 */
            HDMI_AVMIXER_CONFIG_hdmi_dvi_selSet(HI_TRUE);
            HDMI_AVMIXER_CONFIG_eess_mode_enSet(HI_TRUE);
#endif
            break;
        default:
            HDMI_WARN("un-known tmds mode:%u\n",enTmdsMode);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlNullPacketSet(HI_BOOL bEnable)
{
    HDMI_AVMIXER_CONFIG_null_pkt_enSet(bEnable);

    return HI_SUCCESS;
}

/******************************* public interface  ***********************************/
HI_S32 HAL_HDMI_CtrlInit(HDMI_DEVICE_ID_E enHdmi, HDMI_HAL_INIT_S *pstHalInit)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);

    if (!pstCtrlInfo->bInit) {
        s32Ret = REG_HDMI_CRG_reg_Init();
        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
        s32Ret = HDMI_TX_S_tx_aon_reg_REGS_TYPE_Init(HI_NULL);
        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
        s32Ret = HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_Init(HI_NULL);
        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
        s32Ret = HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init(HI_NULL);
        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
        s32Ret = HDMI_TX_S_video_path_reg_REGS_TYPE_Init(HI_NULL);
        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
        pstCtrlInfo->bInit = HI_TRUE;
        DRV_HDMI_ProdCrgGateSet(HI_TRUE);
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlDeinit(HDMI_DEVICE_ID_E enHdmi)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    HDMI_TX_S_tx_aon_reg_REGS_TYPE_DeInit();
    HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_DeInit();
    HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit();
    HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit();

    HDMI_MEMSET(pstCtrlInfo, 0, sizeof(HDMI_CTRL_INFO_S));

    pstCtrlInfo->bInit = HI_FALSE;

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlAvmuteSet(HDMI_DEVICE_ID_E enHdmi,HDMI_AVMUTE_CFG_S *pstAvmuteCfg)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pstAvmuteCfg);
    CTRL_INIT_CHK(pstCtrlInfo);

    CtrlAvmuteSet(pstAvmuteCfg);

    return HI_SUCCESS;
}



HI_S32 HAL_HDMI_CtrlAvmuteGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pbEnable);

    return CtrlAvmuteGet(pbEnable);
}

HI_S32 HAL_HDMI_CtrlCscSet(HDMI_DEVICE_ID_E enHdmi, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CTRL_NULL_CHK(pstVideoCfg);

    CtrlVideoPathColormetrySet(pstVideoCfg->enConvStd, pstVideoCfg->enConvStd);
    CtrlVideoPathQuantizationSet(pstVideoCfg->eOutCscQuantization, pstVideoCfg->eOutCscQuantization);
    CtrlVideoPathColorSpaceSet(pstVideoCfg->enInColorSpace, pstVideoCfg->enOutColorSpace);

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlVideoPathSet(HDMI_DEVICE_ID_E enHdmi,HDMI_VIDEO_PATH_S *pstVideoPath)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HI_BOOL             bEnableGcp = HI_FALSE;
    HI_BOOL             bEnalbeDither = HI_FALSE;
    HDMI_VIDEO_DITHER_E enDitherMode = HDMI_VIDEO_DITHER_DISALBE;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstVideoPath);
    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    HDMI_MEMCPY(&pstCtrlInfo->stVideoPath,pstVideoPath,sizeof(HDMI_VIDEO_PATH_S));

    /* dither */
    switch(pstVideoPath->enOutDeepColor) {
        case  HDMI_DEEP_COLOR_24BIT:
            bEnableGcp      = HI_FALSE;
            switch(pstVideoPath->enInDeepColor) {
                case  HDMI_DEEP_COLOR_30BIT:
                    bEnalbeDither   = HI_TRUE;
                    enDitherMode    = HDMI_VIDEO_DITHER_10_8;
                    break;
                case  HDMI_DEEP_COLOR_36BIT:
                    bEnalbeDither   = HI_TRUE;
                    enDitherMode    = HDMI_VIDEO_DITHER_12_8;
                    break;
                case  HDMI_DEEP_COLOR_24BIT:
                default:
                    bEnalbeDither = HI_FALSE;
                    enDitherMode    = HDMI_VIDEO_DITHER_DISALBE;
                    break;
            }

            break;

        case  HDMI_DEEP_COLOR_30BIT:

            bEnableGcp      = HI_TRUE;
            if(pstVideoPath->enInDeepColor==HDMI_DEEP_COLOR_36BIT) {
                bEnalbeDither   = HI_TRUE;
                enDitherMode    = HDMI_VIDEO_DITHER_12_10;
            } else {
                bEnalbeDither   = HI_FALSE;
                enDitherMode    = HDMI_VIDEO_DITHER_DISALBE;
            }
            break;

        case  HDMI_DEEP_COLOR_36BIT:
            bEnableGcp      = HI_TRUE;
            bEnalbeDither   = HI_FALSE;
            enDitherMode    = HDMI_VIDEO_DITHER_DISALBE;
            break;

        default:
            pstVideoPath->enOutDeepColor   = HDMI_DEEP_COLOR_24BIT;
            bEnableGcp      = HI_FALSE;
            switch(pstVideoPath->enInDeepColor) {
                case  HDMI_DEEP_COLOR_30BIT:
                    bEnalbeDither   = HI_TRUE;
                    enDitherMode    = HDMI_VIDEO_DITHER_10_8;
                    break;
                case  HDMI_DEEP_COLOR_36BIT:
                    bEnalbeDither   = HI_TRUE;
                    enDitherMode    = HDMI_VIDEO_DITHER_12_8;
                    break;
                case  HDMI_DEEP_COLOR_24BIT:
                default:
                    bEnalbeDither = HI_FALSE;
                    enDitherMode    = HDMI_VIDEO_DITHER_DISALBE;
                    break;
            }
            break;
    }

    if( HDMI_VIDEO_TIMING_1440X480I_60000 == pstVideoPath->enTimming
        || HDMI_VIDEO_TIMING_1440X576I_50000 == pstVideoPath->enTimming
        || HDMI_COLORSPACE_YCbCr422 == pstVideoPath->enOutColorSpace ) {
        bEnalbeDither   = HI_FALSE;
        enDitherMode    = HDMI_VIDEO_DITHER_DISALBE;
    }
#if defined(HDMI_PRODUCT_HI3519A)
    CtrlVideoPathLowPowerSet(HI_TRUE);
#endif
    CtrlVideoPathDitherSet(bEnalbeDither,enDitherMode);
    CtrlVideoPathDeepClrSet(bEnableGcp,pstVideoPath->enOutDeepColor);
    s32Ret = CtrlVideoPathPolaritySet(&pstVideoPath->stOutHvSyncPol);
    CtrlVideoPathColormetrySet(pstVideoPath->enInColormetry,pstVideoPath->enOutColormetry);
    CtrlVideoPathQuantizationSet(pstVideoPath->enInQuantization,pstVideoPath->enOutQuantization);
    CtrlVideoPathColorSpaceSet(pstVideoPath->enInColorSpace,pstVideoPath->enOutColorSpace);

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlTmdsModeSet(HDMI_DEVICE_ID_E enHdmi,HDMI_TMDS_MODE_E enTmdsMode)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    s32Ret = CtrlTmdsModeSet(enTmdsMode);
    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlInfoFrameEnSet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable)
{
    HI_S32      s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    switch(enInfoFrameId) {
        case HDMI_INFOFRAME_TYPE_NULL    :
            s32Ret = CtrlNullPacketSet(bEnable);
            break;
        case HDMI_INFOFRAME_TYPE_VENDOR  :
            s32Ret = CtrlVendorInfoFrameEnSet(bEnable);
            break;
        case HDMI_INFOFRAME_TYPE_AVI     :
            s32Ret = CtrlAviInfoFrameEnSet(bEnable);
            break;
        default:
            HDMI_WARN("un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlInfoFrameDataSet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8IfData)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pu8IfData);
    CTRL_NULL_CHK(pstCtrlInfo);

    switch(enInfoFrameId) {
        case HDMI_INFOFRAME_TYPE_VENDOR  :
            s32Ret = CtrlVendorInfoframeDataSet(pu8IfData);
            break;
        case HDMI_INFOFRAME_TYPE_AVI     :
            s32Ret = CtrlAviInfoframeDataSet(pu8IfData);
            break;
        default:
            HDMI_WARN("un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlDataReset(HDMI_DEVICE_ID_E enHdmi, HI_BOOL bDebugMode, HI_U32 u32DelayMs)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);
    static HI_U32       s_u32Delay = 0;

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqSet(HI_TRUE);
    HDMI_UDELAY(5);
    HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(HI_FALSE);
    HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(HI_FALSE);
    HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(HI_FALSE);
    /* TOP rst: will also TX_PACK_FIFO_CTRL reg_fifo_manu_rst */
    HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqSet(HI_FALSE);
    /* at least udelay(80) 20180116 */
    if(bDebugMode) {
        s_u32Delay = u32DelayMs;
    }

    if(s_u32Delay) {
        HDMI_MSLEEP(s_u32Delay);
        HDMI_INFO("dataRst %u ms\n",s_u32Delay);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_CtrlTmdsStableGet(HDMI_DEVICE_ID_E  enHdmi, HI_BOOL *pbStable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pbStable);
    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    *pbStable = HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableGet() ? HI_TRUE : HI_FALSE;
    return HI_SUCCESS;
}


