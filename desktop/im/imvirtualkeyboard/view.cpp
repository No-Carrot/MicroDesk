#include "view.h"
#include <QQuickView>
#include <QQmlContext>
#include "iminputcontext.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QScreen>
#include <QFontMetrics>
#include "implatforminputcontext.h"
#include <QUrl>

View::View(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
{
}

void View::create()
{
    if(m_view != nullptr)
        return;

    m_view = new QQuickView;
    m_view->setKeyboardGrabEnabled(false);
    m_view->setMouseGrabEnabled(false);
    m_view->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    m_view->setFlags(m_view->flags() | Qt::Tool);
    m_view->rootContext()->setContextProperty("imctx",  IMInputContext::instance());

    QString fileName = qgetenv("IM_KEYBOARD_QML");
    if(QFile::exists(fileName) && fileName.contains("Keyboard.qml"))
        m_view->setSource(QUrl::fromLocalFile(fileName));
    else
         m_view->setSource(QUrl("qrc:/Keyboard.qml"));

    connect(qGuiApp, &QCoreApplication::aboutToQuit, [=](){
        m_view->close();
        m_view->update();
        delete m_view;
    });

    connect(qApp, &QGuiApplication::focusWindowChanged, this, [=](QWindow *focusWindow){
           if(focusWindow){
               focusWindow->installEventFilter(this);
           }
    });
}

bool View::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::PlatformSurface){
        QPlatformSurfaceEvent *p = dynamic_cast<QPlatformSurfaceEvent*>(event);
        if(p && p->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed){
            m_view->setParent(NULL);
        }
    }

    return  QObject::eventFilter(obj, event);
}

#include <QTimer>
void View::show()
{

    //qDebug()<<"focusWindow="<<qApp->focusWindow() << m_view->isVisible();

    if(qApp->focusWindow())
        m_view->setParent(qApp->focusWindow());
    m_view->show();
    QTimer::singleShot(10, this, [=](){
            this->resize();
        });
}

void View::hide()
{
    if(m_view->isVisible())
        m_view->hide();
}

bool View::isVisible()
{
    return m_view->isVisible();
}

void View::move(int x, int y)
{
    QPoint point(x, y);
    if(qApp->focusWindow()){
        point = qApp->focusWindow()->mapFromGlobal(point);
    }


    m_view->setX(point.x());
    m_view->setY(point.y());
}

QRect View::rect()
{
    return m_view->geometry();
}


void View::resize()
{
    if(qApp->focusWindow()){

#if 0
        QTransform t = QGuiApplication::inputMethod()->inputItemTransform();
        QRect cusRext = IMInputContext::instance()->platformCtx()->inputMethodQuery(Qt::ImCursorRectangle).toRect();
        QRect gCursorFocusWindowRect = t.mapRect(cusRext);
        QPoint gWindowPos =qApp->focusWindow()->mapToGlobal(QPoint(0,0));

        int gCurorX = gWindowPos.x() + gCursorFocusWindowRect.x() + gCursorFocusWindowRect.width();
        int gCurorY = gWindowPos.y() + gCursorFocusWindowRect.y() + gCursorFocusWindowRect.height();

        QPoint gloabePos(gCurorX, gCurorY);

        int width =this->rect().width();
        int heigth = this->rect().height();

        qDebug()<<"width ="<<width;
        qDebug()<<"height="<<heigth;

        QScreen *screen = qApp->focusWindow()->screen();
        if(screen){
            int maxX = screen->availableGeometry().width()- width;
            int maxY = screen->availableGeometry().height()- heigth;

            qDebug()<<"maxX"<< maxX;
            qDebug()<<"maxY"<< maxY;
            if(gloabePos.x() > maxX)
            {
                gloabePos.setX(maxX);
            }

            if(gloabePos.y() >maxY)
            {
                gloabePos.setY(maxY- heigth/2);
            }

             qDebug()<<"gloabePos ="<<gloabePos;
        }

#else
        QTransform t = QGuiApplication::inputMethod()->inputItemTransform();
        QRect cursorInputObjectRect = IMInputContext::instance()->platformCtx()->inputMethodQuery(Qt::ImCursorRectangle).toRect();
        QRect cursorInputWindowRect = t.mapRect(cursorInputObjectRect);
        QPoint gWindowPos =qApp->focusWindow()->mapToGlobal(QPoint(0,0));
        int gCurorX = gWindowPos.x() + cursorInputWindowRect.x() + cursorInputWindowRect.width();
        int gCurorY = gWindowPos.y() + cursorInputWindowRect.y() + cursorInputWindowRect.height();

        QScreen *screen = qApp->focusWindow()->screen();
        if(screen){

            int maxX = screen->availableGeometry().width()- rect().width();
            int maxY = screen->availableGeometry().height()- rect().height();
//            qDebug()<<"screen availableGeometry:" << screen->availableGeometry();
//            qDebug()<<"view width: " << rect().width();
//            qDebug()<<"view height: " << rect().height();
//            qDebug()<<"gCurorX:" <<gCurorX;
//            qDebug()<<"gCurorY:" <<gCurorY;
//            qDebug()<<"max :" << maxX;
//            qDebug()<<"maxY: "<<maxY;

            if(gCurorX > maxX){
                gCurorX =maxX;
            }
            if(gCurorY >maxY){
                gCurorY =gCurorY-rect().height();
            }
        }


#endif


        this->move(gCurorX, gCurorY);
    }
}

