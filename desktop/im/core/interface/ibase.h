#ifndef IBase_H
#define IBase_H

#include <QObject>
#include "core_global.h"
#include "ustring.h"
namespace fl_common {
class DataReload;
class CORE_LIBRARY IBase
{
public:
    explicit IBase();
    virtual ~IBase();
    virtual bool open();
    virtual void close();
    virtual bool isOpen();
    virtual QString devName();
    void loadData(DataReload *d);
    int id();
    QString name();
protected:
    int handle();
    int m_fd  = -1;
    DataReload *m_d=nullptr;
};

class CORE_LIBRARY IBaseObejct :public QObject,  public IBase {
public:
    IBaseObejct(QObject *parent =nullptr):QObject(parent),IBase(){}
};
}
#endif // IBase_H
