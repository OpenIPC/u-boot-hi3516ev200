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
#ifndef __HDMI_HAL_SCDC_H__
#define __HDMI_HAL_SCDC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_type.h"
#include "hdmi_hal_machine.h"
#include "hdmi_hal_ddc.h"
#include "drv_hdmi_common.h"

#define SCDC_DEFAULT_SCRAMBLE_TIMEOUT       200
#define SCDC_MAX_SCRAMBLE_TIMEOUT           1000
#define SCDC_DEFAULT_SCRAMBLE_INTERVAL      20

#define SCDC_DEFAULT_POLL_INTERVAL          150

#define SCDC_MAX_UPDATE_RECORD_NUM          3
#define SCDC_WHILE_SCRAMBLE_TIMEOUT         3500

#define SCDC_ERROR_MAX_NUM_SCRAMBLE         3
#define SCDC_ERROR_MAX_NUM_OE_DISABLE       3
#define SCDC_ERROR_MAX_NUM_OE_ENABLE        3
#define SCDC_ERROR_MAX_NUM_RR_TEST          3
#define SCDC_ERROR_MAX_NUM_SINK_UNREAD      1
#define SCDC_MAX_ERROR_RECORD_NUM           ((SCDC_ERROR_MAX_NUM_SCRAMBLE)+\
                                                (SCDC_ERROR_MAX_NUM_OE_DISABLE)+\
                                                (SCDC_ERROR_MAX_NUM_OE_ENABLE))


/* HDMI2.1 section 10.4.1.3,OFFSET 0x10 */
typedef union {
    struct {
        HI_U8   bzStatusUpdate              : 1 ;
        HI_U8   bzCedUpdate                 : 1 ;
        HI_U8   bzRrTest                    : 1 ;
        HI_U8   bzSourceTestUpate           : 1 ;
        HI_U8   bzFrlStart                  : 1 ;
        HI_U8   bzFltUpdate                 : 1 ;
        HI_U8   bzRsedUpdate                : 1 ;
        HI_U8   bzRsvd                      : 1 ;
    }u8;

    HI_U8   u8Byte;
}SCDC_UPDATE0_FLAG_U;

/* HDMI2.1 section 10.4.1.3,OFFSET 0x11 */
typedef union {
    struct {
        HI_U8    bzRsvd                     : 1 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_UPDATE1_FLAG_U;

/* HDMI2.1 section 10.4.1.4,OFFSET 0x20 */
typedef union {
    struct {
        HI_U8    bzScreamblingEnable        : 1 ;
        HI_U8    bzTmdsBitClockRatio        : 1 ;
        HI_U8    bzRsvd                     : 6 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_TMDS_CONFIG_U;

/* HDMI2.1 section 10.4.1.5,OFFSET 0x21 */
typedef union {
    struct {
        HI_U8    bzScreamblingStatus        : 1 ;
        HI_U8    bzRsvd                     : 7 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_SCRAMBLER_STATUS_U;


/* HDMI2.1 section 10.4.1.6,OFFSET 0x30 */
typedef union {
    struct {
        HI_U8    bzRrEnable                 : 1 ;
        HI_U8   bzFltNoRetrain              : 1 ;
        HI_U8    bzRsvd                     : 6 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_CONFIG0_U;

/* HDMI2.1 section 10.4.1.6,OFFSET 0x31 */
typedef union {
    struct {
        HI_U8    bzFrlRate                  : 4 ;
        HI_U8   bzFfeLevels                 : 4 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_CONFIG1_U;

/* HDMI2.1 section 10.4.1.6,OFFSET 0x35 */
typedef union {
    struct {
        HI_U8    bzRsvd0                    : 1 ;
        HI_U8   bzPreShootOnly              : 1 ;
        HI_U8   bzDeEmphasisOnly            : 1 ;
        HI_U8   bzNoFfe                     : 1 ;
        HI_U8    bzRsvd1                    : 1 ;
        HI_U8   bzFltNoTimeout              : 1 ;
        HI_U8   bzDscFrlMax                 : 1 ;
        HI_U8   bzFrlMax                    : 1 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_TEST_CONFIG_U;


/* HDMI2.1 section 10.4.1.7,OFFSET 0x40 */
typedef union {
    struct {
        HI_U8    bzColckDetected             : 1 ;
        HI_U8    bzCh0Locked                 : 1 ;
        HI_U8    bzCh1Locked                 : 1 ;
        HI_U8    bzCh2Locked                 : 1 ;
        HI_U8    bzCh3Locked                 : 1 ;
        HI_U8    bzRsvd                      : 1 ;
        HI_U8    bzFltReady                  : 1 ;
        HI_U8    bzDscDecodeFail             : 1 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_STATUS_FLAG0_U;

/* HDMI2.1 section 10.4.1.7,OFFSET 0x41 */
typedef union {
    struct {
        HI_U8    bzLn0LtpReq                 : 4 ;
        HI_U8    bzLn1LtpReq                 : 4 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_STATUS_FLAG1_U;

/* HDMI2.1 section 10.4.1.7,OFFSET 0x42 */
typedef union {
    struct {
        HI_U8    bzLn2LtpReq                 : 4 ;
        HI_U8    bzLn3LtpReq                 : 4 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_STATUS_FLAG2_U;


/* HDMI2.1 section 10.4.1.8,OFFSET 0x50~0x55  0x57~0x5A */
typedef union {
    struct {
        HI_U8    bzChErrorCnt_L              : 8 ;
        HI_U8    bzChErrorCnt_H              : 7 ;
        HI_U8    bzChValid                   : 1 ;
    }u16;

    HI_U16    u16Word;
}SCDC_CHAR_ERR_DETECT_U;


/* HDMI2.1 section 10.4.1.9,OFFSET 0xC0 */
typedef union {
    struct {
        HI_U8    bzTestReadRequestDelay      : 7 ;
        HI_U8    bzTestReadRequest           : 1 ;
    }u8;

    HI_U8    u8Byte;
}SCDC_RR_TEST_CFG_U;

typedef enum {
    SCDC_OFFSET_SINK_VERSION        = 0x01,
    SCDC_OFFSET_SRC_VERSION         = 0x02,

    SCDC_OFFSET_UPDATE_0            = 0x10,
    SCDC_OFFSET_UPDATE_1            = 0x11,

    SCDC_OFFSET_TMDS_CONFIG         = 0x20,
    SCDC_OFFSET_SCRAMBLER_STATUS    = 0x21,

    SCDC_OFFSET_CONFIG_0            = 0x30,
    SCDC_OFFSET_CONFIG_1            = 0x31,

    SCDC_OFFSET_TEST_CONFIG         = 0x35,


    SCDC_OFFSET_STATUS_FLAG_0       = 0x40,
    SCDC_OFFSET_STATUS_FLAG_1       = 0x41,
    SCDC_OFFSET_STATUS_FLAG_2       = 0x42,


    SCDC_OFFSET_DRR_DET_0_L         = 0x50,
    SCDC_OFFSET_DRR_DET_0_H         = 0x51,
    SCDC_OFFSET_DRR_DET_1_L         = 0x52,
    SCDC_OFFSET_DRR_DET_1_H         = 0x53,
    SCDC_OFFSET_DRR_DET_2_L         = 0x54,
    SCDC_OFFSET_DRR_DET_2_H         = 0x55,
    SCDC_OFFSET_DRR_DET_CHKSUM      = 0x56,

    SCDC_OFFSET_TEST_CONFIG_0       = 0xC0,

    SCDC_OFFSET_IEEE_OUI_3TH        = 0xD0,
    SCDC_OFFSET_IEEE_OUI_2ND        = 0xD1,
    SCDC_OFFSET_IEEE_OUI_1ST        = 0xD2,
    SCDC_OFFSET_DEVICE_ID_HEAD      = 0xD3,
    SCDC_OFFSET_DEVICE_ID_TAIL      = 0xDD,

}SCDC_OFFSET_E;


typedef struct {

    HI_U8                        u8SinkVersion;          /* SCDC_OFFSET_SINK_VERSION */
    HI_U8                        u8SrcVersion;           /* SCDC_OFFSET_SRC_VERSION */
    SCDC_TMDS_CONFIG_U           u8TmdsConfig0;          /* SCDC_OFFSET_TMDS_CONFIG */
    SCDC_CONFIG0_U               u8Config0;              /* SCDC_OFFSET_CONFIG_0 */
    SCDC_RR_TEST_CFG_U           u8RrTestCfg;            /* SCDC_OFFSET_TEST_CONFIG_0 */
    HI_U8                        au8IeeeOut[SCDC_OFFSET_IEEE_OUI_1ST-SCDC_OFFSET_IEEE_OUI_3TH+1];
    HI_U8                        au8DeviceId[SCDC_OFFSET_DEVICE_ID_TAIL-SCDC_OFFSET_DEVICE_ID_HEAD+1];

}SCDC_REG_STATIC_STATUS_S;


typedef struct {
    SCDC_UPDATE0_FLAG_U          u8Update0;          /* SCDC_OFFSET_UPDATE_0 */
    SCDC_SCRAMBLER_STATUS_U      u8ScrambleStatus;   /* SCDC_OFFSET_SCRAMBLER_STATUS */
    SCDC_STATUS_FLAG0_U          u8StatusFlag0;      /* SCDC_OFFSET_STATUS_FLAG_0 */
    SCDC_CHAR_ERR_DETECT_U       u16Ch0Err;          /* SCDC_OFFSET_DRR_DET_0_L,SCDC_OFFSET_DRR_DET_0_H */
    SCDC_CHAR_ERR_DETECT_U       u16Ch1Err;          /* SCDC_OFFSET_DRR_DET_1_L,SCDC_OFFSET_DRR_DET_1_H */
    SCDC_CHAR_ERR_DETECT_U       u16Ch2Err;          /* SCDC_OFFSET_DRR_DET_2_L,SCDC_OFFSET_DRR_DET_2_H */
    HI_U8                        u8ErrChkSum;        /* SCDC_OFFSET_DRR_DET_CHKSUM */
}SCDC_REG_UPDATE_STATUS_S;


typedef struct {
    HI_BOOL             bSinkReadQuest;

    HI_BOOL             bSrcScramble;               /* HI_TRUE indicate enable;HI_FALSE indicate disable. */
    HI_BOOL             bSinkScramble;              /* HI_TRUE indicate enable;HI_FALSE indicate disable. */
    HI_BOOL             bTmdsClkRatio40x;           /* HI_TRUE indicate 40x;HI_FALSE indicate 10x. */

    HI_U32              u32ScrambleTimeout;         /* in unit of ms.for [0,200],force to default 200;or,set the value cfg(>200). */
    HI_U32              u32ScrambleInterval;        /* in unit of ms,range[20,200).for [0,20] or >=200,force to default 20;or,set the value cfg[20,200). */

}SCDC_ATTR_S;


typedef enum{
    SCDC_ERROR_SINK_UNREAD,
    SCDC_ERROR_SCRAMBLE,
    SCDC_ERROR_OE_DISABLE,
    SCDC_ERROR_OE_ENABLE,
    SCDC_ERROR_BUTT,
}SCDC_ERROR_TYPE_E;


typedef struct {

    HI_U32            u32SrambleFailCnt;
    HI_U32            u32OeDisableFailCnt;
    HI_U32            u32OeEnableFailCnt;
    HI_U32            u32SinkUnReadCnt;

}SCDC_ERROR_CNT_S;



typedef struct {
    SCDC_ERROR_TYPE_E        enErrType;
    HI_U64                   u64TimeStamp;
    SCDC_ATTR_S              stErrAttr;

}SCDC_ERROR_RECORD_S;


typedef struct {
    HI_BOOL                     bInit;
    HDMI_DEVICE_ID_E            enHdmiId;
}SCDC_STATUS_S;


typedef struct {
    SCDC_STATUS_S               stStatus;

    SCDC_ATTR_S                 stAttr;
}SCDC_INFO_S;



HI_S32 HAL_HDMI_ScdcInit(HDMI_DEVICE_ID_E enHdmiId);

HI_S32 HAL_HDMI_ScdcDeinit(HDMI_DEVICE_ID_E enHdmiId);

HI_S32 HAL_HDMI_ScdcReset(HDMI_DEVICE_ID_E enHdmiId);

HI_S32 HAL_HDMI_ScdcAttrSet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr);

HI_S32 HAL_HDMI_ScdcAttrGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr);

HI_S32 HAL_HDMI_ScdcAllStatusGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_STATUS_S *pstStatus);

HI_S32 HAL_HDMI_ScdcSupportGet(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL *pbSupport);

HI_S32 HAL_HDMI_ScdcScrambleForce(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable);

HI_S32 HAL_HDMI_ScdcMachEnable(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable);

HI_S32 HAL_HDMI_ScdcCtrl(HDMI_DEVICE_ID_E enHdmiId, SCDC_CMD_E enScdcCmd, HI_VOID *pData);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_SCDC_H__ */




