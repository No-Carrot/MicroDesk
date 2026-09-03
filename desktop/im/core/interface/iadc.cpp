#include "iadc.h"
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <QSocketNotifier>
#include "datareload.h"

namespace fl_common {
union ADC_CONVERT_EVENT{
    input_event e;
    char buf[200];
};
IAdc::IAdc():mNotifier(nullptr)
{
}
bool IAdc::open()
{
    open(true);

    m_fd  = ::open(devName().toLatin1().data(), O_RDONLY);
    if(isOpen()){
        if(mNotifier !=nullptr)
            delete mNotifier;

        mNotifier = new QSocketNotifier(handle(), QSocketNotifier::Read, this);
        connect(mNotifier, &QSocketNotifier::activated, this, [=](){
            ADC_CONVERT_EVENT data;
            ::read(this->handle(), data.buf, sizeof(input_event));
            if(data.e.type == EV_MSC){
                emit readRead(data.e.value);
            }
        });

        return true;
    }

    return false;
}

void IAdc::close()
{
    IBase::close();
    delete mNotifier;
    mNotifier = nullptr;
    this->open(false);
}

void IAdc::open(bool bOpen)
{
    //打开之前执行一条命令.
    QString cmd;
    bOpen ? cmd ="open" :cmd ="close";
    cmd=m_d->data().value(cmd);
    QString openCmd = m_d->data().value("open");
    if(!openCmd.isEmpty())
        system(openCmd.toLatin1().data());
}
}
