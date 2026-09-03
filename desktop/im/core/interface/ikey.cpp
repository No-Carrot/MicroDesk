#include "ikey.h"
#include <fcntl.h>
#include <unistd.h>
#include <QSocketNotifier>
#include <linux/input.h>
#include <QMap>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include "datareload.h"
#include "settings.h"

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
namespace fl_common {
IKey::IKey(QObject *parent) :
    IBaseObejct(parent)
{
}

bool IKey::open()
{
    if(isOpen())
        return true;

    m_fd = ::open(devName().toLatin1().data(), O_RDONLY);
    QSocketNotifier* notifer = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect(notifer, &QSocketNotifier::activated, this, [=](){
        struct input_event event;
        int ret = ::read(m_fd, &event, sizeof(event));
        if (ret !=  sizeof(event))
            return;
        int code = event.code;
        int value = event.value;
        emit keyPress(code, value);
    });

    return m_fd > 0;
}

QList<int> IKey::keyCodes()
{
    static QList<int> resulst;
    if(resulst.isEmpty()){
        QString value= m_d->data().value("values");
        QStringList list =Settings::instance()->valuetoList(value);

        foreach (QString v, list) {
            int code = v.trimmed().toInt();
            resulst <<code;
        }
    }

    return resulst;
}

QList<QString> IKey::keyNames()
{
    static QList<QString> resulst;
    if(resulst.isEmpty())
    {
        QString value= m_d->data().value("names");
        QStringList list =Settings::instance()->valuetoList(value);
        foreach (QString v, list) {
            resulst <<v;
        }
    }
    return resulst;
}

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

static int is_event_device(const struct dirent *dir) {
    return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}
static char * scan_devices(const QString &scan){
    struct dirent **namelist;
    int i, ndev, devnum;
    char *filename;
    int max_device = 0;

    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, alphasort);
    if (ndev <= 0)
        return NULL;

    fprintf(stderr, "Available devices:\n");

    for (i = 0; i < ndev; i++)
    {
        char fname[64];
        int fd = -1;
        char name[256] = "???";

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
        fd = ::open(fname, O_RDONLY);
        if (fd < 0)
            continue;
        ::ioctl(fd, EVIOCGNAME(sizeof(name)), name);
        ::close(fd);
        if (strncmp(name, scan.toLocal8Bit().data(), scan.size()) != 0)
            continue;

        fprintf(stderr, "%s:    %s\n", fname, name);

        sscanf(namelist[i]->d_name, "event%d", &devnum);
        if (devnum > max_device)
            max_device = devnum;

        ::free(namelist[i]);
    }

    if (devnum > max_device || devnum < 0)
        return NULL;

    asprintf(&filename, "%s/%s%d",
             DEV_INPUT_EVENT, EVENT_DEV_NAME,
             devnum);

    return filename;
};

QString IKey::devName()
{
    QString name = IBaseObejct::devName();
    QString autoMatch=m_d->data().value("match");
    if(!autoMatch.isEmpty()){
        name = QString(scan_devices(autoMatch));
    }
    return name;
}
}
