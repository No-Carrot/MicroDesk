#include "conf.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include<QStringList>
#include <QDebug>
#include "inetwork.h"
#include "terminal.h"
namespace fl_common {
class Section{
public:
    QString match;  //保存  分组的头信息.
    QList<QStringList> rowListCut; //保存行信息，按照cut 分割报错.
};

void Conf::clearn(){
    foreach (Section *sec, mGroups){
        delete sec;
    }
    mMaps.clear();
    mGroups.clear();
}
QString Conf::fileName(){return mFileName; }
QString Conf::cutName(){return mCutName;}
QString Conf::matchKey(){return mMatchKey;}

QList<UMap<UString, UString> > Conf::getSections(const QString &sectionName)
{
    QList<UMap<UString, UString> > results;

    if(isEmpty())
        this->readSections();

    foreach (Section *section, mGroups){
        if(isEqual(section->match, sectionName)){
            UMap<UString, UString> map;
            foreach (QStringList list, section->rowListCut) {
                QString key;
                QString value;
                if(toPira(list, key, value)) {
                    map.insert(key, value);
                }
            }
            results << map;

        }
    }

    return results;
}

bool Conf::appendSection(const QString &sectionName, const UMap<UString, UString> &maps)
{
    if(isEmpty()){
        readSections();
    }

    Section *section = createSection(sectionName, maps);

    section->rowListCut.clear();
    UMap<UString, UString>::const_iterator iter;
    for(iter=maps.begin();iter!=maps.end(); ++iter){
        QStringList list;
        list <<iter.key() <<iter.value();
        section ->rowListCut <<list;
    }

    return writeSection();
}

Conf::Conf(const QString &fileName, const QString &cut, const QString &matchKey)
    :mFileName(fileName),
      mCutName(cut),
      mMatchKey(matchKey)
{
    if(mMatchKey.isEmpty())
    {
        mMatchKey="<!!!!!!!!!!!!$%$%$%$%$%^^%^&********!!!!+++!!!!!!!!!%%%%%%%%%%@@@@@@defalut!!!+++++!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>";
    }
}

bool Conf::setValues(const UMap<UString, UString> &maps)
{
    if(mMaps.isEmpty())
        this->readSections();

    UMap<UString, UString>::const_iterator iter;
    for(iter = maps.begin(); iter != maps.end(); ++iter){
        mMaps.insert(iter.key().trimmed(), iter.value());
    }

    return writeSection();

}

UMap<UString, UString> Conf::getValues()
{
    if(mMaps.isEmpty())
        this->readSections();

    return mMaps;
}

bool Conf::setValue(const QString &key, const QString &value)
{
    if(mMaps.isEmpty())
        this->readSections();

    mMaps.insert(key, value);

    this->writeSection();

    return true;
}

QString Conf::getValue(const QString &key, const QString &defauValue)
{
    if(mMaps.isEmpty())
        this->readSections();

    QString ret =mMaps.value(key);
    if(ret.isEmpty()){
        ret = defauValue;
    }
    return ret;
}

void  Conf::readSections(){
    clearn();
    QList<QString> infos;
    QFile file(fileName());
    if(!file.open(QFile::ReadOnly| QFile::Text))
    {
        qDebug()<<QString("read conf file falire: %1").arg(fileName());
        return ;
    }

    QString line;
    QTextStream stream(&file);
    while (stream.readLineInto(&line) ) {
        infos<< line.trimmed();
    }
    file.close();


    Section  *section = nullptr;
    QString cut = this->cutName();
    foreach (QString row, infos) {
        QString rowData = comments(row);
        if(rowData.left(matchKey().size()) == matchKey()){
            section = new Section;
            section->match = rowData;
            mGroups << section;
        }else if(section){
            QStringList list = rowData.split(cut);
            list.removeAll("");
            section->rowListCut << list;
        }

        // 起始字段保存 单独key/value
        if(section == nullptr && !rowData.isEmpty()){
            QStringList list = rowData.split(cut);
            QString key;
            QString value;
            if(toPira(list, key, value)){
                mMaps.insert(key,  value);
            }
        }
    }
}

bool Conf::setSection(const QString &sectionName, const QString &key, const QString &value)
{
    UMap<UString, UString> map;
    map.insert(key, value);
    return setSection(sectionName, map, false);
}

UMap<UString, UString> Conf::getSection(const QString &sectionName)
{
    UMap<UString, UString> result;


    if(isEmpty())
        this->readSections();

    if( Section *section = findSection(sectionName))
    {
        foreach (QStringList list, section->rowListCut) {
            QString key;
            QString value;
            if(toPira(list, key, value))
            {
                result.insert(key, value);
            }
        }
    }

    return result;
}

QString Conf::getSection(QString &key, const QString &sectionName)
{
    return  getSection(sectionName).value(key);
}

bool Conf::removeSection(const QString &sectionName)
{
    bool bRet = false;
    if(isEmpty()){
        this->readSections();
    }

    Section *pSection = findSection(sectionName);
    if(pSection)
    {
        mGroups.removeOne(pSection);
        delete pSection;
        bRet = true;
    }

    return bRet;
}

Section * Conf::createSection(const QString &sectionName, const UMap<UString, UString> &maps)
{
    if(sectionName.left(matchKey().size()) != matchKey() )
        return nullptr;

    Section *section = new Section;
    section->match =sectionName;


    UMap<UString, UString>::const_iterator iter;
    for(iter= maps.begin(); iter!=maps.end(); ++iter)
    {
        QStringList l;
        l <<iter.key() <<iter.value();
        section->rowListCut <<l;
    }

    mGroups <<section;
    return section;
}

Section * Conf::findSection(const QString &sectionName)
{
    Section *sec = nullptr;
    foreach (Section *section, mGroups){
        if(isEqual(section->match, sectionName)){
            sec = section;
            break;
        }
    }

    return sec;
}

bool Conf::isEmpty(){
    return mGroups.isEmpty() && mMaps.isEmpty();
}

bool Conf::setSection(const QString &sectionName, const UMap<UString, UString> &maps, bool bOverride)
{
    if(isEmpty()){
        readSections();
    }

    bool bFind = true;
    Section *section = findSection(sectionName);
    if(section == nullptr){
        section = createSection(sectionName, maps);
        bFind = false;
    }
    if(section == nullptr)
        return false;

    if(isEmpty())
        return false;

    if(!bOverride)
    {
        QStringList keys;
        //exits modify.
        for(int i=0; i<section->rowListCut.size(); i++){
            QStringList list = section->rowListCut[i];
            if(list.isEmpty())
                continue;

            QString key = list[0];
            keys <<key;
            if(maps.keys().contains(key)){
                QString value = maps.value(key);
                QStringList l;
                l <<key <<value;
                section->rowListCut[i] = l;
            }
        }


        //not exit add.
        UMap<UString, UString>::const_iterator iter;
        for(iter=maps.begin();iter!=maps.end(); ++iter){
            //            qDebug()<<iter.key() <<"/" <<keys;
            if(!keys.contains(iter.key())){
                QStringList l;
                l <<iter.key() <<iter.value();
                section->rowListCut.append(l);
            }
        }

    }else{
        if(bFind) {
            section->rowListCut.clear();
            UMap<UString, UString>::const_iterator iter;
            for(iter=maps.begin();iter!=maps.end(); ++iter){
                QStringList list;
                list <<iter.key() <<iter.value();
                section ->rowListCut <<list;
            }
        }

    }


    return writeSection();
}

bool Conf::writeSection()
{
    if(isEmpty())
        return false;

    QFile file(fileName());
    if(!file.open(QFile::WriteOnly| QFile::Text | QFile::Truncate))
    {
        qDebug()<<QString("writeSection %1 falre");
        return false;
    }

    QString data;
    QString cut = this->cutName();


    //保存单独的key/value 到文件的开头.
    UMap<UString, UString>::iterator iter;
    for(iter= mMaps.begin(); iter != mMaps.end(); ++iter)
    {
        data += iter.key() + cut + iter.value() +"\n";
    }

    //保存section 组
    foreach (Section *section, mGroups) {
        data += section->match +" \n";
        foreach (QStringList list, section->rowListCut) {
            list.removeAll("");
            QString row ;
            foreach (QString str, list) {
                row +=str +cut;
            }
            if(!row.isEmpty()){
                row = row.left(row.size()-cut.size());
            }

            data += row +"\n";
        }
    }

    int ret = file.write(data.toUtf8());

    file.close();

    return ret != -1;
}

bool Conf::isEqual(const QString &a, const QString &b)
{
    QString source = a;
    QString dest = b;
    source = source.remove(" ");
    dest = dest.remove(" ");
    return source == dest;
}

QString Conf::comments(const QString &row){
    QString result = row;
    int index = result.indexOf("#");
    if(index == 0)
        result.clear();
    if(index >0)
        result = result.left(index +1);
    return result;
}

bool Conf::toPira(const QStringList &list, QString &key, QString &value)
{
    bool bOk = false;
    for (int i=1; i<list.size(); i++) {
        value +=list[i] + cutName();
    }
    if(!value.trimmed().isEmpty()){
        value = value.left(value.size()- cutName().size());
    }

    if(list.size() > 0 ){
        key = list[0];
        bOk = true;
    }

    return bOk;
}

}
