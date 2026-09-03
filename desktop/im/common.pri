# ============================================================
# inputmethod 公共配置文件
# 所有子项目 include 此文件即可
# ============================================================

# 工程根目录
INPUTMETHOD_ROOT = $$PWD

# 输出根目录（与其他桌面应用保持一致）
OUTPUT_ROOT = $$PWD/../desktop/im

# C++11 标准
CONFIG += c++11

# 根据构建类型设置输出目录
CONFIG(release, debug|release) {
    BUILD_TYPE = release
} else {
    BUILD_TYPE = debug
}

# 库输出目录
LIB_OUT_DIR = $$OUTPUT_ROOT/lib

# 插件输出目录
PLUGIN_OUT_DIR = $$OUTPUT_ROOT/plugins

# 可执行文件输出目录
BIN_OUT_DIR = $$OUTPUT_ROOT/bin

# 库搜索路径
LIBS += -L$$LIB_OUT_DIR
DEPENDPATH += $$LIB_OUT_DIR

# 运行时库搜索路径（rpath）
QMAKE_LFLAGS += -Wl,-rpath,\'$$ORIGIN\' -Wl,-rpath,\'$$ORIGIN/lib\' -Wl,-rpath,\'$$ORIGIN/../lib\'

# core 库的头文件路径
CORE_INCLUDE = $$INPUTMETHOD_ROOT/core $$INPUTMETHOD_ROOT/core/interface

# ui 库的头文件路径
UI_INCLUDE = $$INPUTMETHOD_ROOT/ui
