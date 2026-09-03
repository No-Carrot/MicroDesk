#include "resourceloader.h"
#include <QGlobalStatic>
#include "basepath.h"
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>
namespace fl_common {
Q_GLOBAL_STATIC(ResourceLoader, s_instance)

ResourceLoader *ResourceLoader::instacne()
{
    return s_instance;
}

bool ResourceLoader::load(const QString &path, bool bReload)
{
    if(bReload){
        m_path = path;
        m_resources.clear();
    }

    foreach (QFileInfo info, dirRc(path)) {
        if(m_resources.count()<1000){
            if(info.isFile()){
                m_resources.insert(info.baseName(), info.absoluteFilePath());
            }else if(info.isDir()){
                load(info.absoluteFilePath(), false);
            }
        }
    }

    return !m_resources.isEmpty();

}

QString ResourceLoader::rc(const QString &rcName)
{
    QString depth = m_path +"/" +rcName;
    QMultiMap<QString, QString>::iterator iter;
    for(iter = m_resources.begin(); iter != m_resources.end(); ++iter){
        QString value = iter.value();
        if(value.left(depth.size()) == depth ){
            value = value.right(value.size() - depth.size());
            if(!value.contains("/"))
                return  iter.value();
        }
    }

    return "";
}

QList<QFileInfo> ResourceLoader::dirRc(const QString &absrDirName,  QDir::Filters filter)
{
    QList<QFileInfo> ret;
    QDir dir(absrDirName);
    foreach (QFileInfo info, dir.entryInfoList(filter)) {
        ret << info;
    }
    return ret;
}

}
