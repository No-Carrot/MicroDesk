# ============================================================
# ui 库 - 圆角主窗口、通用标题栏、消息弹窗等
# ============================================================

TEMPLATE = lib
TARGET = ui

QT += core widgets

# 包含公共配置
include($$PWD/../common.pri)

# 导出符号定义
DEFINES += UI_LIBRARY_EXPORT

# 输出目录
DESTDIR = $$LIB_OUT_DIR

# 依赖 core 库
LIBS += -lcore
INCLUDEPATH += $$CORE_INCLUDE

HEADERS += \
    appwindow.h \
    eventhandler.h \
    messagebox.h \
    titlebar.h

SOURCES += \
    appwindow.cpp \
    eventhandler.cpp \
    messagebox.cpp \
    titlebar.cpp

# 可选：DWKEYBOARD 键盘支持
DWKEYBOARD {
    message("use DWKEYBOARD..." + $$DEFINES)
    QT += core-private virtualkeyboard
    LIBS += -lvirtualkeyboard
}
