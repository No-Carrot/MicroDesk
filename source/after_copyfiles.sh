#! /bin/sh

HOMEPATH="/home/linux/emb"
BUILDROOT="/home/linux/emb/buildroot"
LINUXSHARE="/mnt/hgfs/linuxshare/UUU/file"
IMAGES=/home/linux/emb/buildroot/output/images
echo "HOMEPATH=$HOMEPATH"

# copy uboot zImage dtb  to  buildroot 

cp -v $IMAGES/rootfs.tar.bz2  $LINUXSHARE









