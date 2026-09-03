#ifndef IBACKLIGHT_H
#define IBACKLIGHT_H

#include <QObject>
#include "core_global.h"
#include "ibase.h"

namespace fl_common {
class  CORE_LIBRARY IBacklight : public IBaseObejct
{
public:
    IBacklight(QObject *parent =nullptr);

    bool open();
    virtual bool setValue(int v) ;
    virtual int getValue();
    void getRange(int &min, int &max);
};
}

#endif // IBACKLIGHT_H
