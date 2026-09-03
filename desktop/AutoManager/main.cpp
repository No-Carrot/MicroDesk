/**
 * @file main.cpp
 * @brief 应用程序入口文件
 * 
 * AutoManager - 应用程序自启动管理器
 * 
 * 功能说明：
 * 该程序用于管理嵌入式设备上应用程序的开机自启动设置。
 * 用户可以通过可视化界面选择哪个应用程序在开机时自动运行。
 * 
 * 技术特点：
 * - 基于 Qt 5 框架开发
 * - 使用 Fusion 风格主题
 * - 无边框全屏窗口
 * - 支持高 DPI 显示
 */

#include "widget.h"
#include <QApplication>

/**
 * @brief 程序主入口函数
 * 
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出码（0 = 正常退出）
 */
int main(int argc, char *argv[])
{
    // ============ 高 DPI 支持 ============
    // 在创建 QApplication 之前设置高 DPI 缩放属性
    // 条件编译：仅在 Qt 5.6.0 及以上版本启用
    // 优点：在高分辨率屏幕上获得更好的显示效果
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
    // ============ 创建应用程序实例 ============
    // QApplication 是 Qt GUI 应用程序的核心类
    // 管理应用程序的控制流和主要设置
    QApplication a(argc, argv);
    
    // 设置应用程序风格为 Fusion
    // Fusion 是 Qt 内置的跨平台风格，外观现代统一
    a.setStyle("Fusion");
    
    // ============ 创建并显示主窗口 ============
    Widget w;
    
    // 设置窗口无边框
    // Qt::FramelessWindowHint 移除标题栏和边框，实现全屏桌面效果
    // 注意：无边框窗口需要自行实现关闭按钮和拖动功能
    w.setWindowFlags(Qt::FramelessWindowHint);
    
    // 显示窗口
    w.show();
    
    // ============ 进入事件循环 ============
    // exec() 启动 Qt 事件循环，处理用户输入和系统事件
    // 直到调用 quit() 或所有窗口关闭时返回
    return a.exec();
}
