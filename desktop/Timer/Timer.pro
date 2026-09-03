QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc


RC_ICONS = images/logo.ico

TRANSLATIONS += \
    en_US.ts

# ========== 输出配置 ==========
# 可执行文件名称
TARGET = Timer

# 输出目录：项目目录的兄弟目录 desktop/MicroDesktop/
# $$PWD 表示 .pro 文件所在目录
DESTDIR = $$PWD/../desktop/Timer

