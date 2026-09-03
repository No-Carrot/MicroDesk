//窗口管理控制接口，
#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H
#include "processhandler.h"

class MainControl;

//小部件接口.
class WidgetControl
{
public:
    virtual void resetTr();
    virtual void setWindowTitleVisible(bool bVisible, int border = 10);
};

//继承式方便，
#define WIDGETCONTROL ,public WidgetControl
//继承是方便.添加了进程处理接口，用于出命令交互令读写
#define WIDGETCONTROLPROGRESS ,public WidgetControl, protected fl_common::ProcessHandler

#endif // WIDGETCONTROL_H
