#include "eventhandler.h"

#include <QGuiApplication>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QtDebug>
#include <QComboBox>
#include <QList>
#include <QListView>
#include<QInputMethod>
#include <QVariant>
#include <QPainter>
#include <QBitmap>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include<QSpinBox>
#include <QScreen>
#include "settings.h"

#ifdef DWKEYBOARD
#include <DWKeyboard/KeyboardGlobal.h>
#endif

EventHandler::EventHandler(QObject *parent):QObject((parent))
{

    qApp->installEventFilter(this);
#ifdef DWKEYBOARD
    GlobalInit();
#endif
}

bool EventHandler::eventFilter(QObject *watched, QEvent *event)
{

    //统一使用DWKEYBOARD 虚拟键盘设置 ，如果平台编译模块了.并且安装到交叉系统跟目录( 头文件和库文件).
#ifdef DWKEYBOARD
    static bool mFocusIn = false;
    QLineEdit *lineEdt = dynamic_cast<QLineEdit*>(watched);
    bool bEdtInput = (lineEdt);
    if(bEdtInput&& !m_objects.contains(watched)){
        watched->installEventFilter(this);
    }

    if(bEdtInput && !lineEdt->topLevelWidget()->isModal()){
        if (event->type()==QEvent::FocusIn){
            mFocusIn = true;
        }
        else if (event->type()==QEvent::FocusOut){
            PlatformInputContextBase->FocusOut(watched);
            mFocusIn = false;
        }

        if (mFocusIn && event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *e = (QMouseEvent *)event;
            PlatformInputContextBase->FocusIn(watched, e->globalPos());
        }
    }
#endif

    //绘制顶层椭圆窗口.
    if(watched->isWidgetType()){
        QWidget *w= (QWidget*)watched;
        if(int round=w->property("Round").toInt())
        {
            if(event->type() == QEvent::Paint)
            {
                 QPainter painter(w);
                 painter.setRenderHint(QPainter::Antialiasing);
                 QStyleOption opt;
                 opt.init(w);
                 w->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, w);

                 if(!w->property("WA_TranslucentBackground").toBool())
                 {
                     QColor defaultColor =w->palette().brush(QPalette::Background).color();
                     painter.setBrush(defaultColor);
                 }
                 QPen pen(Qt::gray);
                 if(qApp->primaryScreen() && w->size() == qApp->primaryScreen()->size())
                 {
                    pen.setColor(Qt::black);
                    pen.setWidthF(1);
                 }else{
                    pen.setWidthF(3.6);
                 }

                 painter.setPen(pen);
                 painter.drawRoundedRect(w->rect(), round, round);

            }
            else if(event->type() == QEvent::Show)
            {
                //设置背景透明.
                w->setAttribute(Qt::WA_TranslucentBackground);
            }
        }

        //统一修改控件高度.
        if(QScreen *pScreen = qApp->primaryScreen()){
            static int widgetMinHeight = fl_common::Settings::instance()->getValue("widgetMinHeight", "35").toUInt();
            if( pScreen->size().height() >=600 && w->height() != widgetMinHeight){
                QComboBox *comBox = dynamic_cast<QComboBox*>(watched);
                QPushButton *button =dynamic_cast<QPushButton*>(watched);
                QRadioButton *radioBtn =dynamic_cast<QRadioButton*>(watched);
                QLineEdit *edit =dynamic_cast<QLineEdit*>(watched);
                QCheckBox *chkBox =dynamic_cast<QCheckBox*>(watched);
                QSpinBox *spinBox = dynamic_cast<QSpinBox*>(watched);
                static QList<QObject*> list;
                if(comBox && !list.contains(watched)){
                    list << watched;
                    comBox->setMinimumHeight(widgetMinHeight);
                    comBox->setView(new QListView());
                    comBox->setStyleSheet(QString("QComboBox QAbstractItemView::item {min-height:%1px; }").arg(widgetMinHeight));
                }
                if(button|| radioBtn || chkBox ||edit || spinBox){
                    if(w->objectName() !="CloseBtn")
                        w->setMinimumHeight(widgetMinHeight);
                }


            }
        }

    }

    return QObject::eventFilter(watched,event);
}

