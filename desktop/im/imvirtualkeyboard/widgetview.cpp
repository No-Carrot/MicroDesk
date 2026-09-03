#include "widgetview.h"
#include <QtGui/QWidgetList>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include "iminputcontext.h"
#include <QStringListModel>
#include <QDebug>
#include <QScroller>
#include <QString>
#include <QStringList>
#include <QPalette>
#include <QFontInfo>
#include <QFont>
#include <QGuiApplication>
#include <QTransform>
#include "implatforminputcontext.h"
#include <QScreen>
#include <QApplication>
#include "inputmethodmanager.h"

class IMListView : public  QListView
{
public:
    void mousePressEvent(QMouseEvent *){}
    void mouseMoveEvent(QMouseEvent *){}
    void mouseReleaseEvent(QMouseEvent *){}
};

WidgetView::WidgetView(QObject *parent):QObject(parent),
    m_widget(nullptr)

{

}

#include <QWindow>
void WidgetView::create()
{
    if(m_widget != nullptr)
        return;

    m_widget =new QWidget;

    m_widget->setWindowFlags(Qt::WindowDoesNotAcceptFocus);
    m_widget->setObjectName("IM_INPUT_VIEW");
    m_widget->setFixedHeight(62);
    // m_widget->setStyleSheet("QWidget#imview{background: white;}");

    QVBoxLayout *layout = new QVBoxLayout(m_widget);
    layout->setSpacing(0);
    layout->setContentsMargins(5, 5,5,5);
    QLabel *spellStringLbl = new QLabel;
    spellStringLbl->setObjectName("spellString");
    spellStringLbl->setFixedHeight(30);
    spellStringLbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QListView *candidateView = new IMListView;
    candidateView->setInputMethodHints(Qt::ImhNone);
    candidateView->setObjectName("candidateView");
    candidateView->setFocusPolicy(Qt::NoFocus);
    candidateView->setStyleSheet("border:none;");
    candidateView->setEditTriggers(QListView::NoEditTriggers);
    candidateView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    candidateView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    candidateView->setFlow(QListView::LeftToRight);
    candidateView->setFixedHeight(30);
    candidateView->setContentsMargins(0,0,0,0);


    layout->addWidget(spellStringLbl);
    layout->addWidget(candidateView);
    QStringListModel *model = new QStringListModel;
    candidateView->setModel(model);


    connect(IMInputContext::instance(), &IMInputContext::dataChange, this, [=, spellStringLbl, model, candidateView](const QStringList &m, const QString &spellString){
        QStringList m2;
        for(int i=0; i<m.size(); i++){
            m2  << QString::number(i+1) + m.at(i);
        }

        model->setStringList(m2);
        spellStringLbl->setText(spellString);

        int fitWidth =0;
        for(int i=0; i<m.size(); i++){
            fitWidth +=candidateView->visualRect( model->index(i, 0)).width();
        }
        m_widget->resize(fitWidth + m_widget->layout()->margin()*2, m_widget->height());

        this->resize();

        if(m.size() >0)
            candidateView->setCurrentIndex(model->index(0, 0));
    });

}

void WidgetView::show()
{
    //qDebug()<<"parent ="<< m_widget->parentWidget();
    if(QWidget *p = focusTopWidget()){
        m_widget->setParent(p);
        this->resize();
    }
    m_widget->show(); // window has parent.  window 'show' and 'hide' must has parent .
}

void WidgetView::hide()
{
    m_widget->hide();
    if(m_widget->parentWidget()){
        m_widget->setParent(NULL);
       // qDebug()<<"set Parent NULL";
    }
}

bool WidgetView::isVisible()
{
    m_widget->isVisible();
}

void WidgetView::resize()
{
    if(m_widget->parentWidget())
    {
        QTransform t = QGuiApplication::inputMethod()->inputItemTransform();
        QRect cursorInputObjectRect = IMInputContext::instance()->platformCtx()->inputMethodQuery(Qt::ImCursorRectangle).toRect();
        QRect cursorInputWindowRect = t.mapRect(cursorInputObjectRect);
        int x =cursorInputWindowRect.x() +cursorInputObjectRect.width();
        int y = cursorInputWindowRect.y() + cursorInputObjectRect.height();
        int width =m_widget->width();
        int height = m_widget->height();


        if(x +width > m_widget->parentWidget()->width()){
            x = m_widget->parentWidget()->width()-width;
        }

        if(y+height >m_widget->parentWidget()->height()){
            y= y-m_widget->height()-cursorInputObjectRect.height();
        }

        this->move(x, y);
    }
}

void WidgetView::move(int x, int y)
{
    m_widget->move(x, y);
}

QRect WidgetView::rect()
{
    return m_widget->rect();
}

QWidget *WidgetView::focusTopWidget() const
{
    if(qApp->focusWidget() && qApp->focusWidget()->topLevelWidget())
        return qApp->focusWidget()->topLevelWidget();

    return NULL;
}
