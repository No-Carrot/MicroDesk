#include "ibacklight.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "conf.h"
#include "terminal.h"
#include "datareload.h"

namespace fl_common {

IBacklight::IBacklight( QObject *parent)
    :IBaseObejct(parent)
{

}

bool IBacklight::open()
{
    if(isOpen())
        return true;

    m_fd = ::open(devName().toLatin1().data(), O_RDWR|O_NONBLOCK);
    return m_fd >0;
}

bool IBacklight::setValue(int v)
{
    if(isOpen())
    {
        char buf[20];
        sprintf(buf,"%d",v);
        ::write(m_fd,buf,sizeof(buf));

        return true;
    }
    return false;
}

int IBacklight::getValue()
{
    int ret = -1;
      if(isOpen()){
          char buf[10];
          lseek(m_fd, 0, SEEK_SET);
          ::read(m_fd, buf, sizeof(buf));
          ret = atoi(buf);
      }
      return ret;
}

void IBacklight::getRange(int &min, int &max)
{
  static int minV=0 , maxV=0;
  if(minV == maxV){
      minV = m_d->data().value("min").toInt();
      maxV = m_d->data().value("max").toInt();
  }
  min = minV;
  max = maxV;
}

}
