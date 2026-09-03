#include "titlebar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "resourceloader.h"
#include <QPainter>
#include <QStyleOption>

class  TitleBarPrivate{
public:
    QLabel *verLbl = nullptr;
    QLabel *textLbl = nullptr;
    QPushButton *closeBtn = nullptr;
};

#include <QDebug>
using namespace fl_common;

TitleBar::TitleBar(const QString &title, bool bFullscreen,  QWidget *parent):QWidget(parent),
    m_d(new TitleBarPrivate)
{
    this->setObjectName("TitleBar");
    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->setContentsMargins(10,2,10,1);

    m_d->closeBtn = new QPushButton("X");
    m_d->closeBtn->setObjectName("CloseBtn");
    m_d->closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(m_d->closeBtn, &QPushButton::clicked, this, &TitleBar::quit);

    m_d->textLbl = new QLabel(title);
    m_d->textLbl->setObjectName("Title");

    if(bFullscreen) m_d->textLbl->setAlignment(Qt::AlignCenter);
    else m_d->textLbl->setAlignment(Qt::AlignLeft);

    m_d->textLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pHLayout->addWidget(m_d->textLbl);
    pHLayout->addWidget(m_d->closeBtn);
    this->setFixedHeight(30);

    setStyleSheet("QWidget{color:gray; font: bold;}"
                  "QLabel#Title{min-height-33px;max-height:33px; background: blue;}"
                  "QPushButton#CloseBtn{ font-size: 22px; min-width:30px; max-width:30px; min-height-25px;max-height:25px; background: gray; color:white;}");
}

TitleBar::~TitleBar()
{
    delete m_d;
}

void TitleBar::setText(const QString &title)
{
    m_d->textLbl->setText(title);
}

