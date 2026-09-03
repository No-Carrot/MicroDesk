# ============================================================
# imvirtualkeyboard - Qt5 中英文软键盘插件
# ============================================================
#
# 使用说明：
# 1. 拷贝 dict_pinyin.dat 到 /usr/share/pinyin/3rdparty/pinyin/data/
# 2. 拷贝 libIM.so 到 $QT_PLUGIN_PATH/platforminputcontexts/
# 3. 设置环境变量 export QT_IM_MODULE=im
#
# 显示模式：
# - 默认：根据输入框位置自动确定显示位置
# - 底部固定：qApp->setProperty("bottom", true)
#
# ============================================================

TEMPLATE = lib
TARGET = IM
CONFIG += plugin c++11

QT += gui gui-private core-private widgets

# 包含公共配置
include($$PWD/../common.pri)

# 插件输出目录
DESTDIR = $$PLUGIN_OUT_DIR/platforminputcontexts

HEADERS += \
    abstractinputmethod.h \
    iminputcontext.h \
    implatforminputcontext.h \
    inputmethodmanager.h \
    keyboardwidget.h

SOURCES += \
    abstractinputmethod.cpp \
    iminputcontext.cpp \
    implatforminputcontext.cpp \
    inputmethodmanager.cpp \
    keyboardwidget.cpp \
    main.cpp

OTHER_FILES += im.json

# 包含拼音输入法
include($$PWD/pinyininputmethod/pinyin/pinyin.pri)
include($$PWD/pinyininputmethod/pinyin2/pinyin2.pri)

# 安装配置
target.path = $$[QT_INSTALL_PLUGINS]/platforminputcontexts
INSTALLS += target

imdata.path = $$[QT_SYSROOT]/usr/share/pinyin/3rdparty/pinyin
imdata.files = $$PWD/pinyininputmethod/pinyin/data
INSTALLS += imdata
