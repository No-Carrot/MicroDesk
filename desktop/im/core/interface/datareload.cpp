#include "datareload.h"
namespace fl_common {
DataReload::DataReload(int id, const UMap<UString, UString> &me):mId(id),mLocal(me){}

UMap<UString, UString> DataReload::data()
{
    return mLocal;
}

int DataReload::id()
{
    return mId;
}
}
