# ============================================================
# core 库 - 通用接口功能实例类
# ============================================================

TEMPLATE = lib
TARGET = core

QT += core serialport

# 包含公共配置
include($$PWD/../common.pri)

# 导出符号定义
DEFINES += CORE_LIBRARY_EXPORT

# 输出目录
DESTDIR = $$LIB_OUT_DIR

# 包含 interface 子目录的源文件
include($$PWD/interface/interface.pri)

HEADERS += \
    basefactory.h \
    basepath.h \
    conf.h \
    processhandler.h \
    resourceloader.h \
    serialport.h \
    settings.h \
    terminal.h \
    threadexec.h \
    ustring.h

SOURCES += \
    basefactory.cpp \
    basepath.cpp \
    conf.cpp \
    processhandler.cpp \
    resourceloader.cpp \
    serialport.cpp \
    settings.cpp \
    terminal.cpp
