SOURCES += \
    $$PWD/share/dictbuilder.cpp \
    $$PWD/share/dictlist.cpp \
    $$PWD/share/dicttrie.cpp \
    $$PWD/share/lpicache.cpp \
    $$PWD/share/matrixsearch.cpp \
    $$PWD/share/mystdlib.cpp \
    $$PWD/share/ngram.cpp \
    $$PWD/share/pinyinime.cpp \
    $$PWD/share/searchutility.cpp \
    $$PWD/share/spellingtable.cpp \
    $$PWD/share/spellingtrie.cpp \
    $$PWD/share/splparser.cpp \
    $$PWD/share/sync.cpp \
    $$PWD/share/userdict.cpp \
    $$PWD/share/utf16char.cpp \
    $$PWD/share/utf16reader.cpp

HEADERS += \
    $$PWD/include/atomdictbase.h \
    $$PWD/include/dictbuilder.h \
    $$PWD/include/dictdef.h \
    $$PWD/include/dictlist.h \
    $$PWD/include/dicttrie.h \
    $$PWD/include/lpicache.h \
    $$PWD/include/matrixsearch.h \
    $$PWD/include/mystdlib.h \
    $$PWD/include/ngram.h \
    $$PWD/include/pinyinime.h \
    $$PWD/include/searchutility.h \
    $$PWD/include/spellingtable.h \
    $$PWD/include/spellingtrie.h \
    $$PWD/include/splparser.h \
    $$PWD/include/sync.h \
    $$PWD/include/userdict.h \
    $$PWD/include/utf16char.h \
    $$PWD/include/utf16reader.h

#OTHER_FILES +=\
#    data/rawdict_utf16_65105_freq.txt \
#    data/valid_utf16.txt

INCLUDEPATH +=$$PWD/include

#include($$PWD/../../lib.pri)
