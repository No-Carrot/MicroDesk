//配置文件的读写

#ifndef CONFREADWRITE_H
#define CONFREADWRITE_H
#include <QString>
#include <QMap>
#include "ustring.h"
#include "core_global.h"
namespace fl_common {
class Section;

class CORE_LIBRARY Conf
{
public:
    //fileName修改的文件名称    cut 分割方式（空格/等号）         matchKey组的分割名称唯一
    Conf(const QString &fileName, const QString &cut, const QString &matchKey=QString(""));

    //添加key/value 到文件开头.
    bool setValue(const QString &key, const QString &value);
    QString getValue(const QString &key, const QString &defauValue ="");

    bool setValues(const UMap<UString, UString> &maps);
    UMap<UString, UString> getValues();

    //添加section组 到文件 sectionName唯一的表示组的名字例子    . bOverride=false, 值对添加字段进行修改，其它字段保留.
    bool setSection( const QString &sectionName, const UMap<UString, UString> &maps, bool bOverride = true);
    //如果同名存在只能读取第一个.
    UMap<UString, UString> getSection(const QString &sectionName);

    //设置section 组
    bool setSection(const QString &sectionName, const QString &key, const QString &value);
    QString getSection( QString &key, const QString &sectionName);
    bool removeSection(const QString &sectionName);

    //读取多个同名的section
    QList<UMap<UString, UString>> getSections(const QString &sectionName);
    //追加section
    bool appendSection( const QString &sectionName, const UMap<UString, UString> &maps);

    QString cutName();
    QString fileName();
    QString matchKey();

protected:
    bool isEmpty();
    void clearn();
    void readSections();
    QString comments(const QString &row);
    bool  toPira(const QStringList &list,  QString &key, QString &value);
    Section * createSection(const QString &sectionName, const UMap<UString, UString> &maps);
    Section * findSection(const QString &sectionName);
    bool writeSection();
    bool  isEqual(const QString &a, const QString &b);

    QList<Section*> mGroups;
    UMap<UString, UString> mMaps;
    QString mFileName;
    QString mCutName;
    QString mMatchKey;
};
}
#endif // CONFREADWRITE_H
