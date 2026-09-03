#include "iminputcontext.h"
#include <QKeyEvent>
#include <QInputMethodQueryEvent>
#include <QEvent>
#include "implatforminputcontext.h"
#include <QtCore/QtDebug>
#include <QString>
#include <QKeyEvent>
#include <QList>
#include <QCoreApplication>
#include "abstractinputmethod.h"
#include "inputmethodmanager.h"
#include <QInputEvent>
#include <QTimer>
#include <QKeyEvent>

static IMInputContext *g_IMInputContext=NULL;
IMInputContext::IMInputContext(QObject *parent):
    QObject((parent)),
    m_ctx(NULL),
    m_bVisible(true)
{
}

bool IMInputContext:: eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
     if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease){

        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if( Qt::Key_V  == e->key() && Qt::AltModifier == e->modifiers()){
            if(!isInputPanelVisible() && event->type() == QEvent::KeyPress)
                InputMethodManager::instance()->changeLanguage();
            return true;
        }else if((Qt::ShiftModifier == e->modifiers() || Qt::NoModifier == e->modifiers())
                 && InputMethodManager::instance()->curentInputMethod()->keyEvent(e))
        {
            return true;
        }
       }
    return false;
}

IMPlatformInputContext *IMInputContext::platformCtx()
{
    return m_ctx;
}

void IMInputContext::setPlatformCtx(IMPlatformInputContext *ctx)
{
    m_ctx = ctx;
}

IMInputContext *IMInputContext::instance()
{
    if(g_IMInputContext == NULL)
        g_IMInputContext = new IMInputContext;
    return g_IMInputContext;
}

bool IMInputContext::isInputPanelVisible()
{
    return m_bVisible;
}

void IMInputContext::setInputPanelVisible(bool bVisible)
{
    m_bVisible = bVisible;
    m_bVisible =true;
}

QObject *IMInputContext::focusObject()
{
    if(m_ctx)
        return m_ctx->focusObject();

    return  NULL;
}

void IMInputContext::commit()
{
    if(isInputPanelVisible()){
        InputMethodManager::instance()->curentInputMethod()->commit();
        InputMethodManager::instance()->curentInputMethod()->dataChange(QStringList(), "");
    }

}
