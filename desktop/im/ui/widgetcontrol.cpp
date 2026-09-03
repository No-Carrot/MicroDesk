#include "widgetcontrol.h"
#include "widgetfactory.h"
#include <QWidget>

void WidgetControl::resetTr()
{

}

void WidgetControl::setWindowTitleVisible(bool bVisible, int border)
{

}

MainControl *WidgetControl::control()
{
    return WidgetFactory::instance()->control();
}
