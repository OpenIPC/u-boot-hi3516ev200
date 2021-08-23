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
#include "boot_hdmi_intf.h"

#include "hdmi_osal.h"
#include "hdmi_product_define.h"
#include "hdmi_hal_phy.h"
#include "hdmi_hal_scdc.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_machine.h"
#include "hdmi_hal_intf.h"

#include "hdmi_reg_tx.h"

#define SCDC_HDMI20_VERSION         1
#define SCDC_UPDATE_TIME_INTERVAL   150

typedef struct {
    HI_BOOL     bSrcScramble;
    HI_BOOL     bSinkScramble;
    HI_BOOL     bTmdsClkRatio40x;
}SCDC_SCRAMBLE_S;


#define SCDC_NULL_CHK(p) do{\
    if(HI_NULL == p) {\
        HDMI_WARN("%s is null pointer!return fail.\n",#p);\
        return HI_FAILURE;\
    }\
}while(0)

#define SCDC_INIT_CHK(pstScdcInfo) do{\
    SCDC_NULL_CHK(pstScdcInfo);\
    if(pstScdcInfo->stStatus.bInit != HI_TRUE) {\
        HDMI_WARN("hdmi scdc module didn't init!\n");\
        return HI_FAILURE;\
    }\
}while(0)

/****************************  private data *****************************/
static SCDC_INFO_S s_stScdcInfo[HDMI_DEVICE_ID_BUTT] = { { {0}, {0} } };


/****************************  private interface *****************************/

static SCDC_INFO_S *ScdcInfoPtrGet(HDMI_DEVICE_ID_E enHdmiId)
{
    if (enHdmiId < HDMI_DEVICE_ID_BUTT) {
        return &s_stScdcInfo[enHdmiId];
    }
    return HI_NULL;

}

static HI_S32 ScdcScrambleSet(HDMI_DEVICE_ID_E enHdmiId,SCDC_SCRAMBLE_S *pstScramble)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    SCDC_TMDS_CONFIG_U      u8DdcConfigByte;
    DDC_CFG_S               stDdcSrcCfg ;

    SCDC_NULL_CHK(pstScramble);

    /* source scramble */
    HDMI_HDMI_ENC_CTRL_enc_scr_onSet(pstScramble->bSrcScramble);

    /* sink scramble */
    u8DdcConfigByte.u8.bzScreamblingEnable     = pstScramble->bSinkScramble ? HI_TRUE : HI_FALSE;
    u8DdcConfigByte.u8.bzTmdsBitClockRatio     = pstScramble->bTmdsClkRatio40x ? HI_TRUE : HI_FALSE;

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcSrcCfg);
    stDdcSrcCfg.enFuncType                  = DDC_FUNC_TYPE_SCDC;
    stDdcSrcCfg.enIssueMode                 = DDC_MODE_WRITE_MUTIL_ACK;
    stDdcSrcCfg.u8Offset                    = SCDC_OFFSET_TMDS_CONFIG;
    stDdcSrcCfg.pu8Data                     = &u8DdcConfigByte.u8Byte;
    stDdcSrcCfg.s32DataSize                 = 1;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSrcCfg);

    return (s32Ret==stDdcSrcCfg.s32DataSize) ? HI_SUCCESS : HI_FAILURE;
}

static HI_S32 ScdcScrambleGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_SCRAMBLE_S *pstScramble)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    SCDC_SCRAMBLER_STATUS_U u8DdcSrambleByte;
    SCDC_TMDS_CONFIG_U      u8DdcConfigByte;
    DDC_CFG_S               stDdcSinkCfg;

    SCDC_NULL_CHK(pstScramble);
    /* source scrambled */
    pstScramble->bSrcScramble = HDMI_HDMI_ENC_CTRL_enc_scr_onGet() ? HI_TRUE : HI_FALSE;

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcSinkCfg);
    /* sink scrambled status */
    stDdcSinkCfg.enFuncType                  = DDC_FUNC_TYPE_SCDC;
    stDdcSinkCfg.enIssueMode                 = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcSinkCfg.s32DataSize                 = 1;

    stDdcSinkCfg.u8Offset                    = SCDC_OFFSET_SCRAMBLER_STATUS;
    u8DdcSrambleByte.u8Byte                  = 0;
    stDdcSinkCfg.pu8Data                     = &u8DdcSrambleByte.u8Byte;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSinkCfg);
    pstScramble->bSinkScramble  = u8DdcSrambleByte.u8.bzScreamblingStatus ? HI_TRUE : HI_FALSE;

    /* sink tmds bit clock ratio & scramble cfg */
    u8DdcSrambleByte.u8Byte                 = 0;
    stDdcSinkCfg.pu8Data                    = &u8DdcConfigByte.u8Byte;
    stDdcSinkCfg.u8Offset                   = SCDC_OFFSET_TMDS_CONFIG;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSinkCfg);
    pstScramble->bTmdsClkRatio40x    = u8DdcConfigByte.u8.bzTmdsBitClockRatio ? HI_TRUE : HI_FALSE;

    return s32Ret==stDdcSinkCfg.s32DataSize ? HI_SUCCESS : HI_FAILURE;
}

/****************************  public interface *****************************/

HI_S32 HAL_HDMI_ScdcInit(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScdcInfo);

    if (!pstScdcInfo->stStatus.bInit) {
        HDMI_MEMSET(pstScdcInfo, 0 , sizeof(SCDC_INFO_S));
        pstScdcInfo->stStatus.bInit             = HI_TRUE;
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_ScdcDeinit(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    HDMI_MEMSET(pstScdcInfo, 0 , sizeof(SCDC_INFO_S));
    pstScdcInfo->stStatus.bInit = HI_FALSE;
    return s32Ret;
}

HI_S32 HAL_HDMI_ScdcAttrSet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr)
{
    HI_U32                  u32ScdcTimestart = 0;
    HI_U32                  u32ScdcTimeCur   = 0;

    HI_S32                  s32Ret     = HI_SUCCESS;
    HI_BOOL                 bOeEabnle  = HI_FALSE;
    HI_U32                  u32TimeOut = 0 ;
    SCDC_SCRAMBLE_S         stScramble = {0};
    SCDC_INFO_S             *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScdcAttr);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    if ((pstScdcAttr->u32ScrambleInterval < SCDC_DEFAULT_SCRAMBLE_INTERVAL)
        || (pstScdcAttr->u32ScrambleInterval > SCDC_DEFAULT_SCRAMBLE_TIMEOUT)) {
        pstScdcAttr->u32ScrambleInterval = SCDC_DEFAULT_SCRAMBLE_INTERVAL;
    }

    if (pstScdcAttr->u32ScrambleTimeout < SCDC_DEFAULT_SCRAMBLE_TIMEOUT ) {
        pstScdcAttr->u32ScrambleTimeout = SCDC_DEFAULT_SCRAMBLE_TIMEOUT;
    } else if (pstScdcAttr->u32ScrambleTimeout > SCDC_MAX_SCRAMBLE_TIMEOUT ) {
        pstScdcAttr->u32ScrambleTimeout = SCDC_MAX_SCRAMBLE_TIMEOUT;
    }

    HDMI_MEMCPY(&pstScdcInfo->stAttr,pstScdcAttr,sizeof(SCDC_ATTR_S));

    HDMI_MSLEEP(20);
    /* scrameble cfg */
    u32TimeOut = pstScdcInfo->stAttr.u32ScrambleInterval;
    do{
        /* oe disable */
#ifndef HDMI_FPGA_SUPPORT
        HAL_HDMI_PhyOeSet(HI_FALSE);
        HDMI_MSLEEP(5);
        HAL_HDMI_PhyOeGet(&bOeEabnle);
#endif
        /* sink & source scramble config */
        stScramble.bSrcScramble     = pstScdcInfo->stAttr.bSrcScramble;
        stScramble.bSinkScramble    = pstScdcInfo->stAttr.bSinkScramble;
        stScramble.bTmdsClkRatio40x = pstScdcInfo->stAttr.bTmdsClkRatio40x;
        ScdcScrambleSet(enHdmiId,&stScramble);

        /* oe enable */
#ifndef HDMI_FPGA_SUPPORT
        HDMI_MSLEEP(u32TimeOut);
        HAL_HDMI_PhyOeSet(HI_TRUE);
        HDMI_MSLEEP(u32TimeOut);
        HAL_HDMI_PhyOeGet(&bOeEabnle);
#endif
        /* poll status */
        ScdcScrambleGet(enHdmiId,&stScramble);
        HDMI_MSLEEP(1);
        /*  Compatibility try again */
        if ((stScramble.bTmdsClkRatio40x == pstScdcInfo->stAttr.bTmdsClkRatio40x)) {
            s32Ret = HI_SUCCESS;
        } else {
            s32Ret = HI_FAILURE;
        }

        u32TimeOut  += pstScdcInfo->stAttr.u32ScrambleInterval;
    }while((s32Ret != HI_SUCCESS) && (u32TimeOut <= pstScdcInfo->stAttr.u32ScrambleTimeout)
       && ((u32ScdcTimeCur - u32ScdcTimestart) < SCDC_WHILE_SCRAMBLE_TIMEOUT));


    HDMI_INFO("\noe=%u, DelayInterval=%u, TimeOut=%d, bSrcScarmble=%d, bSinkScramble=%u, bTmdsClkRatio40x=%u\n",
                                bOeEabnle,
                                u32TimeOut,
                                (u32ScdcTimeCur - u32ScdcTimestart),
                                stScramble.bSrcScramble,
                                stScramble.bSinkScramble,
                                stScramble.bTmdsClkRatio40x);


    if (s32Ret != HI_SUCCESS) {
        HDMI_ERR("scdc scramble %s fail!\n",pstScdcInfo->stAttr.bSinkScramble ? "ON" : "OFF");
    } else {
        HDMI_INFO("scdc scramble %s success!\n",pstScdcInfo->stAttr.bSinkScramble ? "ON" : "OFF");
    }

    return s32Ret;
}


HI_S32 HAL_HDMI_ScdcAttrGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr)
{
    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_SCRAMBLE_S     stScramble = {0};
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScdcAttr);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    ScdcScrambleGet(enHdmiId, &stScramble);
    pstScdcInfo->stAttr.bSinkScramble      = stScramble.bSinkScramble;
    pstScdcInfo->stAttr.bSrcScramble       = stScramble.bSrcScramble;
    pstScdcInfo->stAttr.bTmdsClkRatio40x   = stScramble.bTmdsClkRatio40x;

    HDMI_MEMCPY(pstScdcAttr,&pstScdcInfo->stAttr,sizeof(SCDC_ATTR_S));

    return s32Ret;
}

