#ifndef INETWORK_H
#define INETWORK_H
#include<QString>
#include "core_global.h"
#include "ibase.h"
#include "processhandler.h"

namespace fl_common {
class  CORE_LIBRARY INetwork : public IBaseObejct, protected ProcessHandler
{
    Q_OBJECT
public:
    virtual ~INetwork();
    virtual void setAuto(const QString &cardname)=0;

    virtual void setStatic(const QString &cardname, const QString &ip, const QString &mask, const QString &gateway, const QString &dns)=0;

    virtual bool exec(int *state =nullptr) =0;

    virtual void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList &dns, bool &bAuto) = 0;

    virtual QStringList  availableCard();

    static QString gateway();

    static void getNetwork(const QString &cardName, QString &ip, QString &mask, QString &broadCast, QStringList &dns);
    static QString broadCast(const QString &ip, const QString &mask);

    static QString getMaskBits(const QString &mask);

    static bool isRoot();

    static QStringList getdns();

    static QStringList getIPs(const QString &data);

    static bool isUp(const QString &cardName);

    static bool setUp(const QString &cardName, bool bUp);

    static bool isLinkUp(const QString &cardName);

Q_SIGNALS:
    void runLogChange(const QString &log);

protected:
    void processMessage(const QString &info);
};

class CORE_LIBRARY  NetworkingServer : public INetwork
{
public:
    virtual void setAuto(const QString &cardName);
    virtual void setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns);

    bool exec(int *state =nullptr);

    void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList &dns, bool &bAuto);

private:
    bool isRunning();
protected:
    virtual void creatFile();
};

class CORE_LIBRARY ManualMangeNetwork: public NetworkingServer
{
public:
    virtual void setAuto(const QString &cardName);
    virtual void setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns);

    bool exec(int *state =nullptr);

    void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList & dns, bool &bAuto);
};

class CORE_LIBRARY  SystemdNetworkServer: public NetworkingServer
{
public:
    virtual void setAuto(const QString &cardname);
    virtual void setStatic(const QString &cardName, const QString &ip, const QString &mask, const QString &gateway, const QString &dns);

    bool exec(int *state =nullptr);

    void readConf(const QString &cardName, QString &ip, QString &mask,  QString &gateway, QStringList &dns, bool &bAuto);

private:
    bool isRunning(const QString &serverName);
    void creatFile() override;
};
}

#endif //INETWORK_H

