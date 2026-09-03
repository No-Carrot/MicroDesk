#ifndef AppWindow_H
#define AppWindow_H

#include <QWidget>
#include "ui_global.h"

class TitleBar;
class EventHandler;
class UI_EXPORT AppWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *centerWidget, const QString &title,
                       QWidget *parent = nullptr);

    ~AppWindow();

    static void initGloablBeforeApplication();

    //全局初始化，比如事件系统.
    static void initGloabl(bool bUseDefalutEventhandler =true);

    //销毁处理
    static void destoryGloabl();

    static void loadSplashText(const QString &text, const QColor &c = Qt::black);

    static void setkeyboard(bool bFollow =true);

    //重写显示函数，显示为初始屏幕尺寸.
    void show();

   Q_INVOKABLE void setTitleBarVisible (bool bVisble);
private:
    static EventHandler *g_handler;
    TitleBar *m_bar;
};

#endif // AppWindow_H
