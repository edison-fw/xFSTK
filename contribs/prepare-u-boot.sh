#!/bin/sh

#
# Required: xz, crc32, xxd, xfst-dldr-solo, dfu-util
#

# Version for stable edison-v2016.11 and edison-v2017.01
#dd if=u-boot.bin of=u-boot-4k.bin bs=4k seek=1 && truncate -s %4096 u-boot-4k.bin

# OSIP header (0x000000) for Intel Edison
base64 -d << EOF | xz -d > u-boot-edison.img
/Td6WFoAAATm1rRGAgAhARYAAAB0L+Wj4AH/ADJdABITxmI/dj2HEN524/dYfdgu
VSVrkQEE6XSgsZnwdRltLRkU6GTigpK5eAw3CQAaRzcAAAAAtBe6DKrPzUcAAU6A
BAAAAASYg8ixxGf7AgAAAAAEWVo=
EOF

# OSIP header (0x000000) for Dell Venue 7 3740
base64 -d << EOF | xz -d > u-boot-venue7-3740.img
/Td6WFoAAATm1rRGAgAhARYAAAB0L+Wj4AH/ACVdABITxmI/dj2mun+hQ6HTOrxiAZIam5M83oke
XAr75ok9v2hUsAAAAAAA/BHNkI50EiMAAUGABAAAANEq1TmxxGf7AgAAAAAEWVo=
EOF

# U-Boot binary (0x000200), it has an additional offset 0x001000
dd if=u-boot.bin of=u-boot-edison.img bs=512 seek=1
dd if=u-boot.bin of=u-boot-venue7-3740.img bs=512 seek=1

# Binary size in sectors
size=$(stat -c "%s" u-boot.bin)
sectors=$(printf "%08x" $(((size + 512 - 1) / 512)) | grep -o .. | tac)
echo $sectors | xxd -r -p | dd of=u-boot-venue7-3740.img bs=1 seek=48 conv=notrunc

# XOR checksum of the OSIP and OSII headers
csum=0
for b in $(seq 0x00 $((0x20 + 0x18 - 1))); do
	byte=$(dd if=u-boot-venue7-3740.img bs=1 skip=$b count=1 2>/dev/null)
	dec=$(printf "%d" "'$byte")
	csum=$((csum ^ dec))
done
printf "%02x" $csum | xxd -r -p | dd of=u-boot-venue7-3740.img bs=1 seek=7 conv=notrunc

# Pad the rest of the file to be LBA aligned
truncate -s %512 u-boot-venue7-3740.img

# Get original evironment as a text file
#strings edison-blankcdc.bin > edison-environment.txt

# Prepare U-Boot environment image from a text file (64k, 0xff)
dd if=/dev/zero ibs=1k count=64 | tr '\000' '\377' > edison-environment.bin

# Start byte (0x01)
printf '\1' | dd of=edison-environment.bin bs=1 seek=4 conv=notrunc
# Body (NULL-terminated strings)
tr '\n' '\0' < edison-environment.txt | dd of=edison-environment.bin bs=1 seek=5 conv=notrunc
# End byte (0x00)
printf '\0' | dd of=edison-environment.bin bs=1 seek=$((5 + $(wc -c < edison-environment.txt))) conv=notrunc
# CRC32
crc32=$(dd if=edison-environment.bin skip=5 bs=1 | crc32 /dev/stdin | grep -o .. | tac)
echo $crc32 | xxd -r -p | dd of=edison-environment.bin bs=1 seek=0 conv=notrunc

# U-Boot environment image (0x200200)
dd if=edison-environment.bin of=u-boot-edison.img bs=512 seek=$((4096+1))

# Copy of U-Boot environment image (0x500200)
dd if=edison-environment.bin of=u-boot-edison.img bs=512 seek=$((10240+1))

# Flash an image (recovery):
#xfstk-dldr-solo --gpflags 0x80000007 --osimage u-boot-edison.img --osdnx edison_dnx_osr.bin

# DFU:
#dfu-util -v -d 8087:0a99 --alt u-boot0 -D u-boot.bin
#dfu-util -v -d 8087:0a99 --alt u-boot-env0 -D edison-environment.bin
#dfu-util -v -d 8087:0a99 --alt u-boot-env1 -D edison-environment.bin
