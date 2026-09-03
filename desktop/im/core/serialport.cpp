#include "serialport.h"
#include <QVariant>
#include <QDebug>
namespace fl_common {
SerialPort::SerialPort(QObject *parent) : QSerialPort(parent)
{
}

bool SerialPort::config(const QString &name,
                        QSerialPort::BaudRate baudrate,
                        QSerialPort::DataBits databits,
                        QSerialPort::StopBits stopbit,
                        QSerialPort::Parity parity, FlowControl control)
{
    this->setPortName(name.trimmed());
    this->setBaudRate(baudrate);
    this->setDataBits(databits);
    this->setStopBits(stopbit);
    this->setParity(parity);
    this->setFlowControl(control);

    bool bOpen = this->open(QIODevice::ReadWrite);

    if(!bOpen){
        qDebug() << QString("open serial %1 falire: %2 ").arg(name).arg(this->errorString());
    }

    return bOpen;
}

bool SerialPort::config(const QString &name, int baudrate, int databits, int stopbit, int parity, int control)
{
    QSerialPort::BaudRate b = (QSerialPort::BaudRate)baudrate;

    QSerialPort::DataBits d = (QSerialPort::DataBits)databits;

    QSerialPort::StopBits  s = (QSerialPort::StopBits)stopbit;

    QSerialPort::Parity p = (QSerialPort::Parity)parity;
    QSerialPort::FlowControl c;
    if(control == -1)
    {
        c = QSerialPort::NoFlowControl;
    }else{
        c = (QSerialPort::FlowControl)control;
    }

    return this->config(name, b,d, s, p, c);
}

bool SerialPort::config(const QVariant &settingValue)
{
    QVariantList list =  settingValue.toList();
    if(list.size() == 5)
    {
        return config(list[0].toString(),
                list[1].toInt(),
                list[2].toInt(),
                list[3].toInt(),
                list[4].toInt()
                ) ;
    }else if(list.size() == 6)
    {
        return config(list[0].toString(),
                list[1].toInt(),
                list[2].toInt(),
                list[3].toInt(),
                list[4].toInt(),
                list[5].toInt()
                ) ;
    }

    return false;
}

QByteArray SerialPort::execCMD(const QByteArray &arry)
{
    this->write(arry);
    this->waitForBytesWritten(200);

    QByteArray receive;
    if(waitForReadyRead(200))
    {
        receive = readAll();
        while (waitForReadyRead(10)) {
            receive += readAll();
        }
    }

    return receive;
}

void SerialPort::writeDatas(const QByteArray &arry)
{
    this->write(arry);
    this->waitForBytesWritten(200);
}

QByteArray SerialPort::readDatas()
{
    QByteArray receive;
    if(waitForReadyRead(200))
    {
        receive = readAll();
        while (waitForReadyRead(10)) {
            receive += readAll();
        }
    }

    return receive;
}
}
