#include "isimnetwork.h"
#include "serialport.h"
#include <QMap>
#include <QFile>
#include "terminal.h"
#include "conf.h"
#include "datareload.h"
#include "terminal.h"
#include <QtDebug>
#include "serialport.h"

namespace fl_common {

class ISIMNetworkPrivate{
public:
    QString cardName;
    QString bin;
    //串口相关.
    QString serialName ;
    int baudrate;
    int databits;
    int stopbit;
    int parity;
    int control;
    SerialPort serial;
    QString logs;
    bool bOnline =false;
};

ISIMNetwork::ISIMNetwork(QObject *parent) :
    IBaseObejct(parent),
    m_p(new ISIMNetworkPrivate)
{
}

ISIMNetwork::~ISIMNetwork()
{
    delete m_p;
}

bool ISIMNetwork::open()
{
     if(isOpen())
         return true;

     //close resource.
     stop();
     m_p->serial.close();

    //read config data.
    m_p->bin = m_d->data().value("bin");
    m_p->cardName = m_d->data().value("devName");
    m_p->serialName = m_d->data().value("tty");
    m_p->baudrate = m_d->data().value("baudrate").toInt();
    m_p->databits = m_d->data().value("databits").toInt();
    m_p->stopbit = m_d->data().value("stopbit").toInt();
    m_p->parity = m_d->data().value("parity").toInt();
    m_p->control = m_d->data().value("control").toInt();

    // open wwan0.
    if(!m_p->cardName.isEmpty()){
        Terminal::execCmd(QString("ifconfig %1 up").arg(m_p->cardName));
    }

    bool bStart = false;
    // open serialport if config.
    if(!m_p->cardName.isEmpty()){
        bStart = m_p->serial.config(m_p->serialName, m_p->baudrate,
                                    m_p->databits,  m_p->stopbit,
                                    m_p->parity, m_p->control);
    }

    //start process server.
    if(QFile::exists(m_p->bin)){
        qDebug() <<QString("start %1").arg(m_p->bin);
        start(m_p->bin);
        m_p->cardName.isEmpty() ? bStart = true : bStart &= true;
    }

    if(!m_p->cardName.isEmpty() && !QFile::exists(m_p->bin))
    {
        runLogChange("4g arguments config error");
    }

    return bStart;
}

void ISIMNetwork::close()
{
    m_p->serial.close();

    this->stop();

    if(!m_p->cardName.isEmpty())
    {
        Terminal::execCmd(QString("ifconfig %1 down").arg(m_p->cardName));
    }
}

bool ISIMNetwork::isOpen()
{
    bool bStart =false;
    if(!m_p->serialName.isEmpty()){
        bStart = m_p->serial.isOpen();
    }


    if(QFile::exists(m_p->bin)){
        m_p->serialName.isEmpty()?  bStart = isStart(): bStart &=isStart();
    }

    return bStart;
}

QString ISIMNetwork::exec(const QString &at)
{
    if(!m_p->serial.isOpen() ){
        return "";
    }

    return m_p->serial.execCMD(at.toUtf8());
}

bool ISIMNetwork::mayExec()
{
    return m_p->serial.isOpen();
}

bool ISIMNetwork::supportAPN()
{
    return  m_d->data().value("apn") == "1";
}

bool ISIMNetwork::setAPN(const QString &net, const QString &authentication, const QString &userName, const QString &password)
{ 
    if(!supportAPN()){
        this->processMessage("not support set apn.");
        return false;
    }

    //only fl_4g support.
    UMap<UString, UString> maps;
    maps.insert("dial_APN", net);
    maps.insert("dial_auth", authentication);
    maps.insert("dial_username", userName);
    maps.insert("dial_password", password);
    Conf set("/etc/wiet/daemon_4g.conf", " = ");
    return set.setValues(maps);
}

bool ISIMNetwork::getAPN(QString &net, QString &authentication, QString &userName, QString &password)
{
    if(!supportAPN()){
        this->processMessage("not support get apn.");
        return false;
    }

    //only fl_4g support.
    Conf set("/etc/wiet/daemon_4g.conf", " = ");
    UMap<UString, UString> maps =set.getValues();
    net = maps.value("dial_APN");
    authentication= maps.value("dial_auth");
    userName =maps.value("dial_username");
    password=  maps.value("dial_password");
    return true;
}

bool ISIMNetwork::isOnline()
{
    return m_p->bOnline;
}

void ISIMNetwork::processMessage(const QString &info)
{
    m_p->logs +=info;
    emit runLogChange(info);

    //judage the strings. quectel and  fl_4g support
    if(info.contains("adding dns")){
        m_p->logs.clear();
        m_p->bOnline = true;
        emit  onlineChange(m_p->bOnline);
    }
}

}//end namespace
