#include "messagebox.h"
#include <QEventLoop>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QPropertyAnimation>
#include <QFontMetrics>
#include "titlebar.h"
#include "widgetfactory.h"

MessageBox::MessageBox(QWidget *parent, const QString &text, Button button) : QWidget(parent)
{
    this->resize(350, 250);

    m_pLoop = new QEventLoop(this);

    QLabel *label =new QLabel;
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setText(text);

    QPushButton *yes = new QPushButton(tr("OK"));
    yes->setMinimumHeight(35);
    connect(yes, &QPushButton::pressed, this, [=](){
        m_pLoop->exit(Button::yes);
    });

    QPushButton *no = new QPushButton(tr("Cancel"));
    no->setMinimumHeight(35);
    connect(no, &QPushButton::clicked, this, [=](){
        m_pLoop->exit(Button::no);
    });

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(30);
    hLayout->addStretch();
    hLayout->addWidget(yes);
    hLayout->addWidget(no);

    TitleBar * bar =  new TitleBar("MESSAGE.");
    connect(bar, &TitleBar::quit, m_pLoop, &QEventLoop::quit);

    QVBoxLayout *VLayout = new QVBoxLayout(this);
    VLayout->setContentsMargins(15, 15, 15, 15);
    VLayout->addWidget(bar);
    VLayout->addSpacing(15);
    VLayout->addWidget(label);
    VLayout->addLayout(hLayout);

    switch (button) {
    case Button::yes:{
        no->setVisible(false);
    }break;

    case Button::no:{
        yes->setVisible(false);
    } break;

    default:
        break;
    }
}

void getBlockObjects(QObject *o, QList<QObject *> &blockObjects, QObject *unBlockObject)
{
    if(o == unBlockObject) return;
    foreach (QObject *child, o->children()) {
        if(child != unBlockObject){
            blockObjects << child ;
            getBlockObjects(child, blockObjects, unBlockObject);
        }
    }
}

int MessageBox::exec()
{
    this->setWindowFlags( Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setProperty("Round", 15);
    this->show();

    QList<QObject *> blockObjects;

    QWidget *parentWidget = this->topLevelWidget();
    if(parentWidget == this)
        parentWidget = NULL;

    int x = 0;
    int y = 0;
    if(parentWidget != Q_NULLPTR && parentWidget->isVisible()) {
        x = (parentWidget->width()-width())/2;
        y = (parentWidget->height()-height())/2;
        getBlockObjects(parentWidget, blockObjects, this);
    }else{
        foreach (QWidget *w, qApp->topLevelWidgets()) {
            getBlockObjects(w, blockObjects, this);
        }
        this->setWindowModality(Qt::ApplicationModal);

        if(qApp->primaryScreen()){
            QSize size=qApp->primaryScreen()->size();
            x = (size.width()-this->width())/2;
            y= (size.height()-this->height())/2;
        }
    }

    move(x, y);

    auto setBolckState = [=](QList<QObject*> objects, bool bBlocked){
        foreach (QObject *o, objects) {
           bBlocked? o->installEventFilter(this): o->removeEventFilter(this);
        }
    };

    setBolckState(blockObjects, true);
    int ret = m_pLoop->exec();
    setBolckState(blockObjects, false);

    delete m_pLoop;
    return ret;
}

bool MessageBox::eventFilter(QObject *watched, QEvent *event)
{
    //过滤所非消息窗口的鼠标点击和聚集事件，使只能操作当强消息窗口.
    QInputEvent  *pMouse = dynamic_cast<QInputEvent*>(event);
    QFocusEvent *pFocus = dynamic_cast<QFocusEvent*>(event);
    if(pMouse|| pFocus|| event->type() == QEvent::InputMethod|| event->type() == QEvent::InputMethodQuery)
    {
        event->ignore();
        return true;
    }

    return QWidget::eventFilter(watched, event);
}

int MessageBox::showMessage(QWidget *parent, const QString &text,  Button button)
{
    if(parent && !parent->isVisible()){
        parent = nullptr;
    }
    MessageBox tip(parent, text,  button);
    return tip.exec();
}

void MessageBox::showMessage(QWidget *parent, const QString &text, int seconds)
{
    if(parent && !parent->isVisible()){
        parent = nullptr;
    }

    if(seconds <=0)
        return ;
    MessageBox tip(parent, text,  MessageBox::no);
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &tip, [&tip](){
        tip.m_pLoop->exit();;
    });
    timer.start(seconds*1000);

    tip.exec();
}
