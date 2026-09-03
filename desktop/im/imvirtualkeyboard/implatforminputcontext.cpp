#include "implatforminputcontext.h"
#include "iminputcontext.h"
#include <QInputMethodQueryEvent>
#include <QtDebug>
#include <QGuiApplication>
#include "iminputcontext.h"
#include <QWindow>
#include "inputmethodmanager.h"
#include "keyboardwidget.h"

IMPlatformInputContext::IMPlatformInputContext() :
    m_inputContext(new IMInputContext(this)),
    m_focusObject(NULL),
    m_keyboard(NULL),
    m_bVisible(false)
{
    IMInputContext::instance()->setPlatformCtx(this);
    qDebug() << "[IM] IMPlatformInputContext created, platform:" << qApp->platformName();
}

IMPlatformInputContext::~IMPlatformInputContext()
{
 if(m_keyboard)
 {
     delete m_keyboard;
     m_keyboard = nullptr;
 }
}

bool IMPlatformInputContext::isValid() const
{
    return true;
}

void IMPlatformInputContext::commit()
{
   IMInputContext::instance()->commit();
}

void IMPlatformInputContext::showInputPanel()
{
    qDebug() << "[IM] showInputPanel called, m_focusObject:" << m_focusObject 
             << "inputMethodAccepted:" << inputMethodAccepted()
             << "platform:" << qApp->platformName();

    // 只有当焦点对象接受输入法时才显示键盘
    if(!inputMethodAccepted()){
        qDebug() << "[IM] Cannot show keyboard: input method not accepted";
        return;
    }

    if(m_keyboard == NULL){
        m_keyboard = new KeyboardWidget;
        qDebug() << "[IM] KeyboardWidget created";
    }
    
    if(!m_keyboard->isVisible()){
        if(m_focusObject){
            qDebug() << "[IM] Showing keyboard for:" << m_focusObject;
            m_keyboard->show();
            m_bVisible = true;
        } else {
            qDebug() << "[IM] Cannot show keyboard: m_focusObject is null";
        }
    }
}

void IMPlatformInputContext::hideInputPanel()
{
    if(m_keyboard != NULL && m_keyboard->isVisible()){
        //qDebug()<<"hide hideInputPanel";
        m_keyboard->hide();
        m_bVisible = false;
    }
}

bool IMPlatformInputContext::isInputPanelVisible() const
{
    return m_keyboard && m_keyboard->isVisible();
}

void IMPlatformInputContext::setFocusObject(QObject *object)
{
    QString platform = qApp->platformName();
    bool isEmbedded = (platform == "linuxfb" || platform == "eglfs");
    bool accepted = inputMethodAccepted();
    
    qDebug() << "[IM] setFocusObject:" << object 
             << "inputMethodAccepted:" << accepted
             << "isEmbedded:" << isEmbedded;
    
    // 只有当控件接受输入法时才设置 focusObject
    // 修复：即使在嵌入式平台，也只对接受输入法的控件弹出键盘
    if(accepted && object){
        m_focusObject = object;
        qDebug() << "[IM] Focus object set to:" << object;
    }else{
        m_focusObject = nullptr;
        // 如果控件不接受输入法，隐藏键盘
        if(m_keyboard && m_keyboard->isVisible()){
            qDebug() << "[IM] Hiding keyboard: focus object does not accept input method";
            m_keyboard->hide();
            m_bVisible = false;
        }
        qDebug() << "[IM] Focus object cleared";
    }
}


QObject *IMPlatformInputContext::focusObject()
{
    return m_focusObject;
}

bool IMPlatformInputContext::isVisble()
{
    return m_keyboard && m_keyboard->isVisible();
}

QVariant IMPlatformInputContext::inputMethodQuery(Qt::InputMethodQuery query)
{
    QInputMethodQueryEvent event(query);
    if(m_focusObject){
        qApp->sendEvent(m_focusObject,  &event);
    }
    return event.value(query);
}

void IMPlatformInputContext::update(Qt::InputMethodQueries)
{
    //qDebug()<<m_focusObject;
   if(inputMethodQuery(Qt::ImEnabled).toBool())
   {
       if(!isVisble() && m_bVisible)
       this->showInputPanel();
       //qDebug()<<"show";
   }else{

       if(isVisble() && !m_bVisible)
       this->hideInputPanel();
       // qDebug()<<"hide";
   }
}

