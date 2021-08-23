/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute  it and/or modify it
* under  the terms of  the GNU General Public License as published by the
* Free Software Foundation;  either version 2 of the  License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "cipher_adapt.h"

HI_S32 hi_drv_compat_init(void)
{
    HI_S32 ret = HI_FAILURE;

    ret = DRV_KLAD_Init();
    if (HI_SUCCESS != ret)
    {
         return ret;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_compat_deinit(void)
{
    DRV_KLAD_DeInit();

    return HI_SUCCESS;
}

