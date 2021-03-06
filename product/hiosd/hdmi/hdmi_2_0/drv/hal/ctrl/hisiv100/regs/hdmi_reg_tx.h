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

#ifndef __TX_HDMI_REG_C_UNION_DEFINE_H__
#define __TX_HDMI_REG_C_UNION_DEFINE_H__

typedef union {
    struct {
        unsigned int tmds_pack_mode       : 2;  /* [1:0] */
        unsigned int reg_fifo_auto_rst_en : 1;  /* [2] */
        unsigned int reg_fifo_manu_rst    : 1;  /* [3] */
        unsigned int reg_clock_det_en     : 1;  /* [4] */
        unsigned int reg_ext_tmds_para    : 1;  /* [5] */
        unsigned int rsv_0                : 2;  /* [7:6] */
        unsigned int reg_fifo_delay_cnt   : 8;  /* [15:8] */
        unsigned int rsv_1                : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} U_TX_PACK_FIFO_CTRL;

typedef union {
    struct {
        unsigned int pclk2tclk_stable : 1;  /* [0] */
        unsigned int rsv_2            : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} U_TX_PACK_FIFO_ST;

typedef union {
    struct {
        unsigned int reg_pclk_refer_cnt : 18; /* [17:0] */
        unsigned int rsv_3              : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} U_PCLK_REFER_CNT;

typedef union {
    struct {
        unsigned int reg_tcnt_lower_threshold : 18; /* [17:0] */
        unsigned int rsv_4                    : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} U_TCLK_LOWER_THRESHOLD;

typedef union {
    struct {
        unsigned int reg_tcnt_upper_threshold : 18; /* [17:0] */
        unsigned int rsv_5                    : 14; /* [31:18] */
    } bits;
    unsigned int u32;
} U_TCLK_UPPER_THRESHOLD;

typedef union {
    struct {
        unsigned int avi_pkt_hb0 : 8; /* [7:0] */
        unsigned int avi_pkt_hb1 : 8; /* [15:8] */
        unsigned int avi_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_6       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_PKT_HEADER;

typedef union {
    struct {
        unsigned int avi_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int avi_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_7            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int avi_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int avi_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_8            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int avi_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int avi_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_9            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int avi_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int avi_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int avi_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int avi_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int avi_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int avi_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int avi_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_10           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AVI_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int aif_pkt_hb0 : 8; /* [7:0] */
        unsigned int aif_pkt_hb1 : 8; /* [15:8] */
        unsigned int aif_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_11      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_PKT_HEADER;

typedef union {
    struct {
        unsigned int aif_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int aif_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_12           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int aif_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int aif_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_13           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int aif_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int aif_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_14           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int aif_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int aif_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int aif_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int aif_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int aif_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int aif_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int aif_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_15           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_AIF_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int spd_pkt_hb0 : 8; /* [7:0] */
        unsigned int spd_pkt_hb1 : 8; /* [15:8] */
        unsigned int spd_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_16      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_PKT_HEADER;

typedef union {
    struct {
        unsigned int spd_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int spd_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_17           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int spd_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int spd_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_18           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int spd_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int spd_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_19           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int spd_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int spd_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int spd_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int spd_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int spd_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int spd_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int spd_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_20           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_SPIF_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int mpeg_pkt_hb0 : 8; /* [7:0] */
        unsigned int mpeg_pkt_hb1 : 8; /* [15:8] */
        unsigned int mpeg_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_21       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_PKT_HEADER;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_22            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_23            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_24            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int mpeg_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int mpeg_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int mpeg_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int mpeg_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_25            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_MPEG_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gen_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_26      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_PKT_HEADER;

typedef union {
    struct {
        unsigned int gen_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gen_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_27           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gen_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gen_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_28           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gen_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gen_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_29           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gen_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gen_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_30           : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gen2_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen2_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen2_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_31       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_PKT_HEADER;

typedef union {
    struct {
        unsigned int gen2_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gen2_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_32            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gen2_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gen2_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_33            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gen2_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gen2_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_34            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gen2_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen2_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gen2_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen2_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen2_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_35            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN2_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gen3_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen3_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen3_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_36       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_PKT_HEADER;

typedef union {
    struct {
        unsigned int gen3_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gen3_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_37            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gen3_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gen3_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_38            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gen3_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gen3_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_39            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gen3_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen3_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gen3_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen3_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen3_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_40            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN3_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gen4_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen4_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen4_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_41       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_PKT_HEADER;

typedef union {
    struct {
        unsigned int gen4_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gen4_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_42            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gen4_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gen4_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_43            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gen4_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gen4_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_44            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gen4_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen4_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gen4_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen4_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen4_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_45            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN4_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gen5_pkt_hb0 : 8; /* [7:0] */
        unsigned int gen5_pkt_hb1 : 8; /* [15:8] */
        unsigned int gen5_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_46       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_PKT_HEADER;

typedef union {
    struct {
        unsigned int gen5_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gen5_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_47            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gen5_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gen5_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_48            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gen5_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gen5_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_49            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gen5_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gen5_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gen5_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gen5_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gen5_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_50            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GEN5_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int gamut_pkt_hb0 : 8; /* [7:0] */
        unsigned int gamut_pkt_hb1 : 8; /* [15:8] */
        unsigned int gamut_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_51        : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_PKT_HEADER;

typedef union {
    struct {
        unsigned int gamut_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int gamut_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_52             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int gamut_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int gamut_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_53             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int gamut_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int gamut_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_54             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int gamut_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int gamut_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int gamut_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int gamut_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int gamut_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_55             : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_GAMUT_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int vsif_pkt_hb0 : 8; /* [7:0] */
        unsigned int vsif_pkt_hb1 : 8; /* [15:8] */
        unsigned int vsif_pkt_hb2 : 8; /* [23:16] */
        unsigned int rsv_56       : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_PKT_HEADER;

typedef union {
    struct {
        unsigned int vsif_sub_pkt0_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt0_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt0_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt0_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT0_L;

typedef union {
    struct {
        unsigned int vsif_sub_pkt0_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt0_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt0_pb6 : 8; /* [23:16] */
        unsigned int rsv_57            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT0_H;

typedef union {
    struct {
        unsigned int vsif_sub_pkt1_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt1_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt1_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt1_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT1_L;

typedef union {
    struct {
        unsigned int vsif_sub_pkt1_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt1_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt1_pb6 : 8; /* [23:16] */
        unsigned int rsv_58            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT1_H;

typedef union {
    struct {
        unsigned int vsif_sub_pkt2_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt2_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt2_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt2_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT2_L;

typedef union {
    struct {
        unsigned int vsif_sub_pkt2_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt2_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt2_pb6 : 8; /* [23:16] */
        unsigned int rsv_59            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT2_H;

typedef union {
    struct {
        unsigned int vsif_sub_pkt3_pb0 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt3_pb1 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt3_pb2 : 8; /* [23:16] */
        unsigned int vsif_sub_pkt3_pb3 : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT3_L;

typedef union {
    struct {
        unsigned int vsif_sub_pkt3_pb4 : 8; /* [7:0] */
        unsigned int vsif_sub_pkt3_pb5 : 8; /* [15:8] */
        unsigned int vsif_sub_pkt3_pb6 : 8; /* [23:16] */
        unsigned int rsv_60            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} U_VSIF_SUB_PKT3_H;

typedef union {
    struct {
        unsigned int cea_avi_en     : 1;  /* [0] */
        unsigned int cea_avi_rpt_en : 1;  /* [1] */
        unsigned int rsv_61         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_AVI_CFG;

typedef union {
    struct {
        unsigned int cea_spf_en     : 1;  /* [0] */
        unsigned int cea_spf_rpt_en : 1;  /* [1] */
        unsigned int rsv_62         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_SPF_CFG;

typedef union {
    struct {
        unsigned int cea_aud_en     : 1;  /* [0] */
        unsigned int cea_aud_rpt_en : 1;  /* [1] */
        unsigned int rsv_63         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_AUD_CFG;

typedef union {
    struct {
        unsigned int cea_mpeg_en     : 1;  /* [0] */
        unsigned int cea_mpeg_rpt_en : 1;  /* [1] */
        unsigned int rsv_64          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_MPEG_CFG;

typedef union {
    struct {
        unsigned int cea_gen_en     : 1;  /* [0] */
        unsigned int cea_gen_rpt_en : 1;  /* [1] */
        unsigned int rsv_65         : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GEN_CFG;

typedef union {
    struct {
        unsigned int cea_cp_en      : 1;  /* [0] */
        unsigned int cea_cp_rpt_en  : 1;  /* [1] */
        unsigned int cea_cp_rpt_cnt : 8;  /* [2:7] */
        unsigned int rsv_66         : 22; /* [31:8] */
    } bits;
    unsigned int u32;
} U_CEA_CP_CFG;

typedef union {
    struct {
        unsigned int cea_gen2_en     : 1;  /* [0] */
        unsigned int cea_gen2_rpt_en : 1;  /* [1] */
        unsigned int rsv_67          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GEN2_CFG;

typedef union {
    struct {
        unsigned int cea_gen3_en     : 1;  /* [0] */
        unsigned int cea_gen3_rpt_en : 1;  /* [1] */
        unsigned int rsv_68          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GEN3_CFG;

typedef union {
    struct {
        unsigned int cea_gen4_en     : 1;  /* [0] */
        unsigned int cea_gen4_rpt_en : 1;  /* [1] */
        unsigned int rsv_69          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GEN4_CFG;

typedef union {
    struct {
        unsigned int cea_gen5_en     : 1;  /* [0] */
        unsigned int cea_gen5_rpt_en : 1;  /* [1] */
        unsigned int rsv_70          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GEN5_CFG;

typedef union {
    struct {
        unsigned int cea_gamut_en     : 1;  /* [0] */
        unsigned int cea_gamut_rpt_en : 1;  /* [1] */
        unsigned int rsv_71           : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_GAMUT_CFG;

typedef union {
    struct {
        unsigned int cea_vsif_en     : 1;  /* [0] */
        unsigned int cea_vsif_rpt_en : 1;  /* [1] */
        unsigned int rsv_72          : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CEA_VSIF_CFG;

typedef union {
    struct {
        unsigned int cea_avi_state   : 1;  /* [0] */
        unsigned int cea_aud_state   : 1;  /* [1] */
        unsigned int cea_cp_state    : 1;  /* [2] */
        unsigned int cea_gen_state   : 1;  /* [3] */
        unsigned int cea_gen2_state  : 1;  /* [4] */
        unsigned int cea_gen3_state  : 1;  /* [5] */
        unsigned int cea_gen4_state  : 1;  /* [6] */
        unsigned int cea_gen5_state  : 1;  /* [7] */
        unsigned int cea_spd_state   : 1;  /* [8] */
        unsigned int cea_mpeg_state  : 1;  /* [9] */
        unsigned int cea_gamut_state : 1;  /* [10] */
        unsigned int cea_vsif_state  : 1;  /* [11] */
        unsigned int rsv_73          : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} U_CEA_PKT_STATE;

typedef union {
    struct {
        unsigned int hdmi_mode           : 1;  /* [0] */
        unsigned int dc_pkt_en           : 1;  /* [1] */
        unsigned int null_pkt_en         : 1;  /* [2] */
        unsigned int null_pkt_en_vs_high : 1;  /* [3] */
        unsigned int intr_encryption     : 1;  /* [4] */
        unsigned int pb_ovr_dc_pkt_en    : 1;  /* [5] */
        unsigned int pb_priotity_ctl     : 1;  /* [6] */
        unsigned int pkt_bypass_mode     : 1;  /* [7] */
        unsigned int avmute_in_phase     : 1;  /* [8] */
        unsigned int hdmi_dvi_sel        : 1;  /* [9] */
        unsigned int eess_mode_en        : 1;  /* [10] */
        unsigned int rsv_74              : 21; /* [31:11] */
    } bits;
    unsigned int u32;
} U_AVMIXER_CONFIG;

typedef union {
    struct {
        unsigned int cp_set_avmute : 1;  /* [0] */
        unsigned int cp_clr_avmute : 1;  /* [1] */
        unsigned int rsv_75        : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} U_CP_PKT_AVMUTE;

typedef union {
    struct {
        unsigned int video_blank : 24; /* [23:0] */
        unsigned int rsv_76      : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} U_VIDEO_BLANK_CFG;

typedef union {
    struct {
        unsigned int reg_tbist_en         : 1;  /* [0] */
        unsigned int reg_tbist_syn_pol    : 2;  /* [2:1] */
        unsigned int reg_tbist_timing_sel : 6;  /* [8:3] */
        unsigned int reg_tbist_patt_sel   : 5;  /* [13:9] */
        unsigned int rsv_77               : 18; /* [31:14] */
    } bits;
    unsigned int u32;
} U_TMDS_BIST_CTRL;

typedef union {
    struct {
        unsigned int enc_hdmi2_on   : 1;  /* [0] */
        unsigned int enc_scr_on     : 1;  /* [1] */
        unsigned int enc_scr_md     : 1;  /* [2] */
        unsigned int enc_hdmi_val   : 1;  /* [3] */
        unsigned int enc_hdmi_ovr   : 1;  /* [4] */
        unsigned int enc_bypass     : 1;  /* [5] */
        unsigned int enc_ck_div_sel : 2;  /* [7:6] */
        unsigned int rsv_78         : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} U_HDMI_ENC_CTRL;

typedef union {
    struct {
        unsigned int enc_ck_sharp0 : 10; /* [9:0] */
        unsigned int enc_ck_sharp1 : 10; /* [19:10] */
        unsigned int enc_ck_sharp2 : 10; /* [29:20] */
        unsigned int rsv_79        : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} U_ENC_CK_SHARP;

typedef struct {
    volatile U_TX_PACK_FIFO_CTRL    TX_PACK_FIFO_CTRL;    /* 1800 */
    volatile U_TX_PACK_FIFO_ST      TX_PACK_FIFO_ST;      /* 1804 */
    volatile U_PCLK_REFER_CNT       PCLK_REFER_CNT;       /* 1808 */
    volatile U_TCLK_LOWER_THRESHOLD TCLK_LOWER_THRESHOLD; /* 180C */
    volatile U_TCLK_UPPER_THRESHOLD TCLK_UPPER_THRESHOLD; /* 1810 */
    unsigned int                    Reserved_0[1];
    volatile U_AVI_PKT_HEADER       AVI_PKT_HEADER;       /* 1818 */
    volatile U_AVI_SUB_PKT0_L       AVI_SUB_PKT0_L;       /* 181C */
    volatile U_AVI_SUB_PKT0_H       AVI_SUB_PKT0_H;       /* 1820 */
    volatile U_AVI_SUB_PKT1_L       AVI_SUB_PKT1_L;       /* 1824 */
    volatile U_AVI_SUB_PKT1_H       AVI_SUB_PKT1_H;       /* 1828 */
    volatile U_AVI_SUB_PKT2_L       AVI_SUB_PKT2_L;       /* 182C */
    volatile U_AVI_SUB_PKT2_H       AVI_SUB_PKT2_H;       /* 1830 */
    volatile U_AVI_SUB_PKT3_L       AVI_SUB_PKT3_L;       /* 1834 */
    volatile U_AVI_SUB_PKT3_H       AVI_SUB_PKT3_H;       /* 1838 */
    volatile U_AIF_PKT_HEADER       AIF_PKT_HEADER;       /* 183C */
    volatile U_AIF_SUB_PKT0_L       AIF_SUB_PKT0_L;       /* 1840 */
    volatile U_AIF_SUB_PKT0_H       AIF_SUB_PKT0_H;       /* 1844 */
    volatile U_AIF_SUB_PKT1_L       AIF_SUB_PKT1_L;       /* 1848 */
    volatile U_AIF_SUB_PKT1_H       AIF_SUB_PKT1_H;       /* 184C */
    volatile U_AIF_SUB_PKT2_L       AIF_SUB_PKT2_L;       /* 1850 */
    volatile U_AIF_SUB_PKT2_H       AIF_SUB_PKT2_H;       /* 1854 */
    volatile U_AIF_SUB_PKT3_L       AIF_SUB_PKT3_L;       /* 1858 */
    volatile U_AIF_SUB_PKT3_H       AIF_SUB_PKT3_H;       /* 185C */
    volatile U_SPIF_PKT_HEADER      SPIF_PKT_HEADER;      /* 1860 */
    volatile U_SPIF_SUB_PKT0_L      SPIF_SUB_PKT0_L;      /* 1864 */
    volatile U_SPIF_SUB_PKT0_H      SPIF_SUB_PKT0_H;      /* 1868 */
    volatile U_SPIF_SUB_PKT1_L      SPIF_SUB_PKT1_L;      /* 186C */
    volatile U_SPIF_SUB_PKT1_H      SPIF_SUB_PKT1_H;      /* 1870 */
    volatile U_SPIF_SUB_PKT2_L      SPIF_SUB_PKT2_L;      /* 1874 */
    volatile U_SPIF_SUB_PKT2_H      SPIF_SUB_PKT2_H;      /* 1878 */
    volatile U_SPIF_SUB_PKT3_L      SPIF_SUB_PKT3_L;      /* 187C */
    volatile U_SPIF_SUB_PKT3_H      SPIF_SUB_PKT3_H;      /* 1880 */
    volatile U_MPEG_PKT_HEADER      MPEG_PKT_HEADER;      /* 1884 */
    volatile U_MPEG_SUB_PKT0_L      MPEG_SUB_PKT0_L;      /* 1888 */
    volatile U_MPEG_SUB_PKT0_H      MPEG_SUB_PKT0_H;      /* 188C */
    volatile U_MPEG_SUB_PKT1_L      MPEG_SUB_PKT1_L;      /* 1890 */
    volatile U_MPEG_SUB_PKT1_H      MPEG_SUB_PKT1_H;      /* 1894 */
    volatile U_MPEG_SUB_PKT2_L      MPEG_SUB_PKT2_L;      /* 1898 */
    volatile U_MPEG_SUB_PKT2_H      MPEG_SUB_PKT2_H;      /* 189C */
    volatile U_MPEG_SUB_PKT3_L      MPEG_SUB_PKT3_L;      /* 18A0 */
    volatile U_MPEG_SUB_PKT3_H      MPEG_SUB_PKT3_H;      /* 18A4 */
    volatile U_GEN_PKT_HEADER       GEN_PKT_HEADER;       /* 18A8 */
    volatile U_GEN_SUB_PKT0_L       GEN_SUB_PKT0_L;       /* 18AC */
    volatile U_GEN_SUB_PKT0_H       GEN_SUB_PKT0_H;       /* 18B0 */
    volatile U_GEN_SUB_PKT1_L       GEN_SUB_PKT1_L;       /* 18B4 */
    volatile U_GEN_SUB_PKT1_H       GEN_SUB_PKT1_H;       /* 18B8 */
    volatile U_GEN_SUB_PKT2_L       GEN_SUB_PKT2_L;       /* 18BC */
    volatile U_GEN_SUB_PKT2_H       GEN_SUB_PKT2_H;       /* 18C0 */
    volatile U_GEN_SUB_PKT3_L       GEN_SUB_PKT3_L;       /* 18C4 */
    volatile U_GEN_SUB_PKT3_H       GEN_SUB_PKT3_H;       /* 18C8 */
    volatile U_GEN2_PKT_HEADER      GEN2_PKT_HEADER;      /* 18CC */
    volatile U_GEN2_SUB_PKT0_L      GEN2_SUB_PKT0_L;      /* 18D0 */
    volatile U_GEN2_SUB_PKT0_H      GEN2_SUB_PKT0_H;      /* 18D4 */
    volatile U_GEN2_SUB_PKT1_L      GEN2_SUB_PKT1_L;      /* 18D8 */
    volatile U_GEN2_SUB_PKT1_H      GEN2_SUB_PKT1_H;      /* 18DC */
    volatile U_GEN2_SUB_PKT2_L      GEN2_SUB_PKT2_L;      /* 18E0 */
    volatile U_GEN2_SUB_PKT2_H      GEN2_SUB_PKT2_H;      /* 18E4 */
    volatile U_GEN2_SUB_PKT3_L      GEN2_SUB_PKT3_L;      /* 18E8 */
    volatile U_GEN2_SUB_PKT3_H      GEN2_SUB_PKT3_H;      /* 18EC */
    volatile U_GEN3_PKT_HEADER      GEN3_PKT_HEADER;      /* 18F0 */
    volatile U_GEN3_SUB_PKT0_L      GEN3_SUB_PKT0_L;      /* 18F4 */
    volatile U_GEN3_SUB_PKT0_H      GEN3_SUB_PKT0_H;      /* 18F8 */
    volatile U_GEN3_SUB_PKT1_L      GEN3_SUB_PKT1_L;      /* 18FC */
    volatile U_GEN3_SUB_PKT1_H      GEN3_SUB_PKT1_H;      /* 1900 */
    volatile U_GEN3_SUB_PKT2_L      GEN3_SUB_PKT2_L;      /* 1904 */
    volatile U_GEN3_SUB_PKT2_H      GEN3_SUB_PKT2_H;      /* 1908 */
    volatile U_GEN3_SUB_PKT3_L      GEN3_SUB_PKT3_L;      /* 190C */
    volatile U_GEN3_SUB_PKT3_H      GEN3_SUB_PKT3_H;      /* 1910 */
    volatile U_GEN4_PKT_HEADER      GEN4_PKT_HEADER;      /* 1914 */
    volatile U_GEN4_SUB_PKT0_L      GEN4_SUB_PKT0_L;      /* 1918 */
    volatile U_GEN4_SUB_PKT0_H      GEN4_SUB_PKT0_H;      /* 191C */
    volatile U_GEN4_SUB_PKT1_L      GEN4_SUB_PKT1_L;      /* 1920 */
    volatile U_GEN4_SUB_PKT1_H      GEN4_SUB_PKT1_H;      /* 1924 */
    volatile U_GEN4_SUB_PKT2_L      GEN4_SUB_PKT2_L;      /* 1928 */
    volatile U_GEN4_SUB_PKT2_H      GEN4_SUB_PKT2_H;      /* 192C */
    volatile U_GEN4_SUB_PKT3_L      GEN4_SUB_PKT3_L;      /* 1930 */
    volatile U_GEN4_SUB_PKT3_H      GEN4_SUB_PKT3_H;      /* 1934 */
    volatile U_GEN5_PKT_HEADER      GEN5_PKT_HEADER;      /* 1938 */
    volatile U_GEN5_SUB_PKT0_L      GEN5_SUB_PKT0_L;      /* 193C */
    volatile U_GEN5_SUB_PKT0_H      GEN5_SUB_PKT0_H;      /* 1940 */
    volatile U_GEN5_SUB_PKT1_L      GEN5_SUB_PKT1_L;      /* 1944 */
    volatile U_GEN5_SUB_PKT1_H      GEN5_SUB_PKT1_H;      /* 1948 */
    volatile U_GEN5_SUB_PKT2_L      GEN5_SUB_PKT2_L;      /* 194C */
    volatile U_GEN5_SUB_PKT2_H      GEN5_SUB_PKT2_H;      /* 1950 */
    volatile U_GEN5_SUB_PKT3_L      GEN5_SUB_PKT3_L;      /* 1954 */
    volatile U_GEN5_SUB_PKT3_H      GEN5_SUB_PKT3_H;      /* 1958 */
    volatile U_GAMUT_PKT_HEADER     GAMUT_PKT_HEADER;     /* 195C */
    volatile U_GAMUT_SUB_PKT0_L     GAMUT_SUB_PKT0_L;     /* 1960 */
    volatile U_GAMUT_SUB_PKT0_H     GAMUT_SUB_PKT0_H;     /* 1964 */
    volatile U_GAMUT_SUB_PKT1_L     GAMUT_SUB_PKT1_L;     /* 1968 */
    volatile U_GAMUT_SUB_PKT1_H     GAMUT_SUB_PKT1_H;     /* 196C */
    volatile U_GAMUT_SUB_PKT2_L     GAMUT_SUB_PKT2_L;     /* 1970 */
    volatile U_GAMUT_SUB_PKT2_H     GAMUT_SUB_PKT2_H;     /* 1974 */
    volatile U_GAMUT_SUB_PKT3_L     GAMUT_SUB_PKT3_L;     /* 1978 */
    volatile U_GAMUT_SUB_PKT3_H     GAMUT_SUB_PKT3_H;     /* 197C */
    volatile U_VSIF_PKT_HEADER      VSIF_PKT_HEADER;      /* 1980 */
    volatile U_VSIF_SUB_PKT0_L      VSIF_SUB_PKT0_L;      /* 1984 */
    volatile U_VSIF_SUB_PKT0_H      VSIF_SUB_PKT0_H;      /* 1988 */
    volatile U_VSIF_SUB_PKT1_L      VSIF_SUB_PKT1_L;      /* 198C */
    volatile U_VSIF_SUB_PKT1_H      VSIF_SUB_PKT1_H;      /* 1990 */
    volatile U_VSIF_SUB_PKT2_L      VSIF_SUB_PKT2_L;      /* 1994 */
    volatile U_VSIF_SUB_PKT2_H      VSIF_SUB_PKT2_H;      /* 1998 */
    volatile U_VSIF_SUB_PKT3_L      VSIF_SUB_PKT3_L;      /* 199C */
    volatile U_VSIF_SUB_PKT3_H      VSIF_SUB_PKT3_H;      /* 19A0 */
    volatile U_CEA_AVI_CFG          CEA_AVI_CFG;          /* 19A4 */
    volatile U_CEA_SPF_CFG          CEA_SPF_CFG;          /* 19A8 */
    volatile U_CEA_AUD_CFG          CEA_AUD_CFG;          /* 19AC */
    volatile U_CEA_MPEG_CFG         CEA_MPEG_CFG;         /* 19B0 */
    volatile U_CEA_GEN_CFG          CEA_GEN_CFG;          /* 19B4 */
    volatile U_CEA_CP_CFG           CEA_CP_CFG;           /* 19B8 */
    volatile U_CEA_GEN2_CFG         CEA_GEN2_CFG;         /* 19BC */
    volatile U_CEA_GEN3_CFG         CEA_GEN3_CFG;         /* 19C0 */
    volatile U_CEA_GEN4_CFG         CEA_GEN4_CFG;         /* 19C4 */
    volatile U_CEA_GEN5_CFG         CEA_GEN5_CFG;         /* 19C8 */
    volatile U_CEA_GAMUT_CFG        CEA_GAMUT_CFG;        /* 19CC */
    volatile U_CEA_VSIF_CFG         CEA_VSIF_CFG;         /* 19D0 */
    unsigned int                    Reserved_1[3];
    volatile U_CEA_PKT_STATE        CEA_PKT_STATE;        /* 19E0 */
    unsigned int                    Reserved_2[9];
    volatile U_AVMIXER_CONFIG       AVMIXER_CONFIG;       /* 1A08 */
    volatile U_CP_PKT_AVMUTE        CP_PKT_AVMUTE;        /* 1A0C */
    volatile U_VIDEO_BLANK_CFG      VIDEO_BLANK_CFG;      /* 1A10 */
    unsigned int                    Reserved_3[16];
    volatile U_TMDS_BIST_CTRL       TMDS_BIST_CTRL;       /* 1A54 */
    unsigned int                    Reserved_4[2];
    volatile U_HDMI_ENC_CTRL        HDMI_ENC_CTRL;        /* 1A60 */
    volatile U_ENC_CK_SHARP         ENC_CK_SHARP;         /* 1A64 */
} S_tx_hdmi_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_hdmi_reg */
int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init(HI_CHAR *pcAddr);

int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(unsigned int utmds_pack_mode);
/* Declare the AVI Infoframe Set */
int HDMI_AVI_PKT_HEADER_hbSet(unsigned int uhb0, unsigned int uhb1, unsigned int uhb2);

int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb0, unsigned int uavi_pkt0_pb1,
                                       unsigned int uavi_pkt0_pb2, unsigned int uavi_pkt0_pb3);

int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb4, unsigned int uavi_pkt0_pb5,
                                       unsigned int uavi_pkt0_pb6);

int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb0, unsigned int uavi_pkt1_pb1,
                                       unsigned int uavi_pkt1_pb2, unsigned int uavi_pkt1_pb3);

int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb4, unsigned int uavi_pkt1_pb5,
                                       unsigned int uavi_pkt1_pb6);

int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb0, unsigned int uavi_pkt2_pb1,
                                       unsigned int uavi_pkt2_pb2, unsigned int uavi_pkt2_pb3);

int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb4, unsigned int uavi_pkt2_pb5,
                                       unsigned int uavi_pkt2_pb6);

int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb0, unsigned int uavi_pkt3_pb1,
                                       unsigned int uavi_pkt3_pb2, unsigned int uavi_pkt3_pb3);

int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb4, unsigned int uavi_pkt3_pb5,
                                       unsigned int uavi_pkt3_pb6);
/* Declare the AVI Infoframe Get */
int HDMI_AVI_PKT_HEADER_hbGet(U_AVI_PKT_HEADER *u_avi_pkt_header);

int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbGet(U_AVI_SUB_PKT0_L *u_avi_sub_pkt0_l);

int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbGet(U_AVI_SUB_PKT0_H *u_avi_sub_pkt0_h);

int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbGet(U_AVI_SUB_PKT1_L *u_avi_sub_pkt1_l);

int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbGet(U_AVI_SUB_PKT1_H *u_avi_sub_pkt1_h);

int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbGet(U_AVI_SUB_PKT2_L *u_avi_sub_pkt2_l);

int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbGet(U_AVI_SUB_PKT2_H *u_avi_sub_pkt2_h);

int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbGet(U_AVI_SUB_PKT3_L *u_avi_sub_pkt3_l);

int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbGet(U_AVI_SUB_PKT3_H *u_avi_sub_pkt3_h);

/* Declare the Audio Infoframe Get */
int HDMI_AIF_PKT_HEADER_hbGet(U_AIF_PKT_HEADER *u_aif_pkt_header);

int HDMI_AIF_SUB_PKT0_L_aif_pkt0_pbGet(U_AIF_SUB_PKT0_L *u_aif_sub_pkt0_l);

int HDMI_AIF_SUB_PKT0_H_aif_pkt0_pbGet(U_AIF_SUB_PKT0_H *u_aif_sub_pkt0_h);

int HDMI_AIF_SUB_PKT1_L_aif_pkt1_pbGet(U_AIF_SUB_PKT1_L *u_aif_sub_pkt1_l);

int HDMI_AIF_SUB_PKT1_H_aif_pkt1_pbGet(U_AIF_SUB_PKT1_H *u_aif_sub_pkt1_h);

int HDMI_AIF_SUB_PKT2_L_aif_pkt2_pbGet(U_AIF_SUB_PKT2_L *u_aif_sub_pkt2_l);

int HDMI_AIF_SUB_PKT2_H_aif_pkt2_pbGet(U_AIF_SUB_PKT2_H *u_aif_sub_pkt2_h);

int HDMI_AIF_SUB_PKT3_L_aif_pkt3_pbGet(U_AIF_SUB_PKT3_L *u_aif_sub_pkt3_l);

int HDMI_AIF_SUB_PKT3_H_aif_pkt3_pbGet(U_AIF_SUB_PKT3_H *u_aif_sub_pkt3_h);

/* Declare the gen Infoframe Set */
int HDMI_GEN_PKT_HEADER_hbSet(unsigned int uhb0, unsigned int uhb1, unsigned int uhb2);
int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb0, unsigned int ugen_pkt0_pb1,
                                       unsigned int ugen_pkt0_pb2, unsigned int ugen_pkt0_pb3);

int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb4, unsigned int ugen_pkt0_pb5,
                                       unsigned int ugen_pkt0_pb6);

int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb0, unsigned int ugen_pkt1_pb1,
                                       unsigned int ugen_pkt1_pb2, unsigned int ugen_pkt1_pb3);

int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb4, unsigned int ugen_pkt1_pb5,
                                       unsigned int ugen_pkt1_pb6);

int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb0, unsigned int ugen_pkt2_pb1,
                                       unsigned int ugen_pkt2_pb2, unsigned int ugen_pkt2_pb3);

int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb4, unsigned int ugen_pkt2_pb5,
                                       unsigned int ugen_pkt2_pb6);

int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb0, unsigned int ugen_pkt3_pb1,
                                       unsigned int ugen_pkt3_pb2, unsigned int ugen_pkt3_pb3);

int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb4, unsigned int ugen_pkt3_pb5,
                                       unsigned int ugen_pkt3_pb6);
/* Declare the gen Infoframe Get */
int HDMI_GEN_PKT_HEADER_hbGet(U_GEN_PKT_HEADER *u_gen_pkt_header);

int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbGet(U_GEN_SUB_PKT0_L *u_gen_sub_pkt0_l);

int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbGet(U_GEN_SUB_PKT0_H *u_gen_sub_pkt0_h);

int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbGet(U_GEN_SUB_PKT1_L *u_gen_sub_pkt1_l);

int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbGet(U_GEN_SUB_PKT1_H *u_gen_sub_pkt1_h);

int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbGet(U_GEN_SUB_PKT2_L *u_gen_sub_pkt2_l);

int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbGet(U_GEN_SUB_PKT2_H *u_gen_sub_pkt2_h);

int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbGet(U_GEN_SUB_PKT3_L *u_gen_sub_pkt3_l);

int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbGet(U_GEN_SUB_PKT3_H *u_gen_sub_pkt3_h);

/* Declare the gamut Infoframe Get */
int HDMI_GAMUT_PKT_HEADER_hbGet(U_GAMUT_PKT_HEADER *u_gamut_pkt_header);

int HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_L *u_gamut_sub_pkt0_l);

int HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_H *u_gamut_sub_pkt0_h);

int HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_L *u_gamut_sub_pkt1_l);

int HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_H *u_gamut_sub_pkt1_h);

int HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_L *u_gamut_sub_pkt2_l);

int HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_H *u_gamut_sub_pkt2_h);

int HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_L *u_gamut_sub_pkt3_l);

int HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_H *u_gamut_sub_pkt3_h);
/* Declare the VSIF Infoframe Set */
int HDMI_VSIF_PKT_HEADER_hbSet(unsigned int uhb0, unsigned int uhb1, unsigned int uhb2);

int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb0, unsigned int uvsif_pkt0_pb1,
                                         unsigned int uvsif_pkt0_pb2, unsigned int uvsif_pkt0_pb3);

int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb4, unsigned int uvsif_pkt0_pb5,
                                         unsigned int uvsif_pkt0_pb6);

int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb0, unsigned int uvsif_pkt1_pb1,
                                         unsigned int uvsif_pkt1_pb2, unsigned int uvsif_pkt1_pb3);

int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb4, unsigned int uvsif_pkt1_pb5,
                                         unsigned int uvsif_pkt1_pb6);

int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb0, unsigned int uvsif_pkt2_pb1,
                                         unsigned int uvsif_pkt2_pb2, unsigned int uvsif_pkt2_pb3);

int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb4, unsigned int uvsif_pkt2_pb5,
                                         unsigned int uvsif_pkt2_pb6);

int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb0, unsigned int uvsif_pkt3_pb1,
                                         unsigned int uvsif_pkt3_pb2, unsigned int uvsif_pkt3_pb3);

int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb4, unsigned int uvsif_pkt3_pb5,
                                         unsigned int uvsif_pkt3_pb6);
/* Declare the VSIF Infoframe Get */
int HDMI_VSIF_PKT_HEADER_hbGet(U_VSIF_PKT_HEADER *u_vsif_pkt_header);

int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_L *u_vsif_sub_pkt0_l);

int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_H *u_vsif_sub_pkt0_h);

int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_L *u_vsif_sub_pkt1_l);

int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_H *u_vsif_sub_pkt1_h);

int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_L *u_vsif_sub_pkt2_l);

int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_H *u_vsif_sub_pkt2_h);


int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_L *u_vsif_sub_pkt3_l);

int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_H *u_vsif_sub_pkt3_h);

/* Declare the Infoframe Enable */
int HDMI_CEA_AVI_CFG_cea_avi_enSet(unsigned int ucea_avi_en);

int HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(unsigned int ucea_avi_rpt_en);

int HDMI_CEA_AUD_CFG_cea_aud_enSet(unsigned int ucea_aud_en);

int HDMI_CEA_AUD_CFG_cea_aud_rpt_enSet(unsigned int ucea_aud_rpt_en);

int HDMI_CEA_GEN_CFG_cea_gen_enSet(unsigned int ucea_gen_en);

int HDMI_CEA_GEN_CFG_cea_gen_rpt_enSet(unsigned int ucea_gen_rpt_en);

int HDMI_CEA_CP_CFG_cea_cp_enSet(unsigned int ucea_cp_en);

int HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(unsigned int ucea_cp_rpt_cnt);

int HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(unsigned int ucea_cp_rpt_en);

int HDMI_CEA_GAMUT_CFG_cea_gamut_enSet(unsigned int ucea_gamut_en);

int HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enSet(unsigned int ucea_gamut_rpt_en);

int HDMI_CEA_VSIF_CFG_cea_vsif_enSet(unsigned int ucea_vsif_en);

int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(unsigned int ucea_vsif_rpt_en);

int HDMI_AVMIXER_CONFIG_hdmi_modeSet(unsigned int uhdmi_mode);

int HDMI_AVMIXER_CONFIG_dc_pkt_enSet(unsigned int udc_pkt_en);

int HDMI_AVMIXER_CONFIG_null_pkt_enSet(unsigned int unull_pkt_en);

int HDMI_AVMIXER_CONFIG_hdmi_dvi_selSet(unsigned int uhdmi_dvi_sel);

int HDMI_AVMIXER_CONFIG_eess_mode_enSet(unsigned int ueess_mode_en);

int HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(unsigned int ucp_set_avmute);

int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(unsigned int ucp_clr_avmute);

int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(unsigned int uenc_hdmi2_on);

int HDMI_HDMI_ENC_CTRL_enc_scr_onSet(unsigned int uenc_scr_on);

int HDMI_HDMI_ENC_CTRL_enc_bypassSet(unsigned int uenc_bypass);

int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet(HI_VOID);

int HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableGet(HI_VOID);

int HDMI_CEA_AVI_CFG_cea_avi_enGet(HI_VOID);

int HDMI_CEA_AUD_CFG_cea_aud_enGet(HI_VOID);

int HDMI_CEA_GEN_CFG_cea_gen_enGet(HI_VOID);

int HDMI_CEA_CP_CFG_cea_cp_rpt_enGet(HI_VOID);

int HDMI_CEA_GAMUT_CFG_cea_gamut_enGet(HI_VOID);

int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enGet(HI_VOID);

int HDMI_AVMIXER_CONFIG_hdmi_modeGet(HI_VOID);

int HDMI_AVMIXER_CONFIG_dc_pkt_enGet(HI_VOID);

int HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet(HI_VOID);

int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onGet(HI_VOID);

int HDMI_HDMI_ENC_CTRL_enc_scr_onGet(HI_VOID);

int HDMI_AVMIXER_CONFIG_null_pkt_enGet(HI_VOID);

#endif /* __TX_HDMI_REG_C_UNION_DEFINE_H__ */

