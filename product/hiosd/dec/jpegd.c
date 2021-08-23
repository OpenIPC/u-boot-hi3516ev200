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
#include "jpegd_image.h"
#include "jpegd_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

extern unsigned long g_hilogo;
extern unsigned long g_jpeg_size;
extern unsigned long g_video_data_base;
extern unsigned long g_jpegd_emar_buf;
extern unsigned int g_output_format;
static const unsigned char zigzag_for_qtable[64] = {
    0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};

motion_jpeg_obj g_jpegd_ctx;

/* decode jpeg picture */
int jpegd_dec_frame(jpegd_handle handle, unsigned int flags)
{
    motion_jpeg_obj *jpegd_hdl_ctx = (motion_jpeg_obj *)handle;
    unsigned int i;
    unsigned int jpeg_flag = 0;
    int result = 0;
    unsigned int type;
    unsigned char *stream;
    unsigned int len;
    hi_u64 pts;
    mjpeg_dec_frame *tmp_dec_frame, dec_frame;
    hi_bool quant_table_valid = HI_FALSE;
    hi_bool huffman_table_valid = HI_FALSE;

    if (jpegd_hdl_ctx == NULL) {
        HI_TRACE("jpegd handle is null!\n");
        return HI_MJPEG_ERR_HANDLE;
    }

    stream = jpegd_hdl_ctx->stream.vir_addr;
    len = jpegd_hdl_ctx->stream.len;
    pts = jpegd_hdl_ctx->stream.pts;
    tmp_dec_frame = &dec_frame;

    if (stream == NULL) {
        HI_TRACE("jpegd stream vir_addr is null!\n");
        return HI_MJPEG_ERR_HANDLE;
    }

    tmp_dec_frame->error_code = 0;
    tmp_dec_frame->error = 0;
    jpegd_hdl_ctx->frame.restart_interval = 0;

    for (i = 0; i < len;) {
        type = 0xFF;

        while ((i < len) && (*(stream++) != 0xff)) {
            i++;
        }
        while ((i < len) && ((type = *(stream++)) == 0xff)) {
            i++;
        }

        switch (type) {
            case BASELINE:  // sequential DCT
            {
                if (jpeg_flag == 1) {
                    int tmp_len = (stream[0] << 8) + stream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    jpegd_hdl_ctx->stream_buffer = stream;
                    i += (tmp_len + 2);
                    stream += (tmp_len);
                    result = decode_sof0(jpegd_hdl_ctx, jpegd_hdl_ctx->stream_buffer, tmp_len);
                    if (result == HI_JPEG_DEC_OK) {
                        jpeg_flag = 2;
                    } else {
                        tmp_dec_frame->error_code = result;
                        HI_TRACE("jpeg decode DCT error!\n");
                        goto END;
                    }
                }
            }
            break;

            case DHT:  // DHT
            {
                unsigned int tmp_len = (stream[0] << 8) + stream[1];
                tmp_len = MIN(tmp_len, len - i);
                jpegd_hdl_ctx->stream_buffer = stream;
                i += (tmp_len + 2);
                stream += (tmp_len);

                if (decode_dht(jpegd_hdl_ctx, jpegd_hdl_ctx->stream_buffer, tmp_len)) {
                    tmp_dec_frame->error_code = HI_ERR_HUFFMAN_TABLE;

                    HI_TRACE("jpeg decode DHT error!\n");
                    goto END;
                }
                huffman_table_valid = HI_TRUE;
            }
            break;

            case SOS:  // scan header
            {
                if (jpeg_flag == 2) {
                    int tmp_len = (stream[0] << 8) + stream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    jpegd_hdl_ctx->stream_buffer = stream;
                    i += (tmp_len);
                    stream += (tmp_len);
                    jpegd_hdl_ctx->first_mcu = 0;
                    result = decode_sos(jpegd_hdl_ctx, jpegd_hdl_ctx->stream_buffer, tmp_len);

                    if (result == HI_JPEG_DEC_OK) {
                        jpeg_flag = 3;  // scan header decoding success
                        goto END;
                    } else {
                        HI_TRACE("jpeg decode SOS error!\n");
                        goto END;  // scan header decoding error
                    }
                }
            }
            break;

            case SOI:  // a new jpeg picture
            {
                i += 2;
                jpeg_flag = 1;
            }
            break;

            case EOI:  // end of jpeg picture
            {
                i += 2;
                goto END;
            }

            case DQT: {
                {
                    int tmp_len = (stream[0] << 8) + stream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    jpegd_hdl_ctx->stream_buffer = stream;
                    i += (tmp_len + 2);
                    stream += (tmp_len);

                    if (decode_dqt(jpegd_hdl_ctx, jpegd_hdl_ctx->stream_buffer, tmp_len)) {
                        tmp_dec_frame->error_code = HI_ERR_QUANT_TABLE;

                        HI_TRACE("jpeg decode DQT error!\n");
                        goto END;
                    }

                    quant_table_valid = HI_TRUE;
                }
            }
            break;

            case DNL: {
                i += 6;
                stream += 4;
                jpeg_flag = 0;
            }
            break;

            case DRI: {
                jpegd_hdl_ctx->stream_buffer[0] = stream[0];
                jpegd_hdl_ctx->stream_buffer[1] = stream[1];
                jpegd_hdl_ctx->stream_buffer[2] = stream[2];
                jpegd_hdl_ctx->stream_buffer[3] = stream[3];
                if (decode_dri(jpegd_hdl_ctx, jpegd_hdl_ctx->stream_buffer, 4)) {
                    jpeg_flag = 0;
                    tmp_dec_frame->error_code = HI_ERR_RESTART_ERROR;
                    HI_TRACE("jpeg decode DRI error!\n");
                    goto END;
                }
                i += 6;
                stream += 4;
            }
            break;

            default:  // do not support
            {
                if ((type & 0xE0) == 0xE0) {
                    int tmp_len = (stream[0] << 8) + stream[1];
                    i += (tmp_len + 2);
                    stream += (tmp_len);

                    if ((type >= 0xE0) && (type <= 0xEF)) {
                    } else if ((type >= 0xE0) && (type <= 0xEF)) {
                    } else {
                    }
                } else if ((type & 0xF0) == 0xC0) {
                    jpeg_flag = 0;
                    tmp_dec_frame->error_code = HI_ERR_NOT_BASELINE;
                    HI_TRACE("jpeg decode error, unsupport type!\n");
                    goto END;
                } else {
                    i += 2;
                }
            }
            break;
        }
    }

END:
    if (jpeg_flag == 3) {
        jpegd_hdl_ctx->stream_offest = i + 2;
        tmp_dec_frame->width = jpegd_hdl_ctx->frame.y_width;
        tmp_dec_frame->height = jpegd_hdl_ctx->frame.y_height;
        tmp_dec_frame->y_stride = jpegd_hdl_ctx->y_stride;
        tmp_dec_frame->c_stride = jpegd_hdl_ctx->c_stride;
        tmp_dec_frame->pic_format = jpegd_hdl_ctx->pic_format;
        tmp_dec_frame->pts = pts;
        tmp_dec_frame->reserved = 0;
        tmp_dec_frame->user_data = NULL;
        jpegd_hdl_ctx->valid_frame ^= 0x1;  //  change to another buffer

        if (quant_table_valid == HI_FALSE) {
            init_default_quant_table(jpegd_hdl_ctx);
        }
        if (huffman_table_valid == HI_FALSE) {
            init_default_huffman_table(jpegd_hdl_ctx);
        }
        return HI_MJPEG_DEC_OK;
    } else {
        tmp_dec_frame->y = NULL;
        tmp_dec_frame->u = NULL;
        tmp_dec_frame->v = NULL;
        tmp_dec_frame->width = 0;
        tmp_dec_frame->height = 0;
        tmp_dec_frame->y_stride = 0;
        tmp_dec_frame->c_stride = 0;
        tmp_dec_frame->pic_format = PICTURE_FORMAT_BUTT;
        tmp_dec_frame->pts = 0;
        tmp_dec_frame->reserved = 0;
        tmp_dec_frame->user_data = NULL;

        return HI_MJPEG_NO_PICTURE;
    }
}

/* create a JPEG decoder handle */
jpegd_handle jpegd_get_handle(void)
{
    motion_jpeg_obj *mjpeg = &g_jpegd_ctx;
    int pic_size;

    mjpeg->max_width = JPEGD_MAX_WIDTH;
    mjpeg->max_height = JPEGD_MAX_HEIGHT;

    pic_size = mjpeg->max_width * mjpeg->max_height;
    mjpeg->buf_size = pic_size + 0x10000;
    mjpeg->stream_buffer = NULL;
    mjpeg->pic_buffer = NULL;
    mjpeg->valid_frame = 0;
    mjpeg->width_in_mcu = 1;
    mjpeg->height_in_mcu = 0;
    mjpeg->y_stride = 0;
    mjpeg->c_stride = 0;
    mjpeg->pic_format = PICTURE_FORMAT_BUTT;
    mjpeg->first_mcu = 0;
    mjpeg->bits.bit_len = 0;
    mjpeg->bits.bit_offset = 0;
    mjpeg->bits.buffer = NULL;
    mjpeg->frame.nf = 0;
    mjpeg->frame.y_height = 0;
    mjpeg->frame.y_width = 0;
    mjpeg->frame.restart_interval = 0;
    mjpeg->frame.restart_interval_logic = 0;
    mjpeg->frame.max_mcu_number = 0;

    init_default_huffman_table(mjpeg);
    init_default_quant_table(mjpeg);

    mjpeg->state = STATE_IDLE;

    return (jpegd_handle)mjpeg;
}

/* Prepare the jpegd Hardware Info */
int jpegd_prepare_hardware_info(motion_jpeg_obj *jpeg_hdl_ctx)
{
    unsigned int i, ysize;
    unsigned int huffman_table[12] = {0};
    unsigned char luma_ac_min_table[16] = {0};
    unsigned char luma_ac_base_table[16] = {0};
    unsigned char chroma_ac_min_table[16] = {0};
    unsigned char chroma_ac_base_table[16] = {0};

    ysize = ALIGN_UP(jpeg_hdl_ctx->frame.y_width, 64) * ALIGN_UP(jpeg_hdl_ctx->frame.y_height, 16);
    jpeg_hdl_ctx->vpu_config.width = jpeg_hdl_ctx->frame.y_width;
    jpeg_hdl_ctx->vpu_config.height = jpeg_hdl_ctx->frame.y_height;
    jpeg_hdl_ctx->vpu_config.width_in_mcu = jpeg_hdl_ctx->width_in_mcu;
    jpeg_hdl_ctx->vpu_config.height_in_mcu = jpeg_hdl_ctx->height_in_mcu;
    jpeg_hdl_ctx->vpu_config.c_phy_addr = jpeg_hdl_ctx->vpu_config.y_phy_addr + ysize;
    jpeg_hdl_ctx->vpu_config.y_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width, 64);
    jpeg_hdl_ctx->vpu_config.c_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width, 64);
    jpeg_hdl_ctx->vpu_config.phy_str_start = jpeg_hdl_ctx->vpu_config.phy_str_start + jpeg_hdl_ctx->stream_offest;
    jpeg_hdl_ctx->vpu_config.y_fac = jpeg_hdl_ctx->frame.h[0];
    jpeg_hdl_ctx->vpu_config.u_fac = jpeg_hdl_ctx->frame.h[1];
    jpeg_hdl_ctx->vpu_config.v_fac = jpeg_hdl_ctx->frame.h[2];
    jpeg_hdl_ctx->vpu_config.dri = jpeg_hdl_ctx->frame.restart_interval_logic;
    jpeg_hdl_ctx->vpu_config.pic_format = jpeg_hdl_ctx->pic_format;
    switch (jpeg_hdl_ctx->vpu_config.pic_format) {
        case PICTURE_FORMAT_YUV420: {
            jpeg_hdl_ctx->vpu_config.pic_type = 3;
            break;
        }
        case PICTURE_FORMAT_YUV422: {
            jpeg_hdl_ctx->vpu_config.pic_type = 4;
            break;
        }
        case PICTURE_FORMAT_YUV444: {
            jpeg_hdl_ctx->vpu_config.pic_type = 6;
            break;
        }
        case PICTURE_FORMAT_YUV422V: {
            jpeg_hdl_ctx->vpu_config.pic_type = 5;
            break;
        }
        case PICTURE_FORMAT_YUV400: {
            jpeg_hdl_ctx->vpu_config.pic_type = 0;
            break;
        }
        default:
        {
            printf("Unkonwn picture format %d!", jpeg_hdl_ctx->vpu_config.pic_format);
            return HI_FAILURE;
        }
    }

    if (jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        jpeg_hdl_ctx->vpu_config.out_yuv = HI_TRUE;
    } else {
        jpeg_hdl_ctx->vpu_config.out_yuv = HI_FALSE;
        if ((jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_ARGB_8888) ||
            (jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_ABGR_8888)) {
            jpeg_hdl_ctx->vpu_config.rgb_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width * 4, 16);
        } else if ((jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_ARGB_1555) ||
               (jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_ABGR_1555)) {
            jpeg_hdl_ctx->vpu_config.rgb_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width * 2, 16);
        } else if ((jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_RGB_888) ||
               (jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_BGR_888)) {
            jpeg_hdl_ctx->vpu_config.rgb_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width * 3, 16);
        } else if ((jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_RGB_565) ||
               (jpeg_hdl_ctx->vpu_config.pixel_format == PIXEL_FORMAT_BGR_565)) {
            jpeg_hdl_ctx->vpu_config.rgb_stride = ALIGN_UP(jpeg_hdl_ctx->frame.y_width * 2, 16);
        }
    }

    /* config quant table */
    {
        unsigned int q_tab_y_index = jpeg_hdl_ctx->frame.tq[0] & 3;
        unsigned int q_tab_u_index = jpeg_hdl_ctx->frame.tq[1] & 3;
        unsigned int q_tab_v_index = jpeg_hdl_ctx->frame.tq[2] & 3;

        unsigned char *q_cr = (unsigned char *)&jpeg_hdl_ctx->q_tab[q_tab_v_index];
        unsigned char *q_cb = (unsigned char *)&jpeg_hdl_ctx->q_tab[q_tab_u_index];
        unsigned char *q_y = (unsigned char *)&jpeg_hdl_ctx->q_tab[q_tab_y_index];
        int pos;

        for (i = 0; i < QUANT_TABLE_SIZE; i++) {
            pos = zigzag_for_qtable[i & 0x3f];
            jpeg_hdl_ctx->vpu_config.quant_table[pos] = q_y[i] + (q_cb[i] << 8) + (q_cr[i] << 16);
        }
    }

    {
        huffman_tab *huf_tab = &jpeg_hdl_ctx->h_tab[1];
        unsigned int i, num, j, index;

        for (index = 0, i = 0; i < 16; i++) {
            num = huf_tab->len[i];
            for (j = 0; (j < num) && (index < 12); j++, index++) {
                int pos = huf_tab->huffman_val[index];
                huffman_table[pos] |= ((i + 1) << 20) + (((j + (unsigned int)huf_tab->min_code[i]) << 12) & 0xFF000);
            }
        }
    }

    {
        huffman_tab *huf_tab = &jpeg_hdl_ctx->h_tab[0];
        unsigned int i, num, j, index;

        for (index = 0, i = 0; i < 16; i++) {
            num = huf_tab->len[i];
            for (j = 0; (j < num) && (index < 12); j++, index++) {
                int pos = huf_tab->huffman_val[index];
                huffman_table[pos] |= ((i + 1) << 8) + ((j + (unsigned int)huf_tab->min_code[i]) & 0xFF);
            }
        }
    }

    {
        huffman_tab *huf_tab = &jpeg_hdl_ctx->h_tab[2];
        int i;

        for (i = 0; i < 16; i++) {
            unsigned int base_code = 0;

            if (huf_tab->len[i]) {
                base_code = huf_tab->huffman_val_ptr[i] - huf_tab->min_code[i];
            }

            luma_ac_min_table[i] = ((unsigned int)huf_tab->min_code[i]) & 0xFF;
            luma_ac_base_table[i] = base_code & 0xFF;
        }
    }

    {
        huffman_tab *huf_tab = &jpeg_hdl_ctx->h_tab[3];
        int i;

        for (i = 0; i < 16; i++) {
            unsigned int base_code = 0;

            if (huf_tab->len[i]) {
                base_code = huf_tab->huffman_val_ptr[i] - huf_tab->min_code[i];
            }

            chroma_ac_min_table[i] = ((unsigned int)huf_tab->min_code[i]) & 0xFF;
            chroma_ac_base_table[i] = base_code & 0xFF;
        }
    }

    /* config huffman table */
    {
        for (i = 0; i < HDC_TABLE_SIZE; i++) {
            jpeg_hdl_ctx->vpu_config.huffman_table[i] = huffman_table[i];
        }
    }

    /* config huffman_min_table table */
    {
        for (i = 0; i < HAC_MIN_TABLE_SIZE; i++) {
            jpeg_hdl_ctx->vpu_config.huffman_min_table[i] =
            (chroma_ac_min_table[2 * i + 1] << 24) + (chroma_ac_min_table[2 * i] << 16) +
            (luma_ac_min_table[2 * i + 1] << 8) +
            (luma_ac_min_table[2 * i]);
        }
    }

    /* config huffman_base_table table */
    {
        for (i = 0; i < HAC_BASE_TABLE_SIZE; i++) {
            jpeg_hdl_ctx->vpu_config.huffman_base_table[i] =
            (chroma_ac_base_table[2 * i + 1] << 24) + (chroma_ac_base_table[2 * i] << 16) +
            (luma_ac_base_table[2 * i + 1] << 8) +
            (luma_ac_base_table[2 * i]);
        }
    }

    /* config huffman_symbol_table table */
    {
        for (i = 0; i < HAC_SYMBOL_TABLE_SIZE; i++) {
            jpeg_hdl_ctx->vpu_config.huffman_symbol_table[i] =
            (jpeg_hdl_ctx->h_tab[3].huffman_val[i] << 8) + jpeg_hdl_ctx->h_tab[2].huffman_val[i];
        }
    }

    return HI_SUCCESS;
}

int jpegd_start_one_frame(jpegd_handle handle, unsigned int flags)
{
    int ret;
    motion_jpeg_obj *jpegd_hdl_ctx = (motion_jpeg_obj *)handle;

    ret = jpegd_dec_frame(handle, flags);
    if (ret != HI_SUCCESS) {
    HI_TRACE("vdec_start_one_frame: decode stream fail for 0x%x\n", ret);
        return ret;
    }

    ret = jpegd_prepare_hardware_info(jpegd_hdl_ctx);
    if (ret != HI_SUCCESS) {
    HI_TRACE("vdec_start_one_frame: decode stream fail for 0x%x\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

/* write the jpegd register */
void jpegd_write_regs(jpegd_handle handle, void *reg_base)
{
    motion_jpeg_obj *jpeg_hdl_ctx = (motion_jpeg_obj *)handle;

    jpegd_drv_write_regs(reg_base, &jpeg_hdl_ctx->vpu_config);
    return;
}

/* read the jpegd register */
void jpegd_read_regs(jpegd_handle handle, void *reg_base)
{
    motion_jpeg_obj *jpeg_hdl_ctx = (motion_jpeg_obj *)handle;

    jpegd_drv_read_regs(reg_base, &jpeg_hdl_ctx->vpu_status);
    return;
}

int jpegd_start_decoding(jpegd_handle handle)
{
    motion_jpeg_obj *jpegd_hld_ctx;
    int ret;

    jpegd_hld_ctx = (motion_jpeg_obj *)handle;
    jpegd_hld_ctx->vpu_config.y_phy_addr = g_video_data_base;
    jpegd_hld_ctx->vpu_config.phy_str_buf_start = g_hilogo;
    jpegd_hld_ctx->vpu_config.phy_str_buf_end = g_hilogo + ALIGN_UP(g_jpeg_size, 128);
    jpegd_hld_ctx->vpu_config.phy_str_start = g_hilogo;
    jpegd_hld_ctx->vpu_config.phy_str_end = g_hilogo + g_jpeg_size;
    jpegd_hld_ctx->vpu_config.phy_emar_buffer0 = g_jpegd_emar_buf;
    jpegd_hld_ctx->vpu_config.phy_emar_buffer1 = g_jpegd_emar_buf + ONE_EMAR_BUF_SIZE;
    jpegd_hld_ctx->stream.vir_addr = (unsigned char *)g_hilogo;
    jpegd_hld_ctx->stream.len = g_jpeg_size;
    jpegd_hld_ctx->stream.phy_addr = g_hilogo;
    jpegd_hld_ctx->stream.pts = 0;
    jpegd_hld_ctx->vpu_config.chn_id = 0;
    jpegd_hld_ctx->vpu_config.alpha = 255;

    if (g_output_format == 0) {
        jpegd_hld_ctx->vpu_config.pixel_format = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    } else if (g_output_format == 1) {
        jpegd_hld_ctx->vpu_config.pixel_format = PIXEL_FORMAT_ARGB_1555;
    } else if (g_output_format == 2) {
        jpegd_hld_ctx->vpu_config.pixel_format = PIXEL_FORMAT_ARGB_8888;
    } else {
        jpegd_hld_ctx->vpu_config.pixel_format = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    }

    ret = jpegd_start_one_frame(handle, 0);
    if (ret != HI_SUCCESS) {
    HI_TRACE("jpegd_start_decoding: decode stream fail for 0x%x\n", ret);
        return ret;
    }

    jpegd_set_clock_en(0, HI_TRUE);
    jpegd_reset(0, HI_NULL);
    jpegd_set_time_out(0, 0xFFFFFFFF);

    jpegd_write_regs(handle, (void *)JPEGD_REGS_ADDR);

    jpegd_start_vpu(0);

    return HI_SUCCESS;
}

void jpegd_finish_decoding(jpegd_handle handle)
{
    unsigned int int_statue;
    motion_jpeg_obj *jpegd_hld_ctx = (motion_jpeg_obj *)handle;

    while (1) {
        udelay(10);
        int_statue = jpegd_read_int(0);
        if (int_statue & 0x1f) {
            break;
        }
    }

    jpegd_read_regs(handle, (void *)JPEGD_REGS_ADDR);
    if (jpegd_hld_ctx->vpu_status.int_dec_finish == 0) {
        printf("hardware decoding error!\n");
    } else {
        if (jpegd_hld_ctx->vpu_config.out_yuv != HI_TRUE) {
            printf("hardware decoding success! %dx%d, stride %d.\n",
                jpegd_hld_ctx->frame.y_width, jpegd_hld_ctx->frame.y_height, jpegd_hld_ctx->vpu_config.rgb_stride);
        } else {
            printf("hardware decoding success! %dx%d, stride %d.\n",
                jpegd_hld_ctx->frame.y_width, jpegd_hld_ctx->frame.y_height, jpegd_hld_ctx->vpu_config.y_stride);
        }
    }

    jpegd_clear_int(0);
    jpegd_reset_select(0, HI_TRUE);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

