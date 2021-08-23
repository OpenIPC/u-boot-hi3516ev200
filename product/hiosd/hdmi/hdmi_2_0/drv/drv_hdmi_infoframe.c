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
#include "hdmi_product_define.h"

#include "drv_hdmi_infoframe.h"
#include "drv_hdmi_common.h"
#include "hdmi_hal.h"

#define CHECK_POINTER(p)\
do{\
    if(HI_NULL == (p)){\
        HDMI_ERR("null pointer\n");\
        return HI_FAILURE;\
    }\
}while(0)

HDMI_VIDEO_TIMING_E DRV_HDMI_VideoTimingLookup(HDMI_VIDEO_CODE_E enVic, HDMI_PICTURE_ASPECT enAspect)
{
    HI_U32 u32Cnt,u32VICSize;
    HDMI_VIDEO_TIMING_E enVideoTiming = HDMI_VIDEO_TIMING_UNKNOWN;

    u32VICSize = sizeof(s_stCEAVideoCodesDes)/sizeof(s_stCEAVideoCodesDes[0]);
    for (u32Cnt = 0; u32Cnt < u32VICSize; u32Cnt++) {
        if (s_stCEAVideoCodesDes[u32Cnt].enVideoCode == enVic &&
            s_stCEAVideoCodesDes[u32Cnt].enAspectRatio == enAspect) {
            enVideoTiming = s_stCEAVideoCodesDes[u32Cnt].enTiming;
            break;
        }
    }

    return enVideoTiming;
}

HDMI_VIDEO_TIMING_E DRV_HDMI_VsifVideoTimingLookup(HDMI_VSIF_VIC_E enVic, HDMI_PICTURE_ASPECT enAspect)
{
    HI_U32 u32Cnt,u32VICSize;
    HDMI_VIDEO_TIMING_E enVideoTiming = 0;

    u32VICSize = sizeof(s_stH14bHdmiVideoCodes_4K)/sizeof(s_stH14bHdmiVideoCodes_4K[0]);
    for (u32Cnt = 0; u32Cnt < u32VICSize; u32Cnt++) {
        if (s_stH14bHdmiVideoCodes_4K[u32Cnt].enHdmiVic == enVic) {
            enVideoTiming = s_stH14bHdmiVideoCodes_4K[u32Cnt].enTiming;
            break;
        }
    }

    return enVideoTiming;
}

static HI_VOID hdmi_infoframe_checksum(HI_VOID *buffer, HI_U32 size)
{
    HI_U8 *ptr = HI_NULL;
    HI_U8 csum = 0;
    HI_U32 i;

    if(HI_NULL == buffer) {
        return;
    }

    ptr = buffer;

    /* compute checksum */
    for (i = 0; i < size; i++) {
        csum += ptr[i];
    }
    if ((0 < csum) && (csum <= 255)) {
        ptr[3] = 256 - csum;
    } else {
        ptr[3] = 0;
    }
}

/**
 * hdmi_avi_infoframe_pack() - write HDMI AVI infoframe to binary buffer
 * @frame: HDMI AVI infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */

static HI_S32 hdmi_avi_infoframe_pack(struct hdmi_avi_infoframe *frame, HI_VOID *buffer,
    HI_U32 size, HDMI_3D_MODE_E HDMI_3D_Mode)
{
    HI_U8 *ptr                      = HI_NULL;
    HI_U32 length                   = 0;
    enum hdmi_video_code video_code = 0;
    HI_BOOL b3DEnable               = HI_FALSE;

    CHECK_POINTER(frame);
    CHECK_POINTER(buffer);

    ptr = buffer;

    length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

    if (size < length)
        return -1;

    b3DEnable = (HDMI_3D_BUTT != HDMI_3D_Mode) ? HI_FALSE : HI_TRUE;

    video_code = DRV_HDMI_VicSearch(frame->video_timing, frame->picture_aspect, b3DEnable);

    HDMI_MEMSET(buffer, 0, length);

    ptr[0] = frame->type;

    if((frame->colorspace & 0x4) || (video_code >= 128)) {
        /* Y2 = 1 or vic >= 128, version shall use 3 */
        frame->version = 3;
    } else if(frame->colorimetry == HDMI_COLORIMETRY__EXTENDED && frame->extended_colorimetry == HDMI_EXTENDED_COLORIMETRY_Additional) {
        /* (C1,C0)=(1,1) and (EC2,EC1,EC0)=(1,1,1), version shall use 4 */
        frame->version = 4;
    }

    ptr[1] = frame->version;
    ptr[2] = frame->length;
    ptr[3] = 0; /* checksum */

    /* start infoframe payload */
    ptr += HDMI_INFOFRAME_HEADER_SIZE;

    ptr[0] = (((HI_U8)frame->colorspace & 0x3) << 5) | ((HI_U8)frame->scan_mode & 0x3);

    if (frame->active_info_valid)
        ptr[0] |= SET_BIT(4);

    if (frame->horizontal_bar_valid)
        ptr[0] |= SET_BIT(3);

    if (frame->vertical_bar_valid)
        ptr[0] |= SET_BIT(2);

    ptr[1] = (((HI_U8)frame->colorimetry & 0x3) << 6)    |
             (((HI_U8)frame->picture_aspect & 0x3) << 4) |
             ((HI_U8)frame->active_aspect & 0xf);

    ptr[2] = (((HI_U8)frame->extended_colorimetry & 0x7) << 4) |
             (((HI_U8)frame->quantization_range & 0x3) << 2)   |
             ((HI_U8)frame->nups & 0x3);

    if (frame->itc)
        ptr[2] |= SET_BIT(7);

    ptr[3] = (HI_U8)video_code & 0xff; /* frame->video_code */

    ptr[4] = (((HI_U8)frame->ycc_quantization_range & 0x3) << 6) |
             (((HI_U8)frame->content_type & 0x3) << 4) |
             ((frame->pixel_repeat - 1) & 0xf);

    ptr[5]  = frame->top_bar & 0xff;
    ptr[6]  = (frame->top_bar >> 8) & 0xff;
    ptr[7]  = frame->bottom_bar & 0xff;
    ptr[8]  = (frame->bottom_bar >> 8) & 0xff;
    ptr[9]  = frame->left_bar & 0xff;
    ptr[10] = (frame->left_bar >> 8) & 0xff;
    ptr[11] = frame->right_bar & 0xff;
    ptr[12] = (frame->right_bar >> 8) & 0xff;
    hdmi_infoframe_checksum(buffer, length);

    return length;
}

/**
 * hdmi_vendor_infoframe_pack() - write a HDMI vendor infoframe to binary
 *                                buffer
 * @frame: HDMI vendor infoframe
 * @buffer: destination buffer
 * @size: size of buffer
 *
 * Packs the information contained in the @frame structure into a binary
 * representation that can be written into the corresponding controller
 * registers. Also computes the checksum as required by section 5.3.5 of
 * the HDMI 1.4 specification.
 *
 * Returns the number of bytes packed into the binary buffer or a negative
 * error code on failure.
 */
static HI_S32 hdmi_vendor_specific_infoframe_pack(struct hdmi_vendor_specific_infoframe *frame,
                   HI_VOID *buffer, HI_U32 size)
{
    HI_U8  *ptr = HI_NULL;
    HI_U32 length;

    CHECK_POINTER(frame);
    CHECK_POINTER(buffer);

    ptr = buffer;

    length = HDMI_INFOFRAME_HEADER_SIZE + frame->length;

    if (size < length)
        return -1;

    HDMI_MEMSET(buffer, 0, length);

    ptr[0] = frame->type;
    ptr[1] = frame->version;
    ptr[2] = frame->length;
    ptr[3] = 0; /* checksum */

    if (frame->vsif_content.h14_vsif_content.ieee == HDMI14_IEEE_OUI) { /* for HDMI 1.4 */
        hdmi_14_vsif_content *vsif_content = &frame->vsif_content.h14_vsif_content;

        /* IEEE OUI */
        ptr[4] = HDMI14_IEEE_OUI & 0xff;
        ptr[5] = (HDMI14_IEEE_OUI >> 8) & 0xff;
        ptr[6] = (HDMI14_IEEE_OUI >> 16) & 0xff;

        /* hdmi video format */
        ptr[7] = ((HI_U8)vsif_content->format & 0x7) << 5;

        if (vsif_content->format == HDMI_VIDEO_FORMAT_4K) {
            ptr[8] = vsif_content->vic;
        } else if (vsif_content->format == HDMI_VIDEO_FORMAT_3D) {
            ptr[8] = ((HI_U8)vsif_content->_3d_structure & 0xf) << 4;
        }
    } else if (frame->vsif_content.hf_vsif_content.ieee == HDMI20_IEEE_OUI) /* for HDMI 2.0,to be define */  {

    }

    hdmi_infoframe_checksum(buffer, length);

    return length;
}

HI_S32 DRV_HDMI_InfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HDMI_INFOFRAME_ID_E enInfoFrameId, HDMI_INFOFRAME_U* uInfoframe)
{
    HI_U8           buffer[32] = {0};
    HDMI_DEVICE_S*  pstHdmiDev = HI_NULL;
    HI_S32          s32Ret     = HI_SUCCESS;

    CHECK_POINTER(pstInfoFrame);
    CHECK_POINTER(uInfoframe);

    pstHdmiDev = HDMI_CONTAINER_OF(pstInfoFrame, __typeof__(*pstHdmiDev), stInfoFrame);
    CHECK_POINTER(pstHdmiDev);

    HDMI_MEMSET(buffer, 0, sizeof(buffer));

    /* pack infoframe */
    switch (enInfoFrameId) {
        case HDMI_INFOFRAME_TYPE_VENDOR: {
            uInfoframe->stVendorInfoFrame.type    = HDMI_INFOFRAME_TYPE_VENDOR;
            uInfoframe->stVendorInfoFrame.version = 1;
            uInfoframe->stVendorInfoFrame.length  = 0x07;
            hdmi_vendor_specific_infoframe_pack(&uInfoframe->stVendorInfoFrame, buffer, 32);
            break;
        }
        case HDMI_INFOFRAME_TYPE_AVI: {
            uInfoframe->stAVIInfoFrame.type      = HDMI_INFOFRAME_TYPE_AVI;
            uInfoframe->stAVIInfoFrame.version   = 2;
            uInfoframe->stAVIInfoFrame.length    = 13;
            hdmi_avi_infoframe_pack(&uInfoframe->stAVIInfoFrame, buffer, 32, pstHdmiDev->stAttr.stVOAttr.enStereoMode);
            break;
        }
        case HDMI_INFOFRAME_TYPE_AUDIO: {
            break;
        }
        default:
            return HI_FAILURE;
    }

    DRV_HDMI_InfoFrameEnableSet(pstInfoFrame, enInfoFrameId, HI_FALSE);
    HDMI_HAL_CALL(HAL_HDMI_InfoframeSet, enInfoFrameId, buffer);
    DRV_HDMI_InfoFrameEnableSet(pstInfoFrame, enInfoFrameId, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_InfoFrameEnableSet(HDMI_INFOFRAME_S* pstInfoFrame, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable)
{
    HI_S32          s32Ret     = HI_NULL;
    HDMI_DEVICE_S*  pstHdmiDev = HI_NULL;

    CHECK_POINTER(pstInfoFrame);

    pstHdmiDev = HDMI_CONTAINER_OF(pstInfoFrame, __typeof__(*pstHdmiDev), stInfoFrame);
    HDMI_HAL_CALL(HAL_HDMI_InfoframeEnableSet, enInfoFrameId, bEnable);

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_AVIInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable)
{
    HDMI_INFOFRAME_U         uInfoframe;
    HDMI_AVI_INFOFRAME_S*    pstAVIInfoframe = HI_NULL;
    HDMI_VO_ATTR_S*          pstVOAttr  = HI_NULL;
    HDMI_APP_ATTR_S*         pstAppAttr = HI_NULL;
    HDMI_DEVICE_S*           pstHdmiDev = HI_NULL;

    CHECK_POINTER(pstInfoFrame);

    if (!bEnable) {
        DRV_HDMI_InfoFrameEnableSet(pstInfoFrame, HDMI_INFOFRAME_TYPE_AVI, HI_FALSE);
        return HI_SUCCESS;
    }

    pstHdmiDev = HDMI_CONTAINER_OF(pstInfoFrame, __typeof__(*pstHdmiDev), stInfoFrame);
    CHECK_POINTER(pstHdmiDev);

    pstVOAttr  = &pstHdmiDev->stAttr.stVOAttr;
    pstAppAttr = &pstHdmiDev->stAttr.stAppAttr;

    pstAVIInfoframe = &pstInfoFrame->stAVIInfoFrame;
    HDMI_MEMSET(pstAVIInfoframe, 0, sizeof(HDMI_AVI_INFOFRAME_S));
    pstAVIInfoframe->disp_fmt               = pstVOAttr->u32DispFmt;
    pstAVIInfoframe->video_timing           = pstVOAttr->enVideoTiming;

    pstAVIInfoframe->picture_aspect         = pstVOAttr->enPictureAspect;
    pstAVIInfoframe->active_aspect          = pstVOAttr->enActiveAspect;
    pstAVIInfoframe->nups                   = pstVOAttr->enPictureScaling;
    pstAVIInfoframe->pixel_repeat           = pstVOAttr->u32PixelRepeat;
    pstAVIInfoframe->active_info_valid      = HI_TRUE;
    pstAVIInfoframe->horizontal_bar_valid   = HI_FALSE;
    pstAVIInfoframe->vertical_bar_valid     = HI_FALSE;
    pstAVIInfoframe->scan_mode              = HDMI_SCAN_MODE_NONE;
    pstAVIInfoframe->itc                    = HI_FALSE;
    pstAVIInfoframe->content_type           = HDMI_CONTENT_TYPE_NONE;
    pstAVIInfoframe->top_bar                = 0;
    pstAVIInfoframe->bottom_bar             = 0;
    pstAVIInfoframe->left_bar               = 0;
    pstAVIInfoframe->right_bar              = 0;

    pstAVIInfoframe->extended_colorimetry   = pstVOAttr->enExtendedColorimetry;
    pstAVIInfoframe->quantization_range     = pstVOAttr->enRGBQuantization;
    pstAVIInfoframe->ycc_quantization_range = pstVOAttr->enYCCQuantization;
    pstAVIInfoframe->colorspace             = pstAppAttr->enOutColorSpace;
    pstAVIInfoframe->colorimetry            = pstAppAttr->bxvYCCMode ? HDMI_COLORIMETRY__EXTENDED : pstVOAttr->enColorimetry;

    HDMI_MEMCPY(&uInfoframe.stAVIInfoFrame, pstAVIInfoframe, sizeof(HDMI_AVI_INFOFRAME_S));
    DRV_HDMI_InfoFrameSend(pstInfoFrame, HDMI_INFOFRAME_TYPE_AVI, &uInfoframe);

    return HI_SUCCESS;

}

HI_S32 DRV_HDMI_VendorInfoFrameSend(HDMI_INFOFRAME_S* pstInfoFrame, HI_BOOL bEnable)
{
    HDMI_INFOFRAME_U         uInfoframe;
    HDMI_VENDOR_INFOFRAME_S* pstVendorInfoframe = HI_NULL;
    hdmi_14_vsif_content*    pstH14VsifContent = HI_NULL;
    HDMI_VO_ATTR_S*          pstVOAttr = HI_NULL;
    HI_BOOL                  bVendorInfoFrameEnable = HI_FALSE;
    HDMI_DEVICE_S*           pstHdmiDev = HI_NULL;
    HI_U32                   enVic = 0;

    CHECK_POINTER(pstInfoFrame);

    if (!bEnable) {
        DRV_HDMI_InfoFrameEnableSet(pstInfoFrame, HDMI_INFOFRAME_TYPE_VENDOR, HI_FALSE);
        return HI_SUCCESS;
    }

    pstHdmiDev = HDMI_CONTAINER_OF(pstInfoFrame, __typeof__(*pstHdmiDev), stInfoFrame);
    CHECK_POINTER(pstHdmiDev);

    pstVOAttr  = &pstHdmiDev->stAttr.stVOAttr;
    pstVendorInfoframe = &pstInfoFrame->stVendorInfoFrame;
    HDMI_MEMSET(pstVendorInfoframe, 0, sizeof(HDMI_VENDOR_INFOFRAME_S));

    pstH14VsifContent = &pstVendorInfoframe->vsif_content.h14_vsif_content;
    enVic = DRV_HDMI_VicSearch(pstVOAttr->enVideoTiming, pstVOAttr->enPictureAspect, HI_FALSE); /* find the real Vic */
    if(((enVic >= HDMI_3840X2160P24_16_9 && enVic <= HDMI_3840X2160P30_16_9) || enVic == HDMI_4096X2160P24_256_135) &&
        pstVOAttr->enStereoMode == HDMI_3D_BUTT) {
        HI_U32  u32Cnt;
        pstH14VsifContent->ieee   = HDMI14_IEEE_OUI;
        pstH14VsifContent->format = HDMI_VIDEO_FORMAT_4K;
        pstH14VsifContent->vic    = 0;

        for (u32Cnt = 0; u32Cnt < 4; u32Cnt++) {
            if (s_stH14bHdmiVideoCodes_4K[u32Cnt].enTiming == pstVOAttr->enVideoTiming) {
                pstH14VsifContent->vic = s_stH14bHdmiVideoCodes_4K[u32Cnt].enHdmiVic;   /* HDMI_VIC */
                break;
            }
        }

        bVendorInfoFrameEnable = HI_TRUE;
    } else if (pstVOAttr->enStereoMode < HDMI_3D_BUTT)   /* common 3D */  {
        pstH14VsifContent->ieee = HDMI14_IEEE_OUI;
        pstH14VsifContent->format = HDMI_VIDEO_FORMAT_3D;
        pstH14VsifContent->_3d_structure = pstVOAttr->enStereoMode;
        bVendorInfoFrameEnable = HI_TRUE;
    } else {
        pstH14VsifContent->ieee = HDMI14_IEEE_OUI;
        pstH14VsifContent->format = HDMI_VIDEO_FORMAT_NONE;
        pstH14VsifContent->_3d_structure = pstVOAttr->enStereoMode;
        bVendorInfoFrameEnable = HI_TRUE;
    }
    if (bVendorInfoFrameEnable == HI_TRUE) {
        HDMI_MEMCPY(&uInfoframe.stVendorInfoFrame, pstVendorInfoframe, sizeof(HDMI_VENDOR_INFOFRAME_S));
        DRV_HDMI_InfoFrameSend(pstInfoFrame, HDMI_INFOFRAME_TYPE_VENDOR, &uInfoframe);
    }

    return HI_SUCCESS;
}

