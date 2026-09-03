#include "widget.h"
#include "gconfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    // 使用主题功能
    a.setStyle("Fusion");
    
    // 隐藏 Qt 鼠标光标
    a.setOverrideCursor(Qt::BlankCursor);

    Widget w;
    // 设置无边框，实现全屏桌面的效果
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.resize(GCInstance.screenWidth, GCInstance.screenHeight);
    
    // 根据是否有自启动应用决定启动方式
    if (w.hasAutoStartApp()) {
        // 有自启动应用：不显示桌面，直接启动应用
        w.startWithAutoApp();
    } else {
        // 无自启动应用：直接显示桌面
        w.showDesktop();
    }
    
    return a.exec();
}
