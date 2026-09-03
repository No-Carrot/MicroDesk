#ifndef IKEY_H
#define IKEY_H

#include <QObject>
#include "core_global.h"
#include <QList>
#include "ibase.h"
namespace fl_common {
class CORE_LIBRARY IKey : public IBaseObejct
{
    Q_OBJECT
public:
    explicit IKey( QObject *parent = nullptr);

    bool open();

    QList<int> keyCodes();
    QList<QString> keyNames();

    QString devName();

Q_SIGNALS:
    void keyPress(int key, bool bPressed);
};
}
#endif // IKEY_H
