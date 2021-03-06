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


#ifndef __HDMI_HAL_H__
#define __HDMI_HAL_H__
/**
* @brief KSV data object
*/
#include "drv_hdmi_common.h"

typedef HI_VOID* HDMI_HANDLE;

typedef enum{
    HDMI_DEBUG_CMD_COLOR_BAR,
    HDMI_DEBUG_CMD_SW_RESET,
    HDMI_DEBUG_CMD_RGB2YUV,
    HDMI_DEBUG_CMD_YUV2RGB,
    HDMI_DEBUG_CMD_DITHER,
    HDMI_DEBUG_CMD_BYPASS,
    HDMI_DEBUG_CMD_DDC_FREQ,
    HDMI_DEBUG_CMD_PHY_DEFAULT_GET,
    HDMI_DEBUG_CMD_PHY_PARA_SET,
    HDMI_DEBUG_CMD_DUMP,
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
    HDMI_DEBUG_CMD_PROC_MACH,
    HDMI_DEBUG_CMD_PROC_SCDC,
    HDMI_DEBUG_CMD_PROC_HDCP14,
    HDMI_DEBUG_CMD_PROC_HDCP22,
    HDMI_DEBUG_CMD_PROC_DDC,
    HDMI_DEBUG_CMD_PROC_CECTX,
    HDMI_DEBUG_CMD_PROC_CECRX,

    HDMI_DEBUG_CMD_DBG_VIDEO_GET,
    HDMI_DEBUG_CMD_DBG_VIDEO_SET,
    HDMI_DEBUG_CMD_SSC,
    HDMI_DEBUG_CMD_FRL,

#endif
}HDMI_HAL_DEBUG_CMD_E;

typedef struct {
    HI_U32  u32RegAddr;
    HI_U32  u32RegVaule;
}HDMI_HAL_REG_S;

typedef struct{

    HI_U8   u8DataSwing;        /* data-swing */
    HI_U8   u8ClkSwing;         /* CLK-swing */
    HI_U8   u8SrcTermination;   /* source termination */
    HI_U8   u8DrvVnbValue;      /* Tx driver vnb value setting */
    HI_U8   u8ClkSrcFineAdjust; /* Fine adjust clock source termination */
    HI_U8   u8TxRegZone;        /* tx_reg_zone */
    HI_U8   u8RiseTime;         /* rise time */
    HI_U8   u8FallTime;         /* fall time */

}HDMI_HAL_PHY_PARA_S;

typedef struct {
    HDMI_HANDLE          hHdmiHw;
    HDMI_HANDLE          hHdmiDev;
    HI_U32               u32HdmiID;
    HDMI_TX_CAPABILITY_S stTxCapability;
    HMDI_CALLBACK        pCallback;
    HDMI_VIDEO_CONFIG_S  stVideoCfg;
    HDMI_AUDIO_CONFIG_S  stAudioCfg;
    HI_CHAR              *pcBaseAddr;
    HI_CHAR              *pcPhyAddr;
}HDMI_HAL_CONTEXT_S;

typedef struct {
    HI_U32              u32DispFmt;
    HI_U32              u32PixClk;
    HDMI_COLORSPACE_E   enColorSpace;
    HDMI_DEEP_COLOR_E   enDeepColor;

}HDMI_HAL_BASE_PARAM_S;

typedef struct hiHDMI_HAL_S {
    HDMI_HAL_CONTEXT_S stHalCtx;
    HI_VOID (*HAL_HDMI_HardwareInit)(struct hiHDMI_HAL_S *pstHdmiHal);
    HI_VOID (*HAL_HDMI_TmdsModeSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TMDS_MODE_E enTmdsMode);
    HI_VOID (*HAL_HDMI_AvMuteSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAvMute);
    HI_VOID (*HAL_HDMI_InfoframeSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 u8InBuffer[]);
    HI_VOID (*HAL_HDMI_InfoframeEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable);
    HI_S32  (*HAL_HDMI_VideoPathSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg);
    HI_VOID (*HAL_HDMI_PhyOutputEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_VOID (*HAL_HDMI_PhyPowerEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_VOID (*HAL_HDMI_TxCapabilityGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TX_CAPABILITY_S *pstTxCapability);
    HI_VOID (*HAL_HDMI_CscParamSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg);
    HI_VOID (*HAL_HDMI_PhySet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_PHY_CFG_S *pstPhyCfg);
    HI_VOID (*HAL_HDMI_CtrlReset)(struct hiHDMI_HAL_S *pstHdmiHal);

#ifdef HDMI_SCDC_SUPPORT
    HI_VOID (*HAL_HDMI_ScdcConfig)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_CONFIG_S* pstScdcConfig);
    HI_VOID (*HAL_HDMI_ScdcStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus);
    HI_VOID (*HAL_HDMI_ScdcStatusSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus);
    HI_S32 (*HAL_HDMI_ScdcProcess)(struct hiHDMI_HAL_S *pstHdmiHal, SCDC_CMD_E enScdcCmd, HI_VOID* pData);
#endif
}HDMI_HAL_S;


/*******************************define_start**********************************************************
Function:
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle)
* Description:  Open HDMI HAL
* Input:        pstHalInit: Init
* Output:       **pstHalHandle: hal global struct pointer
* Return:       HI_SUCCESS / ERROR CODE
* Others:       NONE
************************************end****************************************************************/
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle);

/*******************************define_start**********************************************************
Function:
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal)
* Description:  release HAL
* Input:        *pstHdmiHal:hal global struct pointer
* Output:       NONE
* Return:       NONE
* Others:       NONE
************************************end****************************************************************/
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal);
#endif
