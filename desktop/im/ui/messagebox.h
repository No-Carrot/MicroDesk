/*消息堵塞窗口，堵塞父类窗口.父窗口过小导致消息窗口显示不全，适用全屏的显示窗口.用于部分平台弹窗无法使用情况.
  */
#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include "ui_global.h"

class QEventLoop;
class UI_EXPORT  MessageBox : public QWidget
{
    Q_OBJECT

public:
    enum Button{
        no=1,
        yes=2,
        yes_no=3
    };

    //显示一个单独的按钮
    static int showMessage(QWidget *parent, const QString &text,  Button button= yes);

    //显示一个单独的Ok 按钮，seconds秒后自动关闭.
    static void showMessage(QWidget *parent, const QString &text,  int seconds);

protected:
    explicit MessageBox(QWidget *parent, const QString &text,  Button button= yes);
    int exec();
    QEventLoop *m_pLoop= nullptr;
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MESSAGEBOX_H
