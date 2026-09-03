#include "sensor.h"
#include "ui_sensor.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initUi(); // 初始化窗体

    // 初始化定时器
    timer = new QTimer(this);
    // 连接定时器信号到槽函数
    connect(timer, &QTimer::timeout, this, &Widget::timer_timeout);
    // 启动定时器，每1000毫秒触发一次
    timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

// 定时器超时槽函数
void Widget::timer_timeout()
{
#ifdef __arm__

    // 读取aht20 温度和湿度数据
    // 23.4,15.3
    QProcess process;
    process.start("./aht20");  // 使用相对路径，aht20 在同目录下
    process.waitForFinished();
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    QStringList list = output.split(",");
    if (list.size() >= 2)
    {
        QString temp = list[0].trimmed(); // 23.4  
        QString hum = list[1].trimmed();  // 15.3
        ui->pushButton_temp->setText(temp + "℃"); // 带上单位
        ui->pushButton_hum->setText(hum + "%"); // 带上单位
    }

    // vr 电位器的值 
    // 读取vr 电位器的值
    process.start("cat /sys/bus/iio/devices/iio:device0/in_voltage3_raw");
    process.waitForFinished();
    QString vr_output = QString::fromUtf8(process.readAllStandardOutput());
    QString vr_value = QString::asprintf("%.2f", vr_output.trimmed().toInt()*3.3/4096);
    ui->pushButton_vr->setText(vr_value + "V"); // 带上单位 

    // 读取ina226 vol的值
    process.start("cat /sys/class/hwmon/hwmon0/in1_input");
    process.waitForFinished();
    QString vol_output = QString::fromUtf8(process.readAllStandardOutput());
    QString vol_value = QString::asprintf("%.2f", vol_output.trimmed().toInt()/1000.0);
    ui->pushButton_vol->setText(vol_value + "V"); // 带上单位 

    // 读取ina226 电流的值
    process.start("cat /sys/class/hwmon/hwmon0/curr1_input");
    process.waitForFinished();
    QString cur_output = QString::fromUtf8(process.readAllStandardOutput());
    QString cur_value = QString::asprintf("%.0f", cur_output.trimmed().toInt()/1.0);
    ui->pushButton_cur->setText(cur_value + "MA"); // 带上单位 

    // 读取ina226 功率的值
    process.start("cat /sys/class/hwmon/hwmon0/power1_input");
    process.waitForFinished();
    QString pow_output = QString::fromUtf8(process.readAllStandardOutput());
    QString pow_value = QString::asprintf("%.2f", pow_output.trimmed().toInt()/1000.0/1000.0);
    ui->pushButton_pw->setText(pow_value + "W"); // 带上单位

    // 读取温度传感器数据 CPU 温度
    process.start("cat /sys/class/thermal/thermal_zone0/temp");
    process.waitForFinished();
    QString temp_output = QString::fromUtf8(process.readAllStandardOutput());
    QString temp_value = QString::asprintf("%.0f", temp_output.trimmed().toInt()/1000.0);
    ui->pushButton_cpu->setText(temp_value + "℃"); // 带上单位


#endif
}



void Widget::initUi()
{
    // 绘制背景图片
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(QString::fromUtf8(":/images/back.png"));
    QImage pix = ImgAllbackground.scaled(this->size(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(pix));
    this->setPalette(PAllbackground);



    QFont font;
    font.setFamily(QString::fromUtf8("Noto Sans CJK SC"));
    font.setPointSize(18);
    font.setBold(true);
    ui->pushButton_temp->setStyleSheet("QPushButton{background-image: url(:/images/temp.png);"
                                    "color:white;"
                                    "text-align: right; padding-right: 15px;padding-top: 30px;"
                                    "border: none; }");
    ui->pushButton_temp->setFont(font);

    ui->pushButton_hum->setStyleSheet("QPushButton{background-image: url(:/images/hum.png);"
                                      "color:white;"
                                      "text-align: right; padding-right: 12px;padding-top: 30px;"
                                      "border: none; }");
    ui->pushButton_hum->setFont(font);

    // ui->pushButton_cpu->setStyleSheet("QPushButton{background-image: url(:/images/cpu.png);"
    //                                   "color:white;"
    //                                   "text-align: right; padding-right: 12px;padding-top: 30px;"
    //                                   "border: none; }");
    // ui->pushButton_cpu->setFont(font);

    ui->pushButton_vr->setStyleSheet("QPushButton{background-image: url(:/images/vr.png);"
                                     "color:white;"
                                     "text-align: right; padding-right: 15px;padding-top: 30px;"
                                     "border: none; }");
    ui->pushButton_vr->setFont(font);

    ui->pushButton_vol->setStyleSheet("QPushButton{background-image: url(:/images/vol.png);"
                                      "color:white;"
                                      "text-align: right; padding-right: 15px;padding-top: 30px;"
                                      "border: none; }");
    ui->pushButton_vol->setFont(font);

    ui->pushButton_cur->setStyleSheet("QPushButton{background-image: url(:/images/cur.png);"
                                      "color:white;"
                                      "text-align: right; padding-right: 12px;padding-top: 30px;"
                                      "border: none; }");
    ui->pushButton_cur->setFont(font);

    ui->pushButton_pw->setStyleSheet("QPushButton{background-image: url(:/images/pw.png);"
                                     "color:white;"
                                     "text-align: right; padding-right: 12px;padding-top: 30px;"
                                     "border: none; }");
    ui->pushButton_pw->setFont(font);

    ui->pushButton_cpu->setStyleSheet("QPushButton{background-image: url(:/images/cpu.png);"
                                     "color:white;"
                                     "text-align: right; padding-right: 12px;padding-top: 30px;"
                                     "border: none; }");
    ui->pushButton_cpu->setFont(font);



    // ui->pushButton_temp->setStyleSheet("QPushButton{background-image: url(:/images/temp.png);"
    //                                    "border: none; }");

    // ui->pushButton_hum->setStyleSheet("QPushButton{background-image: url(:/images/hum.png);"
    //                                  "border: none; }");

    // ui->pushButton_vr->setStyleSheet("QPushButton{background-image: url(:/images/vr.png);"
    //                                     "border: none; }");

    // ui->pushButton_vol->setStyleSheet("QPushButton{background-image: url(:/images/vol.png);"
    //                                  "border: none; }");

    // ui->pushButton_cur->setStyleSheet("QPushButton{background-image: url(:/images/cur.png);"
    //                                    "border: none; }");

    // ui->pushButton_pw->setStyleSheet("QPushButton{background-image: url(:/images/pw.png);"
    //                                   "border: none; }");

    ui->pushButton_quit->setStyleSheet("QPushButton{background-image: url(:/images/quit.png);"
                                       "border: none; }");


}

void Widget::on_pushButton_quit_clicked()
{
    close();
    qApp->exit();
}
