//工厂类，实现UI的创建封装，可以重现实现  virtual QWidget *  widget(int id);， 添加新的功能，实现不改变原码添加新的模块,注释： 不要在QApplication 实例前调用
#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include <QObject>
#include "widgetcontrol.h"
#include <QWidget>
#include <QLabel>
#include "titlebar.h"
class WidgetFactoryPrivate;
class WidgetFactory
{
public:
    enum  WIDGET_ID{ //widget id列表.
        widget_4g=1,
        widget_about,
        widget_adc,
        widget_backlight,
        widget_key,
        widget_opengl,
        widget_network,
        widget_rtc,
        widget_serial,
        Widget_watchdog,
        widget_spi,
        widget_sqlite,
        widget_wifi,
        widget_terminal,
        widget_extra
    };

    static WidgetFactory *instance();

    //通过ID获取模块.
    virtual QWidget *  widget(int id);
    int id(QWidget*p);

    //通过ID获取模块.返回所有可用的模块.
    QMap<QString,int> &available();

    //类外添加模块.
    void addWidget(QWidget *widget, int id, const QString &name=QString());
    void addHelp(int id, const QString &name);


    //创建统一标题栏.
    static TitleBar * createTitleBar(const QString &title=QString(), const QString &version= QString());

    //设置控制接口
    MainControl *control();
    void setControl(MainControl *m);

    void translate(const QString &qm);

    explicit WidgetFactory();
    ~WidgetFactory();

protected:
    QWidget *getWidget(int id);

private:
    WidgetFactoryPrivate *m_d;
};

#endif // WIDGETFACTORY_H
