//map 使用QString不进行排序. UMap<UString, xxxx> map; map.insert("aa", xxx); map.insert("cc", xxx); map.insert("bb", xxx);  -> aa cc bb

#ifndef USTRING_H
#define USTRING_H
#include <QString>
#include "core_global.h"
#include <QMap>
namespace fl_common {
//使用Umap 排序时使用插入循序，
class CORE_LIBRARY UString : public QString
{
public:
    UString():QString(){} //1

    UString(const char *s):QString(s){}//2

    UString(const QString &s): QString(s){}//3

    UString(QLatin1String latin1):QString(latin1){}//4

    UString(const QChar *unicode, int size):QString(unicode, size){}//5

    UString(QChar c):QString(c){}//6

    UString(int size, QChar c):QString(size, c){}//7

    UString(const QByteArray &ba):QString(ba){}//8

    bool operator <(const UString & s) const{
        if(s.toUtf8() == this->toUtf8())  //去除相同的key值
            return false;

        return true;  //不进行排序处理
    }

    bool operator ==(const QString &s){return s == this->toLatin1().data();}
};

//使用使用默认读取value 读取不正常，重写value 函数读取值..
template<class Key, class T>
class CORE_LIBRARY UMap:  public QMap<Key, T>
{
public:
    const T value(const Key &key, const T &adefaultValue=T())const{
        QList<Key> keys= this->keys();
        QList<T> values= this->values();
        int index = keys.indexOf(key);
        if(index != -1){
            return values.at(index);
        }
        return adefaultValue;
    }

};
}
#endif // USTRING_H

