#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <QString>
#include <QObject>
#include "ui_global.h"

class UI_EXPORT EventHandler : public QObject
{
public:
    EventHandler(QObject * parent = nullptr);

private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    QList<QObject*> m_objects;
};

#endif // EventHandler_H
