#ifndef BASEFACTORY_H
#define BASEFACTORY_H

#include "core_global.h"
#include "ibacklight.h"
#include "ikey.h"
#include "inetwork.h"
#include "iwatchdog.h"
#include "isimnetwork.h"
#include "ispi.h"
#include "iadc.h"

namespace fl_common {

class BaseFactoryPrivate;
class CORE_LIBRARY  BaseFactory
{
public:
    static BaseFactory *instance();

    IKey*  key();

    IWatchDog *  watchdog();

    INetwork  *  wriednetwork();

    int backlightCount();
    IBacklight  *  backlight(int id=0);

    int simNetworkCount();
    ISIMNetwork * simNetowrk(int id=0);

    int spiCount();
    ISpi *spi(int id=0);

    int adcCount();
    IAdc *adc(int id=0);

    BaseFactory();
    ~ BaseFactory();

private:
    BaseFactoryPrivate *m_d;
};
} //end namespace

#endif // BASEFACTORY_H
