#include "appwindow.h"
#include <QVBoxLayout>
#include <QRegion>
#include <QTabBar>
#include <QPair>
#include <QCoreApplication>
#include <QDebug>
#include <QComboBox>
#include <QListView>
#include "titlebar.h"
#include "eventhandler.h"
#include <qguiapplication.h>
#include <QScreen>
#include "titlebar.h"
#include <QPainter>
#include "settings.h"
#include <QSplashScreen>
#include <QTimer>

EventHandler * AppWindow::g_handler =nullptr;
AppWindow::AppWindow(QWidget *centerWidget, const QString &title, QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setProperty("Round", 10);

    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);
    vBox->setSpacing(0);
    m_bar = new TitleBar(title, true);
    //connect(m_bar, &TitleBar::quit, qApp, &QCoreApplication::quit);
    connect(m_bar, &TitleBar::quit, this, [=](){
        this->close();
        QTimer::singleShot(1000, qApp, &QCoreApplication::quit);
    });
    vBox->addWidget(m_bar);
    vBox->addWidget(centerWidget);
    centerWidget->setObjectName("WidgetBox");
}

AppWindow::~AppWindow()
{
    this->destoryGloabl();
}

void AppWindow::initGloablBeforeApplication()
{
#ifndef DWKEYBOARD
 if(!qEnvironmentVariableIsSet("QT_IM_MODULE"))
 {
     qputenv("QT_IM_MODULE", QByteArray("im"));
 }
#endif
}

void AppWindow::initGloabl(bool bUseDefalutEventhandler)
{
    if(bUseDefalutEventhandler){
        g_handler = new EventHandler;
    }
}

void AppWindow::destoryGloabl()
{
    if(g_handler){
        delete g_handler;
        g_handler = nullptr;
    }
}

void AppWindow::loadSplashText(const QString &text, const QColor &c)
{
    QScreen *pScreen = qApp->primaryScreen();
    if(pScreen)
    {
        QPixmap pixmap(pScreen->size());
        QPainter painter(&pixmap);
        QWidget w;
        QRect rect =QRect(QPoint() ,pScreen->size());
        painter.fillRect(rect, c);
        painter.setPen(Qt::white);
        QRectF centerRect =painter.boundingRect(rect, text, QTextOption(Qt::AlignCenter));
        painter.drawText(centerRect, text);
        QSplashScreen splash(pixmap);
        splash.show();
        qApp->processEvents();
    }
}

void AppWindow::setkeyboard(bool bFollow)
{
    qApp->setProperty("bottom", !bFollow);
}

void AppWindow::show()
{
    if(QScreen *pScreen =qApp->primaryScreen())
    {
        this->resize(pScreen->size());
    }

    QWidget::show();
}

void AppWindow::setTitleBarVisible(bool bVisble)
{
  m_bar->setVisible(bVisble);
}
