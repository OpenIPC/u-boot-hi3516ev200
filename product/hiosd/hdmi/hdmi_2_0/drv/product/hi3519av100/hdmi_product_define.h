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
#ifndef __HDMI_PRODUCT_DEFINE_H__
#define __HDMI_PRODUCT_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "ubi_uboot.h"
#include "malloc.h"
#include "boot_hdmi_intf.h"

#include "hi_type.h"

/******************************************************************************/
/* Macro type for hdmi module                                                 */
/******************************************************************************/

#define HDMI_CHIP_HI3519AV100       0x3519a100

#define HDMI_CTRL_BASE_ADDR         0x04740000
#define HDMI_ADDR_BASE_IO_CFG       0x047b8000
#define HDMI_ADDR_CRG               0x04510000      /* CRG reg */
#define BASE_ADDR_tx_dphy           0x04760000      /* hdmi tx phy reg */
/* No mux IO */
#define HDMI_COLOR_BAR_MASK         0x70000000
#define HDMI_COLOR_BAR_UPDATE_MASK  0x00000001
#define HDMI_COLOR_BAR_BASE         0x0470d000      /* VDP */

/* HDMI REG */
#define HDMI_TX_BASE_ADDR           0x04740000
#define HDMI_TX_REG_LEN             (0x30000)

#define HDMI_TX_BASE_ADDR_CTRL      0x0000
#define HDMI_TX_BASE_ADDR_AIDEO     0x0800
#define HDMI_TX_BASE_ADDR_AUDIO     0x1000
#define HDMI_TX_BASE_ADDR_HDMITX    0x1800
#define HDMI_TX_BASE_ADDR_HDCP1X    0x2000
#define HDMI_TX_BASE_ADDR_AON       0x4000
#define HDMI_TX_BASE_ADDR_CEC       0x4800
#define HDMI_TX_BASE_ADDR_MCU_PRAM  0x10000
#define HDMI_TX_BASE_ADDR_MCU_DRAM  0x18000
#define HDMI_TX_BASE_ADDR_MCU       0x1c000
#define HDMI_TX_BASE_ADDR_DPHY      0x20000

/* HDMI PHY */
#define HDMI_TX_PHY_ADDR            (HDMI_TX_BASE_ADDR + HDMI_TX_BASE_ADDR_DPHY)

#define BASE_ADDR_tx_ctrl_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_CTRL)
#define BASE_ADDR_video_path_reg    (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AIDEO)
#define BASE_ADDR_audio_path_reg    (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AUDIO)
#define BASE_ADDR_tx_hdmi_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_HDMITX)
#define BASE_ADDR_tx_hdcp_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_HDCP1X)
#define BASE_ADDR_tx_aon_reg        (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AON)
#define BASE_ADDR_tx_cec_reg        (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_CEC)
#define BASE_ADDR_mcu_cpu_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU)
#define BASE_ADDR_mcu_pram          (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU_PRAM)
#define BASE_ADDR_mcu_dram          (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU_DRAM)

/* pm disable */
#ifdef HI_ADVCA_FUNCTION_RELEASE
/* PROC disable */
#define  CONFIG_HDMI_PROC_DISABLE
/* LOG disable */
#define  CONFIG_HDMI_DEBUG_DISABLE
#endif

#define HDMI_MSLEEP(ms)                 mdelay((ms))
#define HDMI_MDELAY(ms)                 mdelay((ms))
#define HDMI_UDELAY(us)                 udelay((us))
#define HDMI_SNPRINTF                   snprintf
#define HDMI_MEMSET                     memset
#define HDMI_MEMCPY                     memcpy
#define HDMI_MEMCMP                     memcmp
#define HDMI_VMALLOC(size)              vmalloc(size)
#define HDMI_KMALLOC(size)              kmalloc(size, GFP_KERNEL)
#define HDMI_KFREE(viraddr)             kfree(viraddr)
#define HDMI_VFREE(viraddr)             vfree(viraddr)

#define HDMI_IO_MAP(phys_addr, len)     (phys_addr)
#define HDMI_IO_UNMAP(virt_addr)

#define HDMI_DO_DIVU64(dividend, divisor)        div_u64(dividend, divisor)
#define HDMI_DO_DIVS64(dividend, divisor)        div_s64(dividend, divisor)
#define HDMI_DIVU64_REM(dividend, divisor, rem)  div_u64_rem(dividend, divisor, rem)
#define HDMI_DIVS64_REM(dividend, divisor, rem)  div_s64_rem(dividend, divisor, rem)

typedef struct {
    HI_U32 u32SscBypassDiv;
    HI_U32 u32TmdsClkDiv;
}HDMI_CRG_CFG_S;

/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/
HI_S32 HDMITXRegWrite(HI_U32 *pu32RegAddr, HI_U32 u32Value);
HI_U32 HDMITXRegRead(HI_U32 *pu32RegAddr);
HI_S32 HDMIRegWriteU32(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_U32 HDMIRegReadU32 (HI_U32 u32RegAddr);
HI_S32 HDMITXRegWrite64(HI_U32 *pu32RegAddr, HI_U32 u32Value);
HI_U32 HDMITXRegRead64(HI_U32 *pu32RegAddr);

HI_VOID DRV_HDMI_ProdIoCfgSet(HI_VOID);
HI_VOID DRV_HDMI_ProdCrgAllResetSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgGateSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgDivSet(HDMI_CRG_CFG_S *pstCrgCfg);
HI_VOID DRV_HDMI_ProdCrgPhyResetSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgPhyResetGet(HI_BOOL *pbEnable);
HI_VOID DRV_HDMI_ProdCrgInit(HI_VOID);
HI_VOID DRV_HDMI_ProdCrgDeInit(HI_VOID);
HI_VOID DRV_HDMI_HardwareReset(HI_VOID);
HI_VOID DRV_HDMI_LowPowerSet(HI_BOOL bEnable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_PRODUCT_DEFINE_H__ */


