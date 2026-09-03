#ifndef ISPI_H
#define ISPI_H
#include "ibase.h"
#include <linux/spi/spidev.h>

namespace fl_common {
class ISpiPrivate;
class ISpi : public IBaseObejct
{
public:
    ISpi();

    ~ ISpi();

    void close();
    bool open(int speedHz= 5000, int nBit = 8, int delayUsecs = 1);

    int mode();
    bool setmode(int mode);

    int maxSpeed();
    bool setMaxSpeed(int speedHz);

    int bitsPerWord();
    bool setBitsPerWord(int nbit);

    void setDelay(int delayUsecs);

    QString send(const QString &cmd);

private:
    ISpiPrivate *m_data;
};

}

#endif // ISPI_H
