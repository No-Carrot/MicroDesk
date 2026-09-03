#ifndef GPIO_H
#define GPIO_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>

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

    void on_pushButton_led1_clicked(bool checked);

    void on_pushButton_led2_clicked(bool checked);

    void on_pushButton_beep_clicked(bool checked);

    void on_pushButton_quit_clicked();

    void on_pushButton_extio1_clicked(bool checked);

    void on_pushButton_extio2_clicked(bool checked);

    void on_pushButton_extio3_clicked(bool checked);

private:
    Ui::Widget *ui;
};
#endif // GPIO_H
