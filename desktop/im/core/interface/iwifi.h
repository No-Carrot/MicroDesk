#ifndef IWIFI_H
#define IWIFI_H
#include "core_global.h"
#include "ibaseio.h"
namespace fl_common {
class CORE_LIBRARY IWifi : public IBaseIO
{
    Q_OBJECT
public:
    explicit IWifi(QObject *parent = nullptr);

    virtual bool open();

    virtual void close();

    virtual void link(const QString &wifiName, const QString &wifiPassword, int id);

    virtual  void disLink();

Q_SIGNALS:
    void infoChange(const QList<QString> &name, const QList<QString> &ids, const QList<QString> &Level);
    void linkSuffcessful(const QString & wifiName, int id);

private:
    QString m_moduleName;
};
}

#endif // IWIFI_H
