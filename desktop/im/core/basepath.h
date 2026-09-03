//全局资源路径指定.

#ifndef BASEPATH_H
#define BASEPATH_H

#include <QObject>
#include <QString>
#include "core_global.h"

namespace fl_common {
class BasePath
{
public:
    static BasePath * instance();

    //资源路径.
    QString resourcePath();

    //配置文件路径.
    QString confPath();

    QString catchPath();

    //应用程序路径.
    QString appPath();

    //初始资源目下和配置文件名, rcPath不带/, conf配置文件名.不调用使用默认. rcPath=appPath/data/rc, conf=appPath/data/conf.txt
    void initPath(const QString &rcPath, const QString &conf);

private:
    std::string m_rcPath;
    std::string m_confPath;
};
}
#endif // BASEPATH_H
