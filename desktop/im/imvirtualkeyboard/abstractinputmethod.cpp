#include "abstractinputmethod.h"
#include <QLocale>
#include <QInputMethodEvent>
#include <QGuiApplication>
#include "iminputcontext.h"
#include <QtDebug>

void AbstractInputMethod::commit(const QString &text)
{
    Q_UNUSED(text)
}

void AbstractInputMethod::reset()
{

}

void AbstractInputMethod::commitText(const QString &text)
{
    if(IMInputContext::instance()->focusObject()){
        QInputMethodEvent event;
        event.setCommitString(text);
        qApp->sendEvent(IMInputContext::instance()->focusObject(), &event);
    }
}

void AbstractInputMethod::setPreeditText(const QString &text)
{
    if(IMInputContext::instance()->focusObject()){
        QList <QInputMethodEvent::Attribute> attrs;
        attrs << QInputMethodEvent::Attribute(QInputMethodEvent::Cursor, 0, 1, QVariant());
        QInputMethodEvent event2(text, attrs);
        qApp->sendEvent(IMInputContext::instance()->focusObject(), &event2);
    }
}

void AbstractInputMethod::sendKeyEvent(QKeyEvent *e)
{
    if(IMInputContext::instance()->focusObject()){
        qApp->sendEvent(IMInputContext::instance()->focusObject(), e);
    }
}


bool AbstractInputMethod::isNumber(int in)
{
    bool bRet =in >= Qt::Key_0 && in <= Qt::Key_9;

    return bRet;
}

bool AbstractInputMethod::isLetter(int in)
{
    bool bRet = in >= Qt::Key_A && in <=Qt::Key_Z | in >=(Qt::Key_A+32) && in <=(Qt::Key_Z+32);
    return bRet;
}

QString AbstractInputMethod::getText(int key)
{

    QString ret;
    if(m_map.isEmpty()) {

        struct KeyData{
            QString value[2];  //value[0] 英文字符 value[1] 中文字符.
            int k;
        };

        #define  max 73

       struct KeyData data[max]={
                    "~", "～", Qt::Key_AsciiTilde,
                    "!", "！", Qt::Key_Exclam,
                    "@", "@", Qt::Key_At,
                    "#", "#", Qt::Key_NumberSign,
                    "$", "￥", Qt::Key_Dollar,
                    "%", "%", Qt::Key_Percent,
                    "^", "……", Qt::Key_AsciiCircum,
                    "&", "&", Qt::Key_Ampersand,
                    "*", "*", Qt::Key_Asterisk,
                    "(", "（", Qt::Key_ParenLeft,
                    ")", "）", Qt::Key_ParenRight,
                    "_", "——", Qt::Key_Underscore,
                    "+", "+", Qt::Key_Plus,
                    "}", "】", Qt::Key_BraceRight,
                    "{", "【", Qt::Key_BraceLeft,
                    "\"", "“", Qt::Key_QuoteDbl,
                    "|", "|", Qt::Key_Bar,
                    "?", "？", Qt::Key_Question,
                    ">", "》", Qt::Key_Greater,
                    "<", "《", Qt::Key_Less,
                    "-", "-", Qt::Key_Minus,
                    "=", "=", Qt::Key_Equal,
                    "[", "『", Qt::Key_BracketLeft,
                    "]", "』", Qt::Key_BracketRight,
                    "\\", "、", Qt::Key_Backslash,
                    "'", "‘", Qt::Key_Apostrophe,
                    ";", "；", Qt::Key_Semicolon,
                    "/", "/", Qt::Key_Slash,
                    ".", "。", Qt::Key_Period,
                    ",", "，", Qt::Key_Comma,
                    "\r","\r", Qt::Key_Return,
                    "\b", "\b",Qt::Key_Backspace,
                    " "," ", Qt::Key_Space,
                    "`", "·", Qt::Key_QuoteLeft,
                    ":" ,"：" ,Qt::Key_Colon,
                    "P", "P", Qt::Key_P,
                    "O", "O", Qt::Key_O,
                    "I", "I", Qt::Key_I,
                    "U", "U", Qt::Key_U,
                    "Y", "Y", Qt::Key_Y,
                    "T", "T", Qt::Key_T,
                    "R", "R", Qt::Key_R,
                    "E", "E", Qt::Key_E,
                    "W", "W", Qt::Key_W,
                    "Q", "Q", Qt::Key_Q,
                    "A", "A", Qt::Key_A,
                    "S", "S", Qt::Key_S,
                    "D", "D", Qt::Key_D,
                    "F", "F", Qt::Key_F,
                    "G", "G", Qt::Key_G,
                    "H", "H", Qt::Key_H,
                    "J", "J", Qt::Key_J,
                    "K", "K", Qt::Key_K,
                    "L", "L", Qt::Key_L,
                    "M", "M", Qt::Key_M,
                    "N", "N", Qt::Key_N,
                    "B", "B", Qt::Key_B,
                    "V", "V", Qt::Key_V,
                    "C", "C", Qt::Key_C,
                    "X", "X", Qt::Key_X,
                    "Z", "Z", Qt::Key_Z,
                    "1", "1", Qt::Key_1,
                    "2", "2", Qt::Key_2,
                    "3", "3", Qt::Key_3,
                    "4", "4", Qt::Key_4,
                    "5", "5", Qt::Key_5,
                    "6", "6", Qt::Key_6,
                    "7", "7", Qt::Key_7,
                    "8", "8", Qt::Key_8,
                    "9", "9", Qt::Key_9,
                    "0", "0", Qt::Key_0,
                    "",  "", Qt::Key_CapsLock,
                    " ", " ", Qt::Key_Space
        };


        for(int i=0; i<max; i++){
            switch (this->language()) {
            case QLocale::Chinese:
                m_map.insert(data[i].k, data[i].value[1]);
                break;

            case QLocale::English:
                 m_map.insert(data[i].k, data[i].value[0]);
                break;
            default:
                break;
            }
        }
    }

    ret = m_map.value(key);

    return ret;
}

int DefaultInputMethod::language()
{
    return QLocale::English;
}

bool DefaultInputMethod::keyEvent(QKeyEvent *e)
{

    this->sendKeyEvent(e);
    return false;
}

#include <QDebug>
QKeyEvent *g_keyEvent= NULL;
bool GroupInputMethod::keyEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_Alt ||e->key() == Qt::Key_Escape) && e->type() == QEvent::KeyPress){
        IMInputContext::instance()->commit();
        return false;
    }


    if(e->type() == QEvent::KeyPress){
        m_mode = e->key()!= Qt::Key_Shift;
    }else if(e->type() == QEvent::KeyRelease && m_mode ==0)
    {
        m_bShitf =  !m_bShitf;
        IMInputContext::instance()->commit();
    }


    bool bRet =false;
    if(e->type() == QEvent::KeyPress && !m_bShitf && m_mode ==1){
        bRet = handleKeyPressEvent(e);
    }



    return bRet;
}

void GroupInputMethod::reset()
{
    m_mode =0;
    m_bShitf =false;
}
