#include "basepath.h"
#include <QGlobalStatic>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
namespace fl_common {
Q_GLOBAL_STATIC(BasePath, s_instance)

QString BasePath::resourcePath()
{
    QString path = QString::fromStdString(m_rcPath);
    if(path.isEmpty()){
        path = appPath() +"/data/rc";
    }

    return path;
}

QString BasePath::confPath()
{
    QString path = QString::fromStdString(m_confPath);
    if(path.isEmpty()){
        path = appPath() +"/data/conf.txt";
    }
    return path;
}

QString BasePath::catchPath()
{
   return  appPath() +"/data/cache";

}

QString BasePath::appPath()
{
    return qApp->applicationDirPath();
}

void BasePath::initPath(const QString &rcPath, const QString &conf)
{
    m_rcPath = rcPath.toStdString();
    m_confPath = conf.toStdString();
}

BasePath *BasePath::instance()
{
    return s_instance;
}

}
