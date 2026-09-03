#include "settings.h"
#include "basepath.h"
#include <QGlobalStatic>
namespace fl_common {
Q_GLOBAL_STATIC(Settings, s_instance)

Settings::Settings():
    Conf(BasePath::instance()->confPath(), "=", "[")
{

}

Settings *Settings::instance()
{
    return s_instance;
}

QStringList Settings::valuetoList(const QString &value)
{
    QStringList list = value.split(";");

    return list;
}
}
