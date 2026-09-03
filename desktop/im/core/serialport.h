#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include "core_global.h"
namespace fl_common {
class  CORE_LIBRARY SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    bool config(const QString &name,
                QSerialPort::BaudRate baudrate= QSerialPort::Baud115200,
                QSerialPort::DataBits databits = QSerialPort::Data8,
                QSerialPort::StopBits stopbit= QSerialPort::OneStop,
                QSerialPort::Parity parity= NoParity,
                QSerialPort::FlowControl control = QSerialPort::NoFlowControl);

    bool config(const QString &name,
                int baudrate,
                int databits,
                int stopbit,
                int parity,
                int control=-1);

    bool config(const QVariant & settingValue);

    void writeDatas(const QByteArray &arry);

    QByteArray execCMD(const QByteArray &arry);

    QByteArray readDatas();

};
}
#endif // SERIALPORT_H
