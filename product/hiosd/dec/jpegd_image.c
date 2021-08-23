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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static const unsigned char default_huffman_table[418] = { 0x01, 0xA2,
    // Table K.3 Table for luminance DC coefficient differences
    0x00,
    0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0A, 0x0B,

    // Table K.4 Table for chrominance DC coefficient differences
    0x01,
    0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0A, 0x0B,

    // Table K.5 Table for luminance AC coefficients
    0x10,
    0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7D,
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA,

    // Table K.6 Table for chrominance AC coefficients
    0x11,
    0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77,
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
    0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
    0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
    0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
    0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
    0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA,
};

static const unsigned char default_quant_table[160] = { 0x00, 0x84,
    // Table K.1 Luminance quantization table
    0x00,
    16,  11,  12,  14,  12,  10,  16,  14,  13,  14,  18,  17,  16,  19,  24,  40,
    26,  24,  22,  22,  24,  49,  35,  37,  29,  40,  58,  51,  61,  60,  57,  51,
    56,  55,  64,  72,  92,  78,  64,  68,  87,  69,  55,  56,  80, 109,  81,  87,
    95,  98, 103, 104, 103,  62,  77, 113, 121, 112, 100, 120,  92, 101, 103,  99,

    // Table K.2 Chrominance quantization table
    0x01,
    17, 18, 18, 24, 21, 24, 47, 26, 26, 47, 99, 66, 56, 66, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
};

int decode_dht(motion_jpeg_obj *mjpeg, const unsigned char *stream, unsigned int len)
{
    huffman_tab *huffman = &mjpeg->huffman_tmp;
    unsigned int error_flag = 0;
    unsigned int i, tab_len;
    unsigned char ptr[16];
    unsigned int k = 2;
    unsigned int lh;
    unsigned char code, tc, th;
    unsigned int ii = 0;
    unsigned int jj;

    lh = (stream[0] << 8) + stream[1];
    stream += 2;

    while (k < lh) {
        int max_code = 0;
        code = *stream++;
        tc = (code >> 4) & 0x1;
        th = code & 0x1;
        error_flag |= (unsigned int)(code & 0xEE);
        if (error_flag != 0) {
            return HI_JPEG_DEC_DHT_ERR;
        }

        for (i = 0, tab_len = 0; i < 16; i++) {
            ptr[i] = *stream++;

            if (ptr[i]) {
                huffman->min_code[i] = (unsigned int)(max_code) << 1;
                huffman->max_code[i] = huffman->min_code[i] + ptr[i] - 1;
                huffman->max_code16[i] = ((unsigned int)(huffman->max_code[i] + 1) << (15 - i)) - 1;
                huffman->len[i] = ptr[i];
                huffman->huffman_val_ptr[i] = tab_len;
                max_code = huffman->max_code[i] + 1;
                error_flag |= (unsigned int)((huffman->max_code[i] + 1) > (1 << (i + 1)));
            } else {
                huffman->min_code[i] = (unsigned int)max_code << 1;
                huffman->max_code[i] = -1;
                huffman->max_code16[i] = -1;
                huffman->len[i] = 0;
                huffman->huffman_val_ptr[i] = tab_len;
                max_code = (unsigned int)max_code << 1;
            }
            tab_len += ptr[i];
        }
        huffman->max_code16[15] = 0x0000ffff;
        huffman->max_code[15] = huffman->min_code[15] + ptr[15] - 1;
        k += 17 + tab_len;
        error_flag |= (unsigned int)(tab_len > 255);

        if ((k > lh) || error_flag) {
            return HI_JPEG_DEC_DHT_ERR;
        }

        memset(&huffman->huffman_val[0], 0, sizeof(huffman->huffman_val));
        for (i = 0; (i < tab_len) && (i < 256); i++) {
            huffman->huffman_val[i] = *stream++;
        }

        for (i = 0, jj = 0; (i < 8) && (jj < 256); i++) {
            for (ii = 0; ii < ptr[i]; ii++) {
                int count = 1 << (7 - i);
                unsigned char huffmancode = huffman->huffman_val[huffman->huffman_val_ptr[i] + ii];
                while (count--) {
                    huffman->fast_huffman_len[jj] = i + 1;
                    huffman->fast_huffman_code[jj++] = huffmancode;
                }
            }
        }
        huffman->u_code32 = (jj - 1) << 24;
        mjpeg->h_tab[(tc << 1) + th] = *huffman;
    }

    return HI_JPEG_DEC_OK;
}

int decode_sof0(motion_jpeg_obj *mjpeg, const unsigned char *stream, unsigned int len)  // 0xFF C0
{
    unsigned int error_flag = 0;
    frame_header header = {0};
    int ls;

    ls = (stream[0] << 8) + stream[1];
    header.color_depth = stream[2];
    header.y_height = (stream[3] << 8) + stream[4];
    header.y_width = (stream[5] << 8) + stream[6];
    header.nf = stream[7];
    stream += 8;

    error_flag |= (unsigned int)(header.color_depth != 8);
    error_flag |= (unsigned int)(ls != (header.nf * 3 + 8));
    error_flag |= (unsigned int)(header.y_height > mjpeg->max_height);
    error_flag |= (unsigned int)(header.y_width > mjpeg->max_width);
    error_flag |= (unsigned int)(header.y_width == 0);
    error_flag |= (unsigned int)(header.y_height == 0);
    error_flag |= (unsigned int)((header.nf != 1) && (header.nf != 3));

    if (header.color_depth != 8) {
        return HI_ERR_NOT_BASELINE;
    }

    if (ls != (header.nf * 3 + 8)) {
        return HI_ERR_FRAME_HEADER;
    }

    if ((header.y_height > mjpeg->max_height) || (header.y_height == 0)) {
        return HI_UNSUPPORT_PIC_SIZE;
    }

    if ((header.y_width > mjpeg->max_width) || (header.y_width == 0)) {
        return HI_UNSUPPORT_PIC_SIZE;
    }
    if ((header.nf != 3) && (header.nf != 1)) {
        return HI_UNSUPPORT_PIC_STRUCT;
    }

    if (header.nf == 1) {
        header.ci[0] = stream[0];
        header.h[0] = stream[1];
        header.h[1] = 0;
        header.h[2] = 0;
        header.tq[0] = stream[2];
        header.tq[1] = header.tq[0];
        header.tq[2] = header.tq[0];
        error_flag |= (unsigned int)(header.h[0] != 0x11);  // only support H = V = 1;
        error_flag |= (unsigned int)(header.tq[0] & 0xfc);  // tq = (0 ~ 3), inclusive
        if (error_flag != 0) {
            return HI_UNSUPPORT_PIC_STRUCT;
        }
        mjpeg->width_in_mcu = (header.y_width + 7) >> 3;
        mjpeg->height_in_mcu = (header.y_height + 7) >> 3;
        mjpeg->y_stride = mjpeg->width_in_mcu << 3;
        mjpeg->pic_format = PICTURE_FORMAT_YUV400;  // YUV100
    }

    if (header.nf == 3) {
        header.ci[0] = stream[0];
        header.h[0] = stream[1];
        header.tq[0] = stream[2];
        header.ci[1] = stream[3];
        header.h[1] = stream[4];
        header.tq[1] = stream[5];
        header.ci[2] = stream[6];
        header.h[2] = stream[7];
        header.tq[2] = stream[8];

        error_flag |= (unsigned int)(header.h[1] != 0x11);  // only support H = V = 1;
        error_flag |= (unsigned int)(header.h[2] != 0x11);  // only support H = V = 1;
        error_flag |= (unsigned int)(header.tq[0] & 0xfc);  // tq = (0 ~ 3), inclusive
        error_flag |= (unsigned int)(header.tq[1] & 0xfc);  // tq = (0 ~ 3), inclusive
        error_flag |= (unsigned int)(header.tq[2] & 0xfc);  // tq = (0 ~ 3), inclusive

        if ((header.tq[0] & 0xfc) || (header.tq[1] & 0xfc) || (header.tq[2] & 0xfc)) {
            return HI_ERR_NOT_BASELINE;
        }

        if ((header.h[0] == 0x41) || (header.h[0] == 0x14)) {
            return HI_UNSUPPORT_YUV411;
        }

        if ((header.h[1] != 0x11) || (header.h[2] != 0x11)) {
            return HI_UNSUPPORT_PIC_STRUCT;
        }

        if (header.h[0] == 0x11) {
            mjpeg->width_in_mcu = (header.y_width + 7) >> 3;
            mjpeg->height_in_mcu = (header.y_height + 7) >> 3;
            mjpeg->y_stride = mjpeg->width_in_mcu << 3;
            mjpeg->c_stride = mjpeg->width_in_mcu << 3;
            mjpeg->pic_format = PICTURE_FORMAT_YUV444;  // YUV111
        } else if (header.h[0] == 0x22) {
            mjpeg->width_in_mcu = (header.y_width + 15) >> 4;
            mjpeg->height_in_mcu = (header.y_height + 15) >> 4;
            mjpeg->y_stride = mjpeg->width_in_mcu << 4;
            mjpeg->c_stride = mjpeg->width_in_mcu << 3;
            mjpeg->pic_format = PICTURE_FORMAT_YUV420;  // YUV420
        } else if (header.h[0] == 0x21) {
            mjpeg->width_in_mcu = (header.y_width + 15) >> 4;
            mjpeg->height_in_mcu = (header.y_height + 7) >> 3;
            mjpeg->y_stride = mjpeg->width_in_mcu << 4;
            mjpeg->c_stride = mjpeg->width_in_mcu << 3;
            mjpeg->pic_format = PICTURE_FORMAT_YUV422;  // YUV422
        } else if (header.h[0] == 0x12) {
            mjpeg->width_in_mcu = (header.y_width + 7) >> 3;
            mjpeg->height_in_mcu = (header.y_height + 15) >> 4;
            mjpeg->y_stride = mjpeg->width_in_mcu << 3;
            mjpeg->c_stride = mjpeg->width_in_mcu << 3;
            mjpeg->pic_format = PICTURE_FORMAT_YUV422V;
        } else {
            mjpeg->pic_format = PICTURE_FORMAT_BUTT;  // do not support
            return HI_UNSUPPORT_PIC_STRUCT;
        }
    }

    header.max_mcu_number = mjpeg->width_in_mcu * mjpeg->height_in_mcu;
    header.restart_interval_logic = mjpeg->frame.restart_interval;
    header.restart_interval = (mjpeg->frame.restart_interval != 0) ? mjpeg->frame.restart_interval : header.max_mcu_number;
    mjpeg->frame = header;
    return HI_JPEG_DEC_OK;
}

int decode_sos(motion_jpeg_obj *mjpeg, unsigned char *stream, unsigned int len)  // 0xFF DA
{
    unsigned int error_flag = 0;
    scan_header scan;
    unsigned char code;
    unsigned int i;
    frame_header *fh = &mjpeg->frame;
    unsigned int ls = (stream[0] << 8) + stream[1];
    unsigned int ns = stream[2];
    stream += 3;

    error_flag |= (unsigned int)(ns != fh->nf);  // ns should be equal to nf in frame header
    error_flag |= (unsigned int)(ls != (ns * 2 + 6));

    if (error_flag || (ns > 4)) {
        return HI_ERR_SCAN_HEADER;
    }

    for (i = 0; (i < ns) && (error_flag == 0); i++) {
        scan.cs[i] = *stream++;
        code = *stream++;
        scan.td[i] = code >> 4;
        scan.ta[i] = code & 0xf;
        if (i < 3) {
            mjpeg->q_tab_components[i] = &mjpeg->q_tab[(3 & fh->tq[i])];
            mjpeg->h_tab_components_dc[i] = &mjpeg->h_tab[(1 & scan.td[i])];
            mjpeg->h_tab_components_ac[i] = &mjpeg->h_tab[2 + (1 & scan.ta[i])];
        }

        error_flag |= (unsigned int)(scan.cs[i] != fh->ci[i]);
        error_flag |= (unsigned int)(scan.td[i] > 1);  // baseline: td  = (0 ~ 1), inclusive
        error_flag |= (unsigned int)(scan.ta[i] > 1);  // baseline: ta  = (0 ~ 1), inclusive

        if ((scan.td[i] > 1) || (scan.ta[i] > 1)) {
            return HI_ERR_NOT_BASELINE;
        }

        if (scan.cs[i] != fh->ci[i]) {
            return HI_ERR_SCAN_HEADER;
        }
    }
    scan.ns = ns;
    scan.ss = *stream++;
    scan.se = *stream++;
    code = *stream++;
    scan.ah = code >> 4;
    scan.al = code & 0xf;

    if ((scan.ss != 0) || (scan.se != 63) || (scan.ah != 0) || (scan.al != 0)) {
        return HI_ERR_NOT_BASELINE;
    }

    if (error_flag == 0) {
        mjpeg->scan = scan;
        return HI_JPEG_DEC_OK;
    } else {
        return HI_JPEG_DEC_SCAN_ERR;
    }
}

int decode_dri(motion_jpeg_obj *mjpeg, const unsigned char *stream, unsigned int len)
{
    int lr = (stream[0] << 8) + stream[1];
    int ri = (stream[2] << 8) + stream[3];

    if (lr != 4) {
        return 1;
    }

    mjpeg->frame.restart_interval = ri;
    mjpeg->frame.restart_interval_logic = ri;

    if (mjpeg->frame.max_mcu_number == 0) {
        mjpeg->frame.restart_interval = ri;
    } else {
        mjpeg->frame.restart_interval = (ri == 0) ? mjpeg->frame.max_mcu_number : ri;
    }

    return 0;
}

int decode_dqt(motion_jpeg_obj *mjpeg, const unsigned char *stream, unsigned int len)
{
    unsigned int error_flag = 0;
    unsigned char *quant;
    unsigned int i, code, pq, tq;
    int k = 2;
    int lq = (stream[0] << 8) + stream[1];
    stream += 2;

    while ((k < lq) && (error_flag == 0)) {
        k += 65;
        code = *stream++;
        pq = code >> 4;
        tq = code & 0xf;
        error_flag |= (unsigned int)(pq != 0);  // pq must be 0 in baseline
        error_flag |= (unsigned int)(tq > 3);  // tq should be (0 ~ 3), inclusive
        error_flag |= (unsigned int)(k > lq);
        if (error_flag) {
            return HI_JPEG_DEC_FRAME_ERR;
        }

        quant = mjpeg->q_tab[tq & 3].quant;
        for (i = 0; i < 64; i++) {
            quant[i] = *stream++;
        }
    }
    return 0;
}

void init_default_huffman_table(motion_jpeg_obj *mjpeg)
{
    int ret;
    ret = decode_dht(mjpeg, default_huffman_table, 0x01A2);
    if (ret != HI_JPEG_DEC_OK) {
        HI_TRACE("jpeg decode DHT error!\n");
    }
}

void init_default_quant_table(motion_jpeg_obj *mjpeg)
{
    int ret;
    ret = decode_dqt(mjpeg, default_quant_table, 0x0084);
    if (ret != 0) {
        HI_TRACE("jpeg decode DQT error!\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



