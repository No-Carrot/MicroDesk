#ifndef IWATCHDOG_H
#define IWATCHDOG_H
#include "core_global.h"
#include "ibase.h"
#include <QThread>

class QTimer;
namespace fl_common {
class Watchdog;
class CORE_LIBRARY IWatchDog : public QThread,
                               public IBase
{
    Q_OBJECT
public:
    IWatchDog( QObject *parent = Q_NULLPTR);
    ~IWatchDog();
    bool open();
    void close();
    bool isOpen();

Q_SIGNALS:
    void setFeeddogInterval(int intervalSecs =1);
    void setTimeout(int secs = 5);
    void setFeedEnable(bool bEnable =true);
    void feedCount(int count);

protected:
    void run();
};

}
#endif // IWATCHDOG_H
