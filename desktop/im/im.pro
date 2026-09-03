# ============================================================
# im (Input Method) 主工程 - 中英文软键盘
# ============================================================
#
# 工程结构：
# ├── common.pri          - 公共配置
# ├── core/               - 通用接口功能库
# ├── ui/                 - UI 组件库
# └── imvirtualkeyboard/  - 软键盘插件
#
# 输出目录：../desktop/im/
# ├── lib/                - libcore.so, libui.so
# ├── plugins/            - libIM.so
# └── bin/                - 可执行文件
#
# ============================================================

TEMPLATE = subdirs

# 按顺序编译（ui 依赖 core）
CONFIG += ordered

SUBDIRS += \
    core \
    ui \
    imvirtualkeyboard
