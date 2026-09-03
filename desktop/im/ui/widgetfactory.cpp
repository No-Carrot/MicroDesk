#include "widgetfactory.h"
#include "backlightwidget.h"
#include "keywidget.h"
#include "watchdogwidgt.h"
#include "widgetcontrol.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"
#include <QCoreApplication>
#include <QTranslator>
#include <QFile>
#include "networkwidget.h"
#include "m4gwidget.h"
#include "spiwidget.h"
#include <QMap>
#include <QGlobalStatic>
#include "glwidget.h"
#include "datetimewidget.h"
#include "serialassistantwidget.h"
#include "adcwidget.h"
#include "bookwindow.h"
#include "abountwidget.h"
#include "wifiwidget.h"
#include "terminalwidget.h"
#include <QDebug>

Q_GLOBAL_STATIC(WidgetFactory, s_instance)

class WidgetFactoryPrivate{
public:

    QList<QWidget*> list;
    QList<int>  ids;
    QString qm;
    MainControl *control = nullptr;
    QMap<QString, int> maps;
};

WidgetFactory::WidgetFactory()
{
    m_d = new WidgetFactoryPrivate;
    if(m_d->maps.isEmpty()){
        m_d->maps.insert("key", WidgetFactory::widget_key);
        m_d->maps.insert("4g", WidgetFactory::widget_4g);
        m_d->maps.insert("network", WidgetFactory::widget_network);
        m_d->maps.insert("watchdog", WidgetFactory::Widget_watchdog);
        m_d->maps.insert("backlight", WidgetFactory::widget_backlight);
        m_d->maps.insert("spi", WidgetFactory::widget_spi);
        m_d->maps.insert("opengl", WidgetFactory::widget_opengl);
        m_d->maps.insert("rtc", WidgetFactory::widget_rtc);
        m_d->maps.insert("serial", WidgetFactory::widget_serial);
        m_d->maps.insert("adc", WidgetFactory::widget_adc);
        m_d->maps.insert("sqlite", WidgetFactory::widget_sqlite);
        m_d->maps.insert("about", WidgetFactory::widget_about);
        m_d->maps.insert("wifi", WidgetFactory::widget_wifi);
        m_d->maps.insert("terminal", WidgetFactory::widget_terminal);
    }
}

WidgetFactory::~WidgetFactory()
{
    delete m_d;
}

MainControl *WidgetFactory::control()
{
    return m_d->control;
}

void WidgetFactory::setControl(MainControl *m)
{
    m_d->control = m;
}

QMap<QString,int>  &WidgetFactory::available()
{

    return m_d->maps;
}

WidgetFactory *WidgetFactory::instance()
{
    return s_instance;
}

QWidget *WidgetFactory::widget(int id)
{
    QWidget *pWidget = getWidget(id);

    if(pWidget)
        return pWidget;

    switch (id) {
    case widget_backlight:
        pWidget = new BacklightWidget;
        break;

    case widget_key:
        pWidget = new KeyWidget;
        break;

    case widget_network:
        pWidget = new NetworkWidget;
        break;

    case Widget_watchdog:
        pWidget = new WatchDogWidgt;
        break;

    case widget_sqlite:
        pWidget = new BookWindow;
        break;

    case widget_4g:
        pWidget = new M4gWidget;
        break;

    case widget_spi:
        pWidget = new SpiWidget;
        break;

    case widget_opengl:
        pWidget = new OpenglWidget;
        break;

    case widget_adc:
        pWidget = new AdcWidget;
        break;

    case widget_about:
        pWidget = new AbountWidget;
        break;

    case widget_rtc:
        pWidget = new DateTimeWidget;
        break;

    case widget_serial:
        pWidget = new MultiSerialWidget;
        break;

    case widget_terminal:
        pWidget = new TerminalWidget;
        break;

    case widget_wifi:
        pWidget = new WifiWidget;
        break;
    default:
        break;
    }

    if(pWidget != nullptr){
        addWidget(pWidget, id);
    }

    return pWidget;
}

int WidgetFactory::id(QWidget *p)
{
    if(p == nullptr)
        return -1;

    int index = m_d->list.indexOf(p);
    if(index == -1)
        return index;

    return  m_d->ids.indexOf(index);
}

TitleBar *WidgetFactory::createTitleBar(const QString &title, const QString &version)
{
    if(version.isEmpty())
        return new TitleBar(title);
    return new TitleBar(title, version);
}

void WidgetFactory::translate(const QString &qm)
{
    if(qm.isEmpty() || m_d->qm == qm){
        return;
    }

    if(QFile::exists(qm) && qm.right(3) == ".qm") {
        m_d->qm = qm;
        QTranslator translator;
        translator.load(m_d->qm);
        qApp->installTranslator(&translator);
        foreach (QWidget *p, m_d->list) {
            if(WidgetControl *pInterface =  dynamic_cast<WidgetControl*>(p)){
                pInterface->resetTr();
            }
        }
    }
}

void WidgetFactory::addWidget(QWidget *widget, int id, const QString &name)
{
    m_d->ids <<id;
    m_d->list <<widget;
    this->addHelp(id, name);
}

void WidgetFactory::addHelp(int id, const QString &name)
{
    if(!name.isEmpty())
        m_d->maps.insert(name, id);
}


QWidget *WidgetFactory::getWidget(int id)
{
    int index = m_d->ids.indexOf(id);
    if(index >=0){
        return m_d->list.value(index);
    }

    return nullptr;
}
