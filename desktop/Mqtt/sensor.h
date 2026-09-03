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
#include <QFont>
#include <QSettings>
#include <QtMqtt/QMqttClient>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "http_ota.h"







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

    // 上行主题 - 设备属性上报
    const QString upload_topic1 = "v1/devices/me/telemetry"   ;
    // 上行主题 - RPC响应
    const QString upload_topic2 = "v1/devices/me/rpc/response/" ;

     // 下行主题 - RPC请求
    const QString download_topic1 = "v1/devices/me/rpc/request/"   ;
     // 下行主题 - 设备属性订阅（OTA 固件信息）
    const QString download_topic2 = "v1/devices/me/attributes";


    void initUi(); // 初始化窗口的函数
    void mqtt_stateChanged_slot(QMqttClient::ClientState state);
    void mqtt_messageReceived_slot(const QByteArray &message, const QMqttTopicName &topic);
  



private slots:

    void on_pushButton_quit_clicked();

    // 定时器超时槽函数
    void timer_timeout();

    void on_pushButton_open_clicked(bool checked);

    void on_pushButton_led1_clicked(bool checked);

    void on_pushButton_led2_clicked(bool checked);

    void on_pushButton_extio1_clicked(bool checked);

    void on_pushButton_beep_clicked(bool checked);

    // OTA 槽函数
    void onOtaProgress(int percent);
    void onOtaStatusMessage(const QString &msg);
    void onOtaFinished(bool success, const QString &errorMessage);

private:
    void handleFirmwareAttributes(const QJsonObject &shared);

    Ui::Widget *ui;
    QTimer *timer;
    QSettings settings;
    QMqttClient *mqttclient;
    HttpOta *httpOta;

    // OTA 状态
    QString ota_currentFwTitle;
    QString ota_currentFwVersion;
};
#endif // SENSOR__H
