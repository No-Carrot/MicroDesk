#ifndef DATARELOAD_H
#define DATARELOAD_H

#include <QObject>
#include "conf.h"
#include "ustring.h"
namespace fl_common {
class DataReload
{
public:
    explicit DataReload(int id, const UMap<UString, UString>&  me);
    UMap<UString, UString>  data();
    int id();
private:
    int mId;
    UMap<UString, UString> mLocal;
};
}

#endif // DATARELOAD_H
