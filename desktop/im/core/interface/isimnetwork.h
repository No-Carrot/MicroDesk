#ifndef ISIMNETWORK_H
#define ISIMNETWORK_H

#include <QObject>
#include "core_global.h"
#include "processhandler.h"
#include "ibase.h"
#include <QMap>

namespace fl_common {
class ISIMNetworkPrivate;
class CORE_LIBRARY ISIMNetwork : public IBaseObejct,  public ProcessHandler
{
    Q_OBJECT
public:
    explicit ISIMNetwork(QObject *parent = nullptr);
    ~ISIMNetwork();

    bool open();
    void close();
    bool isOpen();

    virtual bool mayExec();
    virtual QString exec(const QString &at);

    virtual bool supportAPN();
    virtual bool setAPN(const QString & net, const QString &authentication, const QString &userName, const QString &password);
    virtual bool getAPN(QString & net, QString &authentication,  QString &userName,  QString &password);
    virtual bool isOnline();

Q_SIGNALS:
    void runLogChange(const QString &log);
    void onlineChange(bool bOnline);

protected:
    void processMessage(const QString &info) ;
    ISIMNetworkPrivate *m_p;
};


}//end namespace
#endif // ISIMNETWORK_H
