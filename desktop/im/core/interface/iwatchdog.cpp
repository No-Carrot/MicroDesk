#include "iwatchdog.h"
#include <QTimer>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <QThread>
#include <QDebug>
namespace fl_common {

IWatchDog::IWatchDog(QObject *parent):QThread(parent)
{
}

IWatchDog::~IWatchDog()
{
    this->close();
}

bool IWatchDog::open()
{
    this->start(QThread::HighestPriority);
    return true;
}

void IWatchDog::close()
{
    this->quit();
    this->wait();
}

bool IWatchDog::isOpen()
{
    return isRunning();
}

void IWatchDog::run()
{
    int fd = ::open(devName().toLocal8Bit().data(),  O_WRONLY);
    if(fd <0){
        return;
    }

    int timeoutSeconds = 6;
    int timeoutSeconds2 =6;
    int timerFeedDogIntervalMSeconds = 1000;
    int count=0;

    auto setTimeoutSconds = [=, &fd](int sec){ ioctl(fd,WDIOC_SETTIMEOUT,&sec);};
    auto keepalive = [=,&count, &fd](){int dummy; ::ioctl(fd, WDIOC_KEEPALIVE, &dummy); emit feedCount(++count);};

    QTimer feedTimer;
    bool bEableFeedDog =true;
    feedTimer.setTimerType(Qt::PreciseTimer);
    connect(&feedTimer, &QTimer::timeout, this, [=, &bEableFeedDog](){
        if(bEableFeedDog)
        keepalive();
    });

    setTimeoutSconds(timeoutSeconds);
    keepalive();
    feedTimer.start(timerFeedDogIntervalMSeconds);

    //set enable.
    connect(this, &IWatchDog::setFeedEnable, [=,&bEableFeedDog](bool bEnable){bEableFeedDog = bEnable;});

    //set timeout.
    connect(this, &IWatchDog::setTimeout, [=, &timeoutSeconds2](int seconds){
        if(seconds*1000 > timerFeedDogIntervalMSeconds)
              timeoutSeconds2 = seconds;
    });

    //set timer interval.
    connect(this, &IWatchDog::setFeeddogInterval, [=, &timerFeedDogIntervalMSeconds, &feedTimer](int invervalSecs){
        if(timeoutSeconds2 >invervalSecs)
             timerFeedDogIntervalMSeconds = invervalSecs*1000;
    });


    QTimer loopTimer;
    connect(&loopTimer, &QTimer::timeout, [=,&timerFeedDogIntervalMSeconds, &bEableFeedDog, &timeoutSeconds, &timeoutSeconds2, &feedTimer](){
        if(timerFeedDogIntervalMSeconds != feedTimer.interval()){
            feedTimer.setInterval(timerFeedDogIntervalMSeconds);
        }else if(bEableFeedDog != feedTimer.isActive()){
            bEableFeedDog ? feedTimer.start() : feedTimer.stop();
        }else if(timeoutSeconds2 != timeoutSeconds){
            setTimeoutSconds(timeoutSeconds=timeoutSeconds2);
        }
    });
    loopTimer.start(10);
    exec();

    //close.
    int flags = WDIOS_DISABLECARD;
    ioctl(fd, WDIOC_SETOPTIONS, &flags);
    ::close(fd);
}
}
