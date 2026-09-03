#ifndef KEY_H
#define KEY_H
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void initUi(); // 初始化窗口的函数

protected : 
    // 重新实现键盘事件 
    // 键盘按下事件
    void keyPressEvent(QKeyEvent *event); 
    // 重新实现键盘事件 
    // 键盘释放事件
    void keyReleaseEvent(QKeyEvent *event); 

    // 重新实现绘画事件
    void paintEvent(QPaintEvent *event);

private slots:

    void on_pushButton_quit_clicked();

private:
    Ui::Widget *ui;
};
#endif // KEY_H
