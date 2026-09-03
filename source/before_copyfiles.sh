#! /bin/sh

HOMEPATH="/home/linux/emb"
BUILDROOT="/home/linux/emb/buildroot"
LINUXSHARE="/mnt/hgfs/linuxshare/UUU/file"
OUTPUT=/home/linux/emb/buildroot/output


# copy uboot zImage dtb  to  buildroot 

cp -v $BUILDROOT/output/build/linux-custom/arch/arm/boot/dts/myd-y6ull-gpmi-weim.dtb $OUTPUT/images
cp -v $OUTPUT/images/u-boot-dtb.imx $LINUXSHARE
cp -v $OUTPUT/images/zImage  $LINUXSHARE
cp -v $OUTPUT/images/myd-y6ull-gpmi-weim.dtb  $LINUXSHARE

# 复制字库到文件系统rootfs 
if [ ! -e $HOMEPATH/buildroot/output/target/usr/lib/fonts ] 
then 
    mkdir -p $HOMEPATH/buildroot/output/target/usr/lib/fonts
    cp -v  $HOMEPATH/source/NotoSansCJK-Regular.ttc  $OUTPUT/target/usr/lib/fonts
fi 

# copy sdma-imx6q.bin to rootfs 
if [ ! -e $OUTPUT/target/lib/firmware/imx/sdma ] 
then 
    mkdir -p $OUTPUT/target/lib/firmware/imx/sdma
fi 
cp -va $HOMEPATH/source/sdma-imx6q.bin $OUTPUT/target/lib/firmware/imx/sdma

# copy S30wifi.sh to rootfs 
cp -va $HOMEPATH/source/S30wifi.sh $OUTPUT/target/etc/init.d/

# copy wpa_supplicant.conf to rootfs 
cp -va $HOMEPATH/source/wpa_supplicant.conf $OUTPUT/target/etc/

# copy Sautostart.sh  to  rootfs
cp -v $HOMEPATH/source/Sautorun.sh $OUTPUT/target/etc/init.d/

# copy inittab to /etc
cp -v $HOMEPATH/source/inittab $OUTPUT/target/etc/


# copy libIM.so to rootfs  
if [ ! -e $OUTPUT/target/usr/lib/qt/plugins/platforminputcontexts ] 
then 
    mkdir -p $OUTPUT/target/usr/lib/qt/plugins/platforminputcontexts    
fi 
cp -v $HOMEPATH/14-desktop/desktop/im/plugins/platforminputcontexts/libIM.so  \
      $OUTPUT/target/usr/lib/qt/plugins/platforminputcontexts/

# copy libcore.so to rootfs 
cp -v $HOMEPATH/14-desktop/desktop/im/lib/libcore.so* $OUTPUT/target/usr/lib/
cp -v $HOMEPATH/14-desktop/desktop/im/lib/libui.so* $OUTPUT/target/usr/lib/

# copy pinyin data to rootfs 
if [ ! -e $OUTPUT/target/usr/share/pinyin/3rdparty/pinyin/data ] 
then 
    mkdir -p $OUTPUT/target/usr/share/pinyin/3rdparty/pinyin/data
fi 
cp -v $HOMEPATH/14-desktop/im/imvirtualkeyboard/pinyininputmethod/pinyin/data/dict_pinyin.dat \
      $OUTPUT/target/usr/share/pinyin/3rdparty/pinyin/data/

#  copy  desktop to imx6 
cp -va $HOMEPATH/14-desktop/desktop/  $OUTPUT/target/root




