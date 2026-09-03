#include "ispi.h"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <QDebug>
#include "conf.h"
#include <QDir>
#include "terminal.h"
#include "datareload.h"
namespace fl_common {
class ISpiPrivate{
public:
    int speedHz= 5000;
    int nBit = 8;
    int delayUsecs = 1;
    QString name;
    QString devname;

    static QList<QString> names;
    static QList<QString> abuoutNames;

};

ISpi::ISpi():m_data(new ISpiPrivate)
{
}

ISpi::~ISpi()
{
    delete m_data;
}

bool ISpi::open(int speedHz, int nBit, int delayUsecs)
{
    if(devName().isEmpty())
        return false;

    m_fd = ::open(devName().toLatin1().data(), O_RDWR);

    this->setDelay(delayUsecs);

    if( !(setmode(0) && setMaxSpeed(speedHz) && setBitsPerWord(nBit))){
        close();
        return false;
    }

    return m_fd > 0;
}
void ISpi::close()
{
    IBase::close();
}

bool ISpi::setmode(int mode)
{
    if(!isOpen())
        return false;

    int m = this->mode();
    if(m < 0){
        return false;
    }

    m|=mode;
    if(::ioctl(handle(),SPI_IOC_WR_MODE,&mode)<0){
        qDebug()<<"write mode falire";
        return false;
    }

    return true;
}

int ISpi::mode()
{
    if(!isOpen())
        return -1;

    int m;
    if(::ioctl(this->handle(), SPI_IOC_RD_MODE, &m) <0){
        qDebug()<<QString("read mode falire: %1").arg(m);
        return -1;
    }

    return m;
}

bool ISpi::setMaxSpeed(int speedHz)
{
    if(!isOpen())
        return false;

    m_data->speedHz = speedHz;
    if(::ioctl(this->handle(),SPI_IOC_WR_MAX_SPEED_HZ,&speedHz) <0)
    {
        qDebug()<<QString("set  speed falire: %1").arg(speedHz);
        return false;
    }
    return true;
}

int ISpi::maxSpeed()
{
    if(!isOpen())
        return false;

    int speedHz;
    if(::ioctl(this->handle(),SPI_IOC_RD_MAX_SPEED_HZ,&speedHz) < 0){
        return -1;
    }
    m_data->speedHz = speedHz;
    return speedHz;
}

bool ISpi::setBitsPerWord(int nbit)
{
    if(!isOpen())
        return false;

    m_data->nBit = nbit;
    if(::ioctl(handle(),SPI_IOC_WR_BITS_PER_WORD,&nbit) <0){
        qDebug()<<QString("set  bit falire: %1").arg(nbit);
        return false;
    }

    return true;
}

int ISpi::bitsPerWord()
{
    if(!isOpen())
        return false;

    int nbit;
    if(::ioctl(handle(),SPI_IOC_RD_BITS_PER_WORD,&nbit)<0)
        return -1;

    m_data->nBit = nbit;
    return nbit;
}

void ISpi::setDelay(int delayUsecs)
{
    m_data->delayUsecs = delayUsecs;
}

QString ISpi::send(const QString &cmd)
{
    std::string str = cmd.toStdString();
    struct spi_ioc_transfer tr;
    char buf[cmd.length()+1] = {0};
    memset(buf,0,cmd.length()+1);

    memset(&tr,0,sizeof(tr));
    tr.tx_buf = (__u64)cmd.toLatin1().constData();
    tr.rx_buf = (__u64)buf;
    tr.len = str.length();

    tr.delay_usecs = m_data->delayUsecs;
    tr.speed_hz = m_data->speedHz;
    tr.bits_per_word = m_data->nBit;
    if(::ioctl(this->handle(),SPI_IOC_MESSAGE(1),&tr)<0){
        qDebug()<<"SPI_IOC_MESSAGE send faire!";
    }

    return QString(buf);
}
}
