#ifndef SENSOR__H
#define SENSOR__H
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

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

private slots:

    void on_pushButton_quit_clicked();

    // 定时器超时槽函数
    void timer_timeout();

private:
    Ui::Widget *ui;
    // 定义1个定时器 
    QTimer *timer;
};
#endif // SENSOR__H
