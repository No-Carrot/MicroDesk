#include "dispbacklight.h"
#include <sys/ioctl.h>
#include <QDebug>

namespace fl_common {

enum {
    DISP_GET_OUTPUT_TYPE = 0x09,
    DISP_OUTPUT_TYPE_LCD    = 1,
    DISP_LCD_GET_BRIGHTNESS = 0x103,
    DISP_LCD_SET_BRIGHTNESS = 0x102
};

DispBacklight::DispBacklight(QObject *parent):IBacklight(parent)
{
}

bool DispBacklight::setValue(int v)
{
    unsigned long args[3];
    int err, i;
    for(i = 0; i < 2; i++)
    {
        args[0] = i;
        if(ioctl(m_fd, DISP_GET_OUTPUT_TYPE,args) == DISP_OUTPUT_TYPE_LCD)
        {
            args[1]  = v;
            args[2]  = 0;
            err = ioctl(m_fd, DISP_LCD_SET_BRIGHTNESS, args);
            return true;
        }
    }
    return false;
}

int DispBacklight::getValue()
{
    unsigned long args[3];
    int err, i;
    int v;
    for(i = 0; i < 2; i++)
    {
        args[0] = i;
        if(ioctl(m_fd, DISP_GET_OUTPUT_TYPE,args) == DISP_OUTPUT_TYPE_LCD)
        {
            args[1]  = (unsigned long)&v;
            args[2]  = 0;
            err = ioctl(m_fd, DISP_LCD_GET_BRIGHTNESS, args);
            return v;
        }
    }
    return -1;
}

}
