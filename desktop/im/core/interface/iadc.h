#ifndef IADC_H
#define IADC_H
#include "ibase.h"
class QSocketNotifier;
namespace fl_common {
class IAdc : public IBaseObejct
{
    Q_OBJECT
public:
    IAdc();

    bool open();
    void close();

Q_SIGNALS:
    void readRead(int v);

private:
    void open(bool bOpen);
    QSocketNotifier *mNotifier;
};
}
#endif // IADC_H
