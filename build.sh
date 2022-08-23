#!/bin/sh -e

export ARCH=arm
export CROSS_COMPILE=${CROSS_COMPILE:-arm-himix100-linux-}

OUTPUTDIR="${HOME}/src/hisi/uboot"
SOCS="hi3516ev200 hi3516ev300 hi3518ev300 hi3516dv200"

for soc in ${SOCS};do

make clean
cp config-${soc} .config
cp reg_info_${soc}.bin .reg
make -j8 #KCFLAGS=-DCONFIG_XM_COMPATIBLE=1

[ ! -f tools/hi_gzip/bin/gzip ] && make -C tools/hi_gzip || cp tools/hi_gzip/bin/gzip arch/arm/cpu/armv7/${soc}/hw_compressed/ -rf

make u-boot-z.bin

cp u-boot-${soc}.bin ${OUTPUTDIR}/u-boot-${soc}-universal.bin
cp u-boot-${soc}.bin /srv/tftp/u-boot-${soc}-universal.bin

done
