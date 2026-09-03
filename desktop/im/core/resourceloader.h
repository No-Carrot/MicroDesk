//全局资源文件的加载类 例 path=/usr/bin/data   获取目录下能文件名称 ：ResourceLoader::instacne()->rc("wifi/max")  返回/usr/bin/data/wifi/max.png

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H
#include <QString>
#include <QMultiMap>
#include <QFileInfo>
#include <QDir>

namespace fl_common {
class ResourceLoader
{
public:
    static  ResourceLoader *instacne();

    //递归读取目录下所有文件.限制最大1000个文件,path=xxx资源的所在目录, bReload=false,类内部使用.
    bool load(const QString &path, bool bReload = true);

    //从数据中查找读取文件 rcName 文件的名称, 返回资源的绝对路径名,不存在返回空.
    //例子:  输入rcName=max  返回xxx/max.*  输入rcName=wifi/max 返回xxx/wifi/max.*
    QString rc(const QString &rcName);

    //获当前取目录下的资源，不递归调用.
    static QList<QFileInfo> dirRc(const QString &absrDirName,
                                  QDir::Filters filter=QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);

private:
    QMultiMap<QString, QString> m_resources;
    QString m_path;
};

}

#endif // RESOURCELOADER_H
