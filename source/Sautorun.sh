#! /bin/sh 

export TSLIB_FBDEVICE=/dev/fb0                        		#framebuffer设备文件
export TSLIB_CONSOLEDEVICE=none                             # 设置none即可

export TSLIB_TSDEVICE=/dev/input/event1                     #触摸屏设备文件
export TSLIB_CALIBFILE=/etc/pointercal                		#指定触摸屏校准文件pintercal的存放位置
export TSLIB_CONFFILE=/etc/ts.conf             				#tslib模块配置文件
export TSLIB_PLUGINDIR=/usr/lib/ts/                			#tslib插件库目录

export QT_QPA_EGLFS_TSLIB=1    
export QT_QPA_FB_TSLIB=1
export QT_PLUGIN_PATH=/usr/lib/qt/plugins
export QT_IM_MODULE=im

#export QT_DEBUG_PLUGINS=1
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/event1,evdevkeyboard
export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/event2



#/root/Key&
echo 0 > /sys/class/graphics/fbcon/cursor_blink
cd /root/desktop/MicroDesktop/ && ./Desktop&
