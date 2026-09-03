include($$PWD/../../buildout.pri)

QT -=gui
TEMPLATE = lib
TARGET = common

HEADERS += \
    basefactory.h \
    processhandler.h \
    serialport.h \
    terminal.h

include($$PWD/Allwinner/allwinner.pri)
#include($$PWD/NXP/nxp.pri)
include($$PWD/interface/interface.pri)

INCLUDEPATH +=$$PWD

CONFIG += c++11

include($$PWD/../../install.pri)

LIBS +=-lcore

SOURCES += \
    processhandler.cpp \
    serialport.cpp \
    terminal.cpp
