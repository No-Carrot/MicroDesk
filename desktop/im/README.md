# Qt5 中英文软键盘 (im) 部署指南

## 简介

本项目是一个 Qt5 平台输入法插件，支持中英文输入，适用于嵌入式 Linux 设备的触摸屏输入场景。

## 工程结构

```
im/
├── im.pro              # 主工程文件
├── common.pri          # 公共配置
├── core/               # 核心功能库 (libcore.so)
├── ui/                 # UI 组件库 (libui.so)
└── imvirtualkeyboard/  # 输入法插件 (libIM.so)
```

## 输出文件

编译后生成以下文件：

| 文件 | 说明 |
|------|------|
| `lib/libcore.so` | 核心功能库 |
| `lib/libui.so` | UI 组件库 |
| `plugins/platforminputcontexts/libIM.so` | 输入法插件 |
| `pinyininputmethod/pinyin/data/dict_pinyin.dat` | 拼音词典 |

---

# 一、Ubuntu 22.04 部署

## 2. 编译

```bash
cd /home/linux/emb/14-desktop/im

# 清理旧的构建
rm -rf build

# 使用 qmake 生成 Makefile
mkdir -p build && cd build
qmake ../im.pro -spec linux-g++ CONFIG+=release

# 编译
make -j$(nproc)
```

## 3. 部署文件

**重要**：若你的 Qt 应用（如 Wifi）是用**本地 Qt**（如 `/home/linux/Qt/5.15.2/gcc_64`）编译的，插件必须放到**该 Qt 的插件目录**或通过 `QT_PLUGIN_PATH` 指定，不能只拷贝到 `/usr/lib/qt5/`（系统 Qt5 路径）。

```bash
# 输出目录
OUTPUT_DIR="/home/linux/emb/14-desktop/desktop/im"

# 1. 拷贝动态库到系统目录（或应用程序目录）
sudo cp $OUTPUT_DIR/lib/libcore.so* /usr/lib/
sudo cp $OUTPUT_DIR/lib/libui.so* /usr/lib/

# 2. 拷贝输入法插件（二选一）
# 方式 A：应用使用【系统 Qt5】时，拷贝到系统路径
sudo mkdir -p /usr/lib/qt5/plugins/platforminputcontexts
sudo cp $OUTPUT_DIR/plugins/platforminputcontexts/libIM.so \
    /usr/lib/qt5/plugins/platforminputcontexts/

# 方式 B：应用使用【本地 Qt 5.15.2】时，拷贝到该 Qt 的插件目录
# 用 ldd your_app 查看应用链接的 Qt 路径，再拷贝到对应 plugins 目录
sudo cp $OUTPUT_DIR/plugins/platforminputcontexts/libIM.so \
    /home/linux/Qt/5.15.2/gcc_64/plugins/platforminputcontexts/

# 3. 拷贝拼音词典
sudo mkdir -p /usr/share/pinyin/3rdparty/pinyin/data
sudo cp /home/linux/emb/14-desktop/im/imvirtualkeyboard/pinyininputmethod/pinyin/data/dict_pinyin.dat \
    /usr/share/pinyin/3rdparty/pinyin/data/

# 4. 更新动态库缓存
sudo ldconfig
```

**不拷贝系统目录时**：启动应用前设置 `QT_PLUGIN_PATH=$OUTPUT_DIR/plugins`，Qt 会从此路径加载插件。

## 4. 配置环境变量

```bash
# 在 ~/.bashrc 或 /etc/profile 中添加
export QT_IM_MODULE=im
```

或者在启动 Qt 应用前设置：

```bash
QT_IM_MODULE=im ./your_qt_app
```

## 5. 测试

```bash
# 启动任意 Qt 应用测试
QT_IM_MODULE=im qtcreator
```

点击输入框，软键盘应自动弹出。

---

# 二、i.MX6ULL 部署

## 1. 交叉编译环境准备

确保已安装 i.MX6ULL 的交叉编译工具链和 Qt 5.15.x SDK。

```bash
# 检查交叉编译器
arm-linux-gnueabihf-gcc --version

# 检查 Qt 交叉编译工具
/home/linux/emb/buildroot/output/host/bin/qmake --version
```

## 2. 交叉编译

```bash
cd /home/linux/emb/14-desktop/im

# 清理旧的构建
rm -rf build

# 使用交叉编译工具链的 qmake
mkdir -p build/imx6 && cd build/imx6
/home/linux/emb/buildroot/output/host/bin/qmake ../../im.pro CONFIG+=release

# 编译
make -j$(nproc)
```

## 3. 部署到设备

### 方法一：SCP 部署

```bash
# 设备 IP 地址
DEVICE_IP=192.168.2.7

# 1. 拷贝动态库
scp ../desktop/im/lib/*.so root@$DEVICE_IP:/usr/lib/

# 2. 拷贝输入法插件
ssh root@$DEVICE_IP "mkdir -p /usr/lib/qt/plugins/platforminputcontexts"
scp ../desktop/im/plugins/platforminputcontexts/libIM.so \
    root@$DEVICE_IP:/usr/lib/qt/plugins/platforminputcontexts/

# 3. 拷贝拼音词典
ssh root@$DEVICE_IP "mkdir -p /usr/share/pinyin/3rdparty/pinyin/data"
scp imvirtualkeyboard/pinyininputmethod/pinyin/data/dict_pinyin.dat \
    root@$DEVICE_IP:/usr/share/pinyin/3rdparty/pinyin/data/
```

### 方法二：打包到根文件系统

将文件放入 Buildroot overlay 目录，重新生成根文件系统：

```bash
# Buildroot overlay 目录结构
overlay/
├── usr/
│   ├── lib/
│   │   ├── libcore.so
│   │   ├── libui.so
│   │   └── qt/
│   │       └── plugins/
│   │           └── platforminputcontexts/
│   │               └── libIM.so
│   └── share/
│       └── pinyin/
│           └── 3rdparty/
│               └── pinyin/
│                   └── data/
│                       └── dict_pinyin.dat
└── etc/
    └── profile.d/
        └── qt-im.sh    # 环境变量配置
```

## 4. 配置环境变量

在设备上创建或编辑 `/etc/profile.d/qt-im.sh`：

```bash
#!/bin/sh
export QT_IM_MODULE=im
export QT_PLUGIN_PATH=/usr/lib/qt/plugins
```

或者直接在 `/etc/profile` 中添加：

```bash
export QT_IM_MODULE=im
```

## 5. 重启生效

```bash
# 重启设备
reboot

# 或者重新加载环境变量
source /etc/profile
```

## 6. 测试验证

```bash
# 在设备上运行 Qt 应用
cd /root/desktop/Wifi
./Wifi

# 点击输入框，软键盘应自动弹出
```

---

# 三、使用说明

## 显示模式

| 模式 | 说明 | 设置方法 |
|------|------|----------|
| 跟随模式 | 键盘跟随输入框位置显示 | 默认 |
| 底部固定 | 键盘固定在窗口底部 | `qApp->setProperty("bottom", true);` |

## 代码示例

```cpp
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 可选：设置键盘固定在底部显示
    // app.setProperty("bottom", true);
    
    // ... 创建窗口和输入框
    
    return app.exec();
}
```

---

# 四、故障排除

## 1. 软键盘不弹出

检查环境变量：
```bash
echo $QT_IM_MODULE
# 应输出: im
```

检查插件是否存在：
```bash
ls -la /usr/lib/qt/plugins/platforminputcontexts/libIM.so
```

## 2. 中文输入无候选词

检查拼音词典：
```bash
ls -la /usr/share/pinyin/3rdparty/pinyin/data/dict_pinyin.dat
```

## 3. 加载库失败

检查动态库依赖：
```bash
ldd /usr/lib/qt/plugins/platforminputcontexts/libIM.so
```

更新库缓存：
```bash
ldconfig
```

---

# 五、文件清单

| 文件 | 设备路径 | 说明 |
|------|----------|------|
| libcore.so | /usr/lib/ | 核心库 |
| libui.so | /usr/lib/ | UI 库 |
| libIM.so | /usr/lib/qt/plugins/platforminputcontexts/ | 输入法插件 |
| dict_pinyin.dat | /usr/share/pinyin/3rdparty/pinyin/data/ | 拼音词典 |

---

**注意**：不同 Qt 版本的插件路径可能不同，请根据实际情况调整：
- Qt 5.15 (Buildroot): `/usr/lib/qt/plugins/platforminputcontexts/`
- Qt 5.15 (Ubuntu): `/usr/lib/qt5/plugins/platforminputcontexts/`
