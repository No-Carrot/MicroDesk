app_src_path=$$PWD
APP_OUT_DIR=$$app_src_path/../../elf_qt_out
CONFIG(debug, debug|release){
LIBS *= -L$$APP_OUT_DIR/debug/lib
DEPENDPATH *=$$APP_OUT_DIR/debug/lib
DESTDIR = $$APP_OUT_DIR/debug/lib
}else{
LIBS *= -L$$APP_OUT_DIR/release/lib
DEPENDPATH *=$$APP_OUT_DIR/release/lib
DESTDIR = $$APP_OUT_DIR/release/lib
}

#QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\' -Wl,-rpath,\'\$$ORIGIN\'/lib -Wl,-rpath,\'\$$ORIGIN\'/../lib'
