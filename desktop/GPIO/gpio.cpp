#include "gpio.h"
#include "ui_gpio.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initUi();// 初始化窗体

    // 读取led1 的状态
    QProcess process;
    process.start("cat /sys/class/leds/led1/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    QString str = process.readAllStandardOutput();
    bool ok;
    int value = str.toInt(&ok);
    if(ok) // true 转换成功
    {
        qDebug() << "led1 value = " << value ;
        if(value == 1)
        {
            ui->pushButton_led1->setChecked(true); // 按钮按下
            ui->pushButton_led1->setStyleSheet("QPushButton{background-image: url(:/images/led1on.png);"
                                       "border: none; }");
        }
        else
        {
            ui->pushButton_led1->setChecked(false); // 按钮抬起
            ui->pushButton_led1->setStyleSheet("QPushButton{background-image: url(:/images/led1off.png);"
                                       "border: none; }");
        }
    }
    else
    {
        qDebug() << "failed to led1 value" ;
    }


    // 读取led2 的状态
    process.start("cat /sys/class/leds/led2/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    str = process.readAllStandardOutput();
    value = str.toInt(&ok);
    if(ok) // true 转换成功
    {
        qDebug() << "led2 value = " << value ;
        if(value == 1)
        {
            ui->pushButton_led2->setChecked(true); // 按钮按下
            ui->pushButton_led2->setStyleSheet("QPushButton{background-image: url(:/images/led2on.png);"
                                       "border: none; }");
        }
        else
        {
            ui->pushButton_led2->setChecked(false); // 按钮抬起
            ui->pushButton_led2->setStyleSheet("QPushButton{background-image: url(:/images/led2off.png);"
                                       "border: none; }");
        }
    }
    else
    {
        qDebug() << "failed to led2 value" ;
    }

    // 读取beep 的状态
    process.start("cat /sys/class/leds/beep/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    str = process.readAllStandardOutput();
    value = str.toInt(&ok);
    if(ok) // true 转换成功
    {
        qDebug() << "beep value = " << value ;
        if(value == 1)
        {
            ui->pushButton_beep->setChecked(true); // 按钮按下
            ui->pushButton_beep->setStyleSheet("QPushButton{background-image: url(:/images/beepon.png);"
                                       "border: none; }");
        }
        else
        {
            ui->pushButton_beep->setChecked(false); // 按钮抬起
            ui->pushButton_beep->setStyleSheet("QPushButton{background-image: url(:/images/beepoff.png);"
                                       "border: none; }");
        }
    }
    else
    {
        qDebug() << "failed to beep value" ;
    }

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_led1_clicked(bool checked)
{
    qDebug() << "led1 clicked=" << checked  ;
    if(checked) // led1 on
    {
        system("echo 1 > /sys/class/leds/led1/brightness"); // 系统调用
        ui->pushButton_led1->setStyleSheet("QPushButton{background-image: url(:/images/led1on.png);"
                                       "border: none; }");
    }
    else // led1 off
    {
        system("echo 0 > /sys/class/leds/led1/brightness"); // 系统调用
        ui->pushButton_led1->setStyleSheet("QPushButton{background-image: url(:/images/led1off.png);"
                                       "border: none; }");
    }

}


void Widget::on_pushButton_led2_clicked(bool checked)
{
    qDebug() << "led2 clicked=" << checked  ;
    if(checked) // led2 on
    {
        system("echo 1 > /sys/class/leds/led2/brightness"); // 系统调用
        ui->pushButton_led2->setStyleSheet("QPushButton{background-image: url(:/images/led2on.png);"
                                       "border: none; }");
    }
    else // led2 off
    {
        system("echo 0 > /sys/class/leds/led2/brightness"); // 系统调用
        ui->pushButton_led2->setStyleSheet("QPushButton{background-image: url(:/images/led2off.png);"
                                       "border: none; }");
    }

}


void Widget::on_pushButton_beep_clicked(bool checked)
{
    qDebug() << "beep clicked=" << checked  ;
    if(checked) // beep on
    {
        system("echo 1 > /sys/class/leds/beep/brightness"); // 系统调用
        ui->pushButton_beep->setStyleSheet("QPushButton{background-image: url(:/images/beepon.png);"
                                       "border: none; }");
    }
    else // beep off
    {
        system("echo 0 > /sys/class/leds/beep/brightness"); // 系统调用
        ui->pushButton_beep->setStyleSheet("QPushButton{background-image: url(:/images/beepoff.png);"
                                       "border: none; }");
    }

}


void Widget::on_pushButton_extio1_clicked(bool checked)
{
    qDebug() << "extio1 clicked=" << checked  ;
    if(checked) // extio1 on
    {
        system("echo 1 > /sys/class/leds/extio1/brightness"); // 系统调用
        ui->pushButton_extio1->setStyleSheet("QPushButton{background-image: url(:/images/extio1on.png);"
                                       "border: none; }");
    }
    else // extio1 off
    {
        system("echo 0 > /sys/class/leds/extio1/brightness"); // 系统调用
        ui->pushButton_extio1->setStyleSheet("QPushButton{background-image: url(:/images/extio1off.png);"
                                       "border: none; }");
    }

}


void Widget::on_pushButton_extio2_clicked(bool checked)
{
    qDebug() << "extio2 clicked=" << checked  ;
    if(checked) // extio2 on
    {
        system("echo 1 > /sys/class/leds/extio2/brightness"); // 系统调用
        ui->pushButton_extio2->setStyleSheet("QPushButton{background-image: url(:/images/extio2on.png);"
                                       "border: none; }");
    }
    else // extio2 off
    {
        system("echo 0 > /sys/class/leds/extio2/brightness"); // 系统调用
        ui->pushButton_extio2->setStyleSheet("QPushButton{background-image: url(:/images/extio2off.png);"
                                       "border: none; }");
    }

}


void Widget::on_pushButton_extio3_clicked(bool checked)
{
    qDebug() << "extio3 clicked=" << checked  ;
    if(checked) // extio3 on
    {
        system("echo 1 > /sys/class/leds/extio3/brightness"); // 系统调用
        ui->pushButton_extio3->setStyleSheet("QPushButton{background-image: url(:/images/extio3on.png);"
                                       "border: none; }");
    }
    else // extio3 off
    {
        system("echo 0 > /sys/class/leds/extio3/brightness"); // 系统调用
        ui->pushButton_extio3->setStyleSheet("QPushButton{background-image: url(:/images/extio3off.png);"
                                       "border: none; }");
    }

}


void Widget::initUi()
{
    // 绘制背景图片
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(QString::fromUtf8(":/images/back.png"));
    QImage pix = ImgAllbackground.scaled(this->size(),Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(pix));
    this->setPalette(PAllbackground);


    ui->pushButton_led1->setStyleSheet("QPushButton{background-image: url(:/images/led1off.png);"
                                       "border: none; }");

    ui->pushButton_led2->setStyleSheet("QPushButton{background-image: url(:/images/led2off.png);"
                                       "border: none; }");

    ui->pushButton_beep->setStyleSheet("QPushButton{background-image: url(:/images/beepoff.png);"
                                       "border: none; }");

    ui->pushButton_extio1->setStyleSheet("QPushButton{background-image: url(:/images/extio1off.png);"
                                        "border: none; }");


    ui->pushButton_extio2->setStyleSheet("QPushButton{background-image: url(:/images/extio2off.png);"
                                         "border: none; }");

    ui->pushButton_extio3->setStyleSheet("QPushButton{background-image: url(:/images/extio3off.png);"
                                         "border: none; }");

    ui->pushButton_quit->setStyleSheet("QPushButton{background-image: url(:/images/quit.png);"
                                         "border: none; }");



}


void Widget::on_pushButton_quit_clicked()
{
    close();
    qApp->exit();
}


