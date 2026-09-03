#!/bin/sh
insmod /lib/modules/$(uname -r)/kernel/net/wireless/cfg80211.ko
insmod /lib/modules/$(uname -r)/extra/8188eu.ko
ifconfig wlan0 up 
wpa_supplicant -Dnl80211 -c /etc/wpa_supplicant.conf -i wlan0 &

# 4G  
echo -e 'ATE0\r\n' > /dev/ttyUSB2
echo -e 'AT+MDIALUP=1,1\r\n' > /dev/ttyUSB2
cat /dev/ttyUSB2 &
echo -e 'AT+MDIALUP?\r\n' > /dev/ttyUSB2


