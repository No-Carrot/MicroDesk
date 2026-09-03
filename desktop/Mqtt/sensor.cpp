#include "sensor.h"
#include "ui_sensor.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), settings("../MicroDesktop/Desktop.conf", QSettings::IniFormat)
{
    ui->setupUi(this);

    // 创建 OTA 模块
    httpOta = new HttpOta(this, &settings);
    connect(httpOta, &HttpOta::progress, this, &Widget::onOtaProgress);
    connect(httpOta, &HttpOta::statusMessage, this, &Widget::onOtaStatusMessage);
    connect(httpOta, &HttpOta::finished, this, &Widget::onOtaFinished);

    this->initUi();

    // 读取 setting 配置文件
    // ============ 设置窗口尺寸 ============
    QString tocken = settings.value("Tocken/tocken").toString();
    qDebug() << "tocken:" << tocken;
    qDebug() << "server:" << ui->comboBox_server->currentText();

    // 初始化定时器
    timer = new QTimer(this);
    // 连接定时器信号到槽函数
    connect(timer, &QTimer::timeout, this, &Widget::timer_timeout);

    // 读取led1 的状态
    QProcess process;
    process.start("cat /sys/class/leds/led1/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    QString str = process.readAllStandardOutput();
    bool ok;
    int value = str.toInt(&ok);
    if (ok) // true 转换成功
    {
        qDebug() << "led1 value = " << value;
        if (value == 1)
        {
            ui->pushButton_led1->setChecked(true); // 按钮按下
            ui->pushButton_led1->setStyleSheet(
                "QPushButton#pushButton_led1 {"
                "  border: none;"
                "  border-image: url(:/images/led1on.png) 1 1 1 1 stretch stretch;"
                "}");
        }
        else
        {
            ui->pushButton_led1->setChecked(false); // 按钮抬起
            ui->pushButton_led1->setStyleSheet(
                "QPushButton#pushButton_led1 {"
                "  border: none;"
                "  border-image: url(:/images/led1off.png) 1 1 1 1 stretch stretch;"
                "}");
        }
    }
    else
    {
        qDebug() << "failed to led1 value";
    }

    // 读取led2 的状态
    process.start("cat /sys/class/leds/led2/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    str = process.readAllStandardOutput();
    value = str.toInt(&ok);
    if (ok) // true 转换成功
    {
        qDebug() << "led2 value = " << value;
        if (value == 1)
        {
            ui->pushButton_led2->setChecked(true); // 按钮按下
            ui->pushButton_led2->setStyleSheet(
                "QPushButton#pushButton_led2 {"
                "  border: none;"
                "  border-image: url(:/images/led2on.png) 1 1 1 1 stretch stretch;"
                "}");
        }
        else
        {
            ui->pushButton_led2->setChecked(false); // 按钮抬起
            ui->pushButton_led2->setStyleSheet(
                "QPushButton#pushButton_led2 {"
                "  border: none;"
                "  border-image: url(:/images/led2off.png) 1 1 1 1 stretch stretch;"
                "}");
        }
    }
    else
    {
        qDebug() << "failed to led2 value";
    }

    // 读取extio1 的状态
    process.start("cat /sys/class/leds/extio1/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    str = process.readAllStandardOutput();
    value = str.toInt(&ok);
    if (ok) // true 转换成功
    {
        qDebug() << "extio1 value = " << value;
        if (value == 1)
        {
            ui->pushButton_beep->setChecked(true); // 按钮按下
            ui->pushButton_beep->setStyleSheet(
                "QPushButton#pushButton_beep {"
                "  border: none;"
                "  border-image: url(:/images/extio1on.png) 1 1 1 1 stretch stretch;"
                "}");
        }
        else
        {
            ui->pushButton_beep->setChecked(false); // 按钮抬起
            ui->pushButton_beep->setStyleSheet(
                "QPushButton#pushButton_beep {"
                "  border: none;"
                "  border-image: url(:/images/extio1off.png) 1 1 1 1 stretch stretch;"
                "}");
        }
    }
    else
    {
        qDebug() << "failed to extio1 value";
    }

    // 读取beep 的状态
    process.start("cat /sys/class/leds/beep/brightness");
    process.waitForFinished(); // 等待程序执行完成
    // 读取进程输出的结果
    str = process.readAllStandardOutput();
    value = str.toInt(&ok);
    if (ok) // true 转换成功
    {
        qDebug() << "beep value = " << value;
        if (value == 1)
        {
            ui->pushButton_beep->setChecked(true); // 按钮按下
            ui->pushButton_beep->setStyleSheet(
                "QPushButton#pushButton_beep {"
                "  border: none;"
                "  border-image: url(:/images/beepon.png) 1 1 1 1 stretch stretch;"
                "}");
        }
        else
        {
            ui->pushButton_beep->setChecked(false); // 按钮抬起
            ui->pushButton_beep->setStyleSheet(
                "QPushButton#pushButton_beep {"
                "  border: none;"
                "  border-image: url(:/images/beepoff.png) 1 1 1 1 stretch stretch;"
                "}");
        }
    }
    else
    {
        qDebug() << "failed to beep value";
    }

    // 按下按钮 ， 触发槽函数 on_pushButton_clicked()
    on_pushButton_open_clicked(true); // 按钮按下 ， 触发槽函数 on_pushButton_clicked()
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
    process.start("./aht20"); // 使用相对路径，aht20 在同目录下
    process.waitForFinished();
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    QStringList list = output.split(",");
    QString temp;
    QString hum;
    if (list.size() >= 2)
    {
        temp = list[0].trimmed();                 // 23.4
        hum = list[1].trimmed();                  // 15.3
        ui->pushButton_temp->setText(temp + "℃"); // 带上单位
        ui->pushButton_hum->setText(hum + "%");   // 带上单位
    }

    // vr 电位器的值
    // 读取vr 电位器的值
    process.start("cat /sys/bus/iio/devices/iio:device0/in_voltage3_raw");
    process.waitForFinished();
    QString vr_output = QString::fromUtf8(process.readAllStandardOutput());
    QString vr_value = QString::asprintf("%.2f", vr_output.trimmed().toInt() * 3.3 / 4096);
    ui->pushButton_vr->setText(vr_value + "V"); // 带上单位

    // 读取ina226 vol的值
    process.start("cat /sys/class/hwmon/hwmon0/in1_input");
    process.waitForFinished();
    QString vol_output = QString::fromUtf8(process.readAllStandardOutput());
    QString vol_value = QString::asprintf("%.2f", vol_output.trimmed().toInt() / 1000.0);
    ui->pushButton_vol->setText(vol_value + "V"); // 带上单位

    // 读取ina226 电流的值
    process.start("cat /sys/class/hwmon/hwmon0/curr1_input");
    process.waitForFinished();
    QString cur_output = QString::fromUtf8(process.readAllStandardOutput());
    QString cur_value = QString::asprintf("%.0f", cur_output.trimmed().toInt() / 1.0);
    ui->pushButton_cur->setText(cur_value + "MA"); // 带上单位

    // 读取ina226 功率的值
    process.start("cat /sys/class/hwmon/hwmon0/power1_input");
    process.waitForFinished();
    QString pow_output = QString::fromUtf8(process.readAllStandardOutput());
    QString pow_value = QString::asprintf("%.2f", pow_output.trimmed().toInt() / 1000.0 / 1000.0);
    ui->pushButton_pw->setText(pow_value + "W"); // 带上单位

    // 读取温度传感器数据 CPU 温度
    process.start("cat /sys/class/thermal/thermal_zone0/temp");
    process.waitForFinished();
    QString temp_output = QString::fromUtf8(process.readAllStandardOutput());
    QString temp_value = QString::asprintf("%.0f", temp_output.trimmed().toInt() / 1000.0);
    ui->pushButton_cpu->setText(temp_value + "℃"); // 带上单位

    QJsonObject sensor_json;
    // 上行主题： v1/devices/me/telemetry
    // {TP:20.5,RH:15.3,VR:2.5,VOL:4.2,CUR:100,PW:10.0,CPU:20.5}
    sensor_json.insert("TP", temp.toFloat());
    sensor_json.insert("RH", hum.toFloat());
    sensor_json.insert("VR", vr_value.toFloat());
    sensor_json.insert("VO", vol_value.toFloat());
    sensor_json.insert("CU", cur_value.toFloat());
    sensor_json.insert("PW", pow_value.toFloat());
    sensor_json.insert("CPU", temp_value.toFloat());
    QJsonDocument doc;                                        // 定义1个json字符串
    doc.setObject(sensor_json);                               // 初始化1个json字符串
    QByteArray cmdArray = doc.toJson(QJsonDocument::Compact); // 把json字符串 转换成 bytearray
    if (mqttclient != nullptr)
    {
        mqttclient->publish(upload_topic1, cmdArray); // 发送数据
    }
#else
    QJsonObject sensor_json;
    // 上行主题： v1/devices/me/telemetry
    // {TP:20.5,RH:15.3,VR:2.5,VOL:4.2,CUR:100,PW:10.0,CPU:20.5}
    int temp_value = 20;
    int hum = 15;
    float vr_value = 2.5;
    float vol_value = 4.2;
    float cur_value = 100.0;
    float pow_value = 10.0;
    int cpu_value = 20;
    sensor_json.insert("TP", temp_value);
    sensor_json.insert("RH", hum);
    sensor_json.insert("VR", vr_value);
    sensor_json.insert("VOL", vol_value);
    sensor_json.insert("CUR", cur_value);
    sensor_json.insert("PW", pow_value);
    sensor_json.insert("CPU", cpu_value);
    QJsonDocument doc;                                        // 定义1个json字符串
    doc.setObject(sensor_json);                               // 初始化1个json字符串
    QByteArray cmdArray = doc.toJson(QJsonDocument::Compact); // 把json字符串 转换成 bytearray
    if (mqttclient != nullptr)
    {
        mqttclient->publish(upload_topic1, cmdArray); // 发送数据
    }

    qDebug() << "upload_topic1:" << cmdArray;
#endif
}



void Widget::on_pushButton_quit_clicked()
{
    close();
    qApp->exit();
}

void Widget::mqtt_stateChanged_slot(QMqttClient::ClientState state)
{
    // qDebug() << "ClientState:" << state ;
    if (state == QMqttClient::Connecting)
    {
        qDebug() << "mqttclient connectting mqttserver...";
        ui->comboBox_server->setEnabled(false); // 不可操作
    }
    else if (state == QMqttClient::Connected)
    {
        qDebug() << "mqttclient connected mqttserver";
        ui->pushButton_open->setText(tr("断开连接"));

        // 订阅 RPC 请求主题
        QMqttSubscription *sub = mqttclient->subscribe(download_topic1 + "+");
        if (sub == nullptr)
        {
            qDebug() << "mqttclient subscribe rpc failed";
        }
        else
        {
            qDebug() << "mqttclient subscribe rpc ok";
        }

        // 订阅设备属性主题（用于接收 OTA 固件信息）
        QMqttSubscription *sub2 = mqttclient->subscribe(download_topic2);
        if (sub2 == nullptr)
        {
            qDebug() << "mqttclient subscribe attributes failed";
        }
        else
        {
            qDebug() << "mqttclient subscribe attributes ok";
        }

        // 启动定时器，每5000毫秒触发一次  , 开始发送数据
        timer->start(5000); //
        timer_timeout();    // 第一次触发
    }
    else if (state == QMqttClient::Disconnected)
    {
        qDebug() << "mqttclient disconnected mqttserver";
        ui->pushButton_open->setText(tr("连接服务器"));
        ui->comboBox_server->setEnabled(true); // 可操作

        // 关闭定时器，停止发送数据
        timer->stop(); // 关闭定时器

        // 取消订阅
        mqttclient->unsubscribe(download_topic1);

        if (mqttclient != nullptr)
        {
            mqttclient->cleanSession();
            mqttclient->deleteLater();
            mqttclient = nullptr;
        }
        // 再次链接服务器
        on_pushButton_open_clicked(true); // 再次连接服务器
    }
}

void Widget::mqtt_messageReceived_slot(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "mqtt_messageReceived_slot:" << message;
    qDebug() << "topic.name:" << topic.name();
    // "v1/devices/me/rpc/request/4"
    // "v1/devices/me/rpc/request/5"
    // "v1/devices/me/rpc/request/6"
    // 匹配 "v1/devices/me/rpc/request/" 开头的主题
    if (topic.name().startsWith(download_topic1))
    {
        // qDebug() << "download_topic1:" << download_topic1;
        // qDebug() << "download topic.name:" << topic.name();
        // qDebug() << "   message:" << message;

        // {\"method\":\"led1Status\",\"params\":null}
        // {\"method\":\"led2Status\",\"params\":null}
        // {\"method\":\"io1Status\",\"params\":null}
        // {\"method\":\"beepStatus\",\"params\":null}

        // {\"method\":\"led1Set\",\"params\":true}
        // {\"method\":\"led2Set\",\"params\":true}
        // {\"method\":\"io1Set\",\"params\":true}
        // {\"method\":\"beepSet\",\"params\":true}

        // 解析json字符串并返回状态
        QJsonDocument jsonstr = QJsonDocument::fromJson(message); // 把message 转换成 json字符串
        if (!jsonstr.isNull())                                    // 判断不为空
        {
            // 收到了正确的数据
            QJsonObject jsonObject = jsonstr.object();          // 解析一个json对象， 就是解析json字符串
            QJsonValue jsonMethod = jsonObject.value("method"); // 获取方法的值
            QJsonValue jsonParams = jsonObject.value("params"); // 获取参数的值
            if (jsonMethod == "led1Status")
            {
                qDebug() << "led1Status";
                // 回复主题消息， 返回led1Status的值 , 服务器是thingsboard的规则引擎
                QJsonObject jsonObject;
                jsonObject.insert("led1Status", ui->pushButton_led1->isChecked());
                QJsonDocument doc(jsonObject);
                QByteArray payload = doc.toJson(QJsonDocument::Compact);
                if (mqttclient != nullptr)
                {
                    // 从请求主题中解析请求 ID，例如：v1/devices/me/rpc/request/4 -> 4
                    QString topicName = topic.name();
                    QString requestId = topicName.mid(download_topic1.length());
                    QString responseTopic = upload_topic2 + requestId; // v1/devices/me/rpc/response/4

                    qDebug() << "payload:" << payload;
                    qDebug() << "responseTopic:" << responseTopic;

                    mqttclient->publish(responseTopic, payload);
                }
            }
            else if (jsonMethod == "led2Status")
            {
                qDebug() << "led2Status";
                // 回复主题消息， 返回led2Status的值 , 服务器是thingsboard的规则引擎
                QJsonObject jsonObject;
                jsonObject.insert("led2Status", ui->pushButton_led2->isChecked());
                QJsonDocument doc(jsonObject);
                QByteArray payload = doc.toJson(QJsonDocument::Compact);
                if (mqttclient != nullptr)
                {
                    // 从请求主题中解析请求 ID，例如：v1/devices/me/rpc/request/4 -> 4
                    QString topicName = topic.name();
                    QString requestId = topicName.mid(download_topic1.length());
                    QString responseTopic = upload_topic2 + requestId; // v1/devices/me/rpc/response/4

                    qDebug() << "payload:" << payload;
                    qDebug() << "responseTopic:" << responseTopic;

                    mqttclient->publish(responseTopic, payload);
                }
            }
            else if (jsonMethod == "io1Status")
            {
                qDebug() << "io1Status";
                // 回复主题消息， 返回io1Status的值 , 服务器是thingsboard的规则引擎
                QJsonObject jsonObject;
                jsonObject.insert("io1Status", ui->pushButton_extio1->isChecked());
                QJsonDocument doc(jsonObject);
                QByteArray payload = doc.toJson(QJsonDocument::Compact);
                if (mqttclient != nullptr)
                {
                    // 从请求主题中解析请求 ID，例如：v1/devices/me/rpc/request/4 -> 4
                    QString topicName = topic.name();
                    QString requestId = topicName.mid(download_topic1.length());
                    QString responseTopic = upload_topic2 + requestId; // v1/devices/me/rpc/response/4

                    qDebug() << "payload:" << payload;
                    qDebug() << "responseTopic:" << responseTopic;

                    mqttclient->publish(responseTopic, payload);
                }
            }
            else if (jsonMethod == "beepStatus")
            {
                qDebug() << "beepStatus";
                // 回复主题消息， 返回beepStatus的值 , 服务器是thingsboard的规则引擎
                QJsonObject jsonObject;
                jsonObject.insert("beepStatus", ui->pushButton_beep->isChecked());
                QJsonDocument doc(jsonObject);
                QByteArray payload = doc.toJson(QJsonDocument::Compact);
                if (mqttclient != nullptr)
                {
                    // 从请求主题中解析请求 ID，例如：v1/devices/me/rpc/request/4 -> 4
                    QString topicName = topic.name();
                    QString requestId = topicName.mid(download_topic1.length());
                    QString responseTopic = upload_topic2 + requestId; // v1/devices/me/rpc/response/4

                    qDebug() << "payload:" << payload;
                    qDebug() << "responseTopic:" << responseTopic;

                    mqttclient->publish(responseTopic, payload);
                }
            }
            else if (jsonMethod == "led1Set")
            {
                qDebug() << "led1Set";
                on_pushButton_led1_clicked(jsonParams.toBool());
            }
            else if (jsonMethod == "led2Set")
            {
                qDebug() << "led2Set";
                on_pushButton_led2_clicked(jsonParams.toBool());
            }
            else if (jsonMethod == "io1Set")
            {
                qDebug() << "io1Set";
                on_pushButton_extio1_clicked(jsonParams.toBool());
            }
            else if (jsonMethod == "beepSet")
            {
                qDebug() << "beepSet";
                on_pushButton_beep_clicked(jsonParams.toBool());
            }
            else
            {
                qDebug() << "unknown method";
            }
        }
        else
        {
            qDebug() << "jsonstr is null";
        }
    }
    // 处理设备属性主题（OTA 固件信息由服务器主动下发）
    else if (topic.name() == download_topic2)
    {
        qDebug() << "Received attributes message";
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            // ThingsBoard 下发的 shared attributes 格式可能是直接的 key-value
            // 或者包含在 "shared" 对象中
            QJsonObject shared;
            if (jsonObj.contains("shared"))
            {
                shared = jsonObj.value("shared").toObject();
            }
            else
            {
                // 直接就是属性
                shared = jsonObj;
            }
            handleFirmwareAttributes(shared);
        }
    }
}

void Widget::handleFirmwareAttributes(const QJsonObject &shared)
{
    // 检查是否包含固件属性
    if (!shared.contains("fw_title") || !shared.contains("fw_version"))
    {
        return;
    }

    QString fwTitle = shared.value("fw_title").toString();
    QString fwVersion = shared.value("fw_version").toString();
    int fwSize = shared.value("fw_size").toInt(0);
    QString fwChecksum = shared.value("fw_checksum").toString();
    QString fwChecksumAlg = shared.value("fw_checksum_algorithm").toString();

    qDebug() << "OTA: Firmware attributes received via MQTT:";
    qDebug() << "  fw_title:" << fwTitle;
    qDebug() << "  fw_version:" << fwVersion;
    qDebug() << "  fw_size:" << fwSize;
    qDebug() << "  fw_checksum:" << fwChecksum;
    qDebug() << "  fw_checksum_algorithm:" << fwChecksumAlg;

    // 检查是否是新版本（避免重复下载）
    if (fwTitle == ota_currentFwTitle && fwVersion == ota_currentFwVersion)
    {
        qDebug() << "OTA: Same firmware version, skip download";
        return;
    }

    // 检查必要的属性
    if (fwTitle.isEmpty() || fwVersion.isEmpty() || fwSize <= 0 ||
        fwChecksum.isEmpty() || fwChecksumAlg.isEmpty())
    {
        qDebug() << "OTA: Incomplete firmware attributes";
        return;
    }

    // 记录当前版本
    ota_currentFwTitle = fwTitle;
    ota_currentFwVersion = fwVersion;

    // 显示 OTA 进度框
    ui->frame_ota->show();
    ui->frame_ota->raise();
    ui->progressBar_ota->setRange(0, 100);
    ui->progressBar_ota->setValue(0);
    ui->label_ota_status->setText(tr("OTA: 准备下载固件..."));

    // 获取下载目录和程序目录
    QDir appDir(QCoreApplication::applicationDirPath());
    QString downloadDir = QDir(appDir.absoluteFilePath("..")).absoluteFilePath("download");
    QString appDirPath = appDir.absolutePath();
    QString exeName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QString host = ui->comboBox_server->currentText();

    qDebug() << "OTA: Starting download...";
    qDebug() << "  Download dir:" << downloadDir;
    qDebug() << "  App dir:" << appDirPath;
    qDebug() << "  Exe name:" << exeName;

    // 开始下载
    httpOta->startDownload(host, fwTitle, fwVersion, fwSize, fwChecksum, fwChecksumAlg,
                           downloadDir, appDirPath, exeName);
}

void Widget::onOtaProgress(int percent)
{
    ui->progressBar_ota->setValue(percent);
}

void Widget::onOtaStatusMessage(const QString &msg)
{
    ui->label_ota_status->setText(msg);
}

void Widget::onOtaFinished(bool success, const QString &errorMessage)
{
    // OTA 重启退出码，MicroDesktop 检测到此退出码后自动重启程序
    const int OTA_RESTART_EXIT_CODE = 100;
    
    if (success)
    {
        ui->label_ota_status->setText(tr("OTA: 安装完成，即将重启..."));
        // 使用特殊退出码，通知 MicroDesktop 重启本程序
        qApp->exit(OTA_RESTART_EXIT_CODE);
    }
    else
    {
        ui->label_ota_status->setText(tr("OTA: 失败 - %1").arg(errorMessage));
        // 3 秒后隐藏 OTA 框
        QTimer::singleShot(3000, this, [this]() {
            ui->frame_ota->hide();
        });
        // 清除版本记录，允许重试
        ota_currentFwTitle.clear();
        ota_currentFwVersion.clear();
    }
}

void Widget::on_pushButton_open_clicked(bool checked)
{
    if(checked) // 为真， 链接服务器
    {
        // mosquitto_sub -h broker.emqx.io -p 1883 -t v1/devices/me/rpc/request/#
        mqttclient = new QMqttClient(this); // 创建1个mqtt对象
        mqttclient->setHostname(ui->comboBox_server->currentText()); // 设置服务器信息 xthings.cloud
        mqttclient->setPort(1883); // 设置服务器的端口号
        QString tocken = settings.value("Tocken/tocken").toString();
        qDebug() << "tocken:" << tocken;
        mqttclient->setUsername(tocken); // 设置用户名
        mqttclient->setPassword(""); // 设置密码
        mqttclient->setKeepAlive(60) ; // 启动保持链接

        // 设置信号处理函数
        connect(mqttclient,&QMqttClient::stateChanged,this,&Widget::mqtt_stateChanged_slot);
        connect(mqttclient,&QMqttClient::messageReceived,this,&Widget::mqtt_messageReceived_slot);
        // 链接服务器
        mqttclient->connectToHost();

    }
    else // 断开服务器的链接
    {
        if(mqttclient != nullptr)
        {
            mqttclient->disconnectFromHost() ; // 断开链接
        }

    }


}


void Widget::on_pushButton_led1_clicked(bool checked)
{
    qDebug() << "led1 clicked=" << checked;
    if (checked) // led1 on
    {
        system("echo 1 > /sys/class/leds/led1/brightness"); // 系统调用
        ui->pushButton_led1->setStyleSheet(
            "QPushButton#pushButton_led1 {"
            "  border: none;"
            "  border-image: url(:/images/led1on.png) 1 1 1 1 stretch stretch;"
            "}");
    }
    else // led1 off
    {
        system("echo 0 > /sys/class/leds/led1/brightness"); // 系统调
        ui->pushButton_led1->setStyleSheet(
            "QPushButton#pushButton_led1 {"
            "  border: none;"
            "  border-image: url(:/images/led1off.png) 1 1 1 1 stretch stretch;"
            "}");
    }
}

void Widget::on_pushButton_led2_clicked(bool checked)
{
    qDebug() << "led2 clicked=" << checked;
    if (checked) // led2 on
    {
        system("echo 1 > /sys/class/leds/led2/brightness"); // 系统调用
        ui->pushButton_led2->setStyleSheet(
            "QPushButton#pushButton_led2 {"
            "  border: none;"
            "  border-image: url(:/images/led2on.png) 1 1 1 1 stretch stretch;"
            "}");
    }
    else // led2 off
    {
        system("echo 0 > /sys/class/leds/led2/brightness"); // 系统调用
        ui->pushButton_led2->setStyleSheet(
            "QPushButton#pushButton_led2 {"
            "  border: none;"
            "  border-image: url(:/images/led2off.png) 1 1 1 1 stretch stretch;"
            "}");
    }
}

void Widget::on_pushButton_beep_clicked(bool checked)
{
    qDebug() << "beep clicked=" << checked;
    if (checked) // beep on
    {
        system("echo 1 > /sys/class/leds/beep/brightness"); // 系统调用
        ui->pushButton_beep->setStyleSheet(
            "QPushButton#pushButton_beep {"
            "  border: none;"
            "  border-image: url(:/images/beepon.png) 1 1 1 1 stretch stretch;"
            "}");
    }
    else // beep off
    {
        system("echo 0 > /sys/class/leds/beep/brightness"); // 系统调用
        ui->pushButton_beep->setStyleSheet(
            "QPushButton#pushButton_beep {"
            "  border: none;"
            "  border-image: url(:/images/beepoff.png) 1 1 1 1 stretch stretch;"
            "}");
    }
}

void Widget::on_pushButton_extio1_clicked(bool checked)
{
    qDebug() << "extio1 clicked=" << checked;
    if (checked) // extio1 on
    {
        system("echo 1 > /sys/class/leds/extio1/brightness"); // 系统调用
        ui->pushButton_extio1->setStyleSheet(
            "QPushButton#pushButton_extio1 {"
            "  border: none;"
            "  border-image: url(:/images/extio1on.png) 1 1 1 1 stretch stretch;"
            "}");
    }
    else // extio1 off
    {
        system("echo 0 > /sys/class/leds/extio1/brightness"); // 系统调用
        ui->pushButton_extio1->setStyleSheet(
            "QPushButton#pushButton_extio1 {"
            "  border: none;"
            "  border-image: url(:/images/extio1off.png) 1 1 1 1 stretch stretch;"
            "}");
    }
}


void Widget::initUi()
{
    // OTA 进度框默认隐藏
    ui->frame_ota->hide();

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

    // led1：使用样式表按按钮大小缩放背景图
    ui->pushButton_led1->setStyleSheet(
        "QPushButton#pushButton_led1 {"
        "  border: none;"
        "  border-image: url(:/images/led1off.png) 1 1 1 1 stretch stretch;"
        "}");

    // led2：使用样式表按按钮大小缩放背景图
    ui->pushButton_led2->setStyleSheet(
        "QPushButton#pushButton_led2 {"
        "  border: none;"
        "  border-image: url(:/images/led2off.png) 1 1 1 1 stretch stretch;"
        "}");

    // extio1：使用样式表按按钮大小缩放背景图
    ui->pushButton_extio1->setStyleSheet(
        "QPushButton#pushButton_extio1 {"
        "  border: none;"
        "  border-image: url(:/images/extio1off.png) 1 1 1 1 stretch stretch;"
        "}");

    // beep ：使用样式表按按钮大小缩放背景图
    ui->pushButton_beep->setStyleSheet(
        "QPushButton#pushButton_beep {"
        "  border: none;"
        "  border-image: url(:/images/beepoff.png) 1 1 1 1 stretch stretch;"
        "}");

    // temp：使用样式表按按钮大小缩放背景图
    ui->pushButton_temp->setStyleSheet(
        "QPushButton#pushButton_temp {"
        "  border: none;"
        "  border-image: url(:/images/temp.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 12px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_temp->setFont(font);

    // hum：使用样式表按按钮大小缩放背景图
    ui->pushButton_hum->setStyleSheet(
        "QPushButton#pushButton_hum {"
        "  border: none;"
        "  border-image: url(:/images/hum.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 12px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_hum->setFont(font);

    // vr：使用样式表按按钮大小缩放背景图
    ui->pushButton_vr->setStyleSheet(
        "QPushButton#pushButton_vr {"
        "  border: none;"
        "  border-image: url(:/images/vr.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 15px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_vr->setFont(font);

    // vol：使用样式表按按钮大小缩放背景图
    ui->pushButton_vol->setStyleSheet(
        "QPushButton#pushButton_vol {"
        "  border: none;"
        "  border-image: url(:/images/vol.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 15px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_vol->setFont(font);

    // cur：使用样式表按按钮大小缩放背景图
    ui->pushButton_cur->setStyleSheet(
        "QPushButton#pushButton_cur {"
        "  border: none;"
        "  border-image: url(:/images/cur.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 12px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_cur->setFont(font);

    // pw：使用样式表按按钮大小缩放背景图
    ui->pushButton_pw->setStyleSheet(
        "QPushButton#pushButton_pw {"
        "  border: none;"
        "  border-image: url(:/images/pw.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 12px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_pw->setFont(font);

    // cpu：使用样式表按按钮大小缩放背景图
    ui->pushButton_cpu->setStyleSheet(
        "QPushButton#pushButton_cpu {"
        "  border: none;"
        "  border-image: url(:/images/cpu.png) 1 1 1 1 stretch stretch;"
        "  color: white;"
        "  text-align: right;"
        "  padding-right: 12px;"
        "  padding-top: 30px;"
        "}");
    ui->pushButton_cpu->setFont(font);
    // quit：使用样式表按按钮大小缩放背景图
    ui->pushButton_quit->setStyleSheet(
        "QPushButton#pushButton_quit {"
        "  border: none;"
        "  border-image: url(:/images/quit.png) 1 1 1 1 stretch stretch;"
        "}");
}


