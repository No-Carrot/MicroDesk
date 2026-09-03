#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置pushbutton  check 模式
    ui->pushButton_connect->setCheckable(true);
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_ssid->setText("V50");
    ui->lineEdit_passwd->setText("020323zyh");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_quit_clicked()
{
    close();
}

void Widget::on_pushButton_connect_clicked(bool checked)
{
    // checked 为真 时，连接 wifi
    if (checked) // 连接 wifi
    {
        // 获取 wifi 名称和密码
        wifi_name = ui->lineEdit_ssid->text();
        wifi_password = ui->lineEdit_passwd->text();
        // 连接 wifi
        if (wifi_name.isEmpty() || wifi_password.isEmpty())
        {
            QMessageBox::warning(this, "警告", "请输入 wifi 名称和密码");
            return;
        }
        // 把账号和密码信息保存到 /etc/wpa_supplicant/wpa_supplicant.conf 文件中
        QString wpa_supplicant_conf = "network={\n"
                                      "    ssid=\"" +
                                      wifi_name + "\"\n"
                                                  "    psk=\"" +
                                      wifi_password + "\"\n"
                                                      "}\n";
        QFile file("/etc/wpa_supplicant.conf");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << wpa_supplicant_conf;
            file.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "无法打开 wpa_supplicant.conf 文件");
            // 按钮抬起 
            ui->pushButton_connect->setChecked(false);
            return;
        }

        // 重启wifi 服务
        //  wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf
        // 我的文件系统是 busybox , 没有 service 命令
        // 手动重启 wifi 服务
        system("wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf &");
        
        // 更新按钮状态提示用户正在连接
        ui->pushButton_connect->setText("连接中...");
        ui->pushButton_connect->setEnabled(false);
        
        // 轮询检测 WiFi 连接状态（最多等待 15 秒，每 500ms 检测一次）
        const int maxWaitTime = 15000;  // 最大等待时间 15 秒
        const int checkInterval = 500;  // 检测间隔 500ms
        int elapsedTime = 0;
        bool connected = false;
        
        while (elapsedTime < maxWaitTime)
        {
            QThread::msleep(checkInterval);
            elapsedTime += checkInterval;
            
            // 检查 WiFi 是否连接成功
            QProcess process;
            process.start("iwgetid", QStringList() << "-r");
            process.waitForFinished(1000);
            QString result = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
            
            qDebug() << "Checking WiFi... elapsed:" << elapsedTime << "ms, result:" << result;
            
            if (result == wifi_name)
            {
                connected = true;
                break;  // 连接成功，立即退出循环
            }
            
            // 处理事件，保持界面响应
            QCoreApplication::processEvents();
        }
        
        // 恢复按钮状态
        ui->pushButton_connect->setEnabled(true);
        
        if (connected)
        {
            QMessageBox::information(this, "提示", "WiFi 连接成功");
            ui->pushButton_connect->setText("已连接");
            // 连接成功后，禁用 ssid 和 passwd 输入框
            ui->lineEdit_ssid->setEnabled(false);
            ui->lineEdit_passwd->setEnabled(false);
        }
        else
        {
            QMessageBox::warning(this, "警告", "WiFi 连接失败（超时）");
            // 连接失败后，启用 ssid 和 passwd 输入框
            ui->lineEdit_ssid->setEnabled(true);
            ui->lineEdit_passwd->setEnabled(true);
            // 按钮抬起 
            ui->pushButton_connect->setChecked(false);
        }
    }
    else
    {
        ui->pushButton_connect->setText("连接");
    }
}
