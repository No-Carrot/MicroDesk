#include "iwifi.h"
#include "terminal.h"
namespace fl_common {
IWifi::IWifi(QObject *parent) : IBaseIO(parent)
{

}

bool IWifi::open()
{
    return false;
}

void IWifi::close()
{

}

void IWifi::link(const QString &wifiName, const QString &wifiPassword, int id)
{
    Q_UNUSED(wifiName)
    Q_UNUSED(wifiPassword)
    Q_UNUSED(id)
}

void IWifi::disLink()
{

}
}
