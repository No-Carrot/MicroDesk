#ifndef DISPBACKLIGHT_H
#define DISPBACKLIGHT_H
#include "core_global.h"
#include "ibacklight.h"

namespace fl_common {
class CORE_LIBRARY DispBacklight : public IBacklight
{
public:
    DispBacklight(QObject *parent =nullptr);
    bool setValue(int v) override;
    int getValue() override;
};
}
#endif // DISPBACKLIGHT_H
