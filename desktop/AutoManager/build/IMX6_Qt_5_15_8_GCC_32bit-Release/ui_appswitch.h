/********************************************************************************
** Form generated from reading UI file 'appswitch.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPSWITCH_H
#define UI_APPSWITCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_appswitch
{
public:
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *appswitch)
    {
        if (appswitch->objectName().isEmpty())
            appswitch->setObjectName(QString::fromUtf8("appswitch"));
        appswitch->resize(400, 80);
        pushButton = new QPushButton(appswitch);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(240, 0, 160, 80));
        label = new QLabel(appswitch);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 171, 60));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);

        retranslateUi(appswitch);

        QMetaObject::connectSlotsByName(appswitch);
    } // setupUi

    void retranslateUi(QWidget *appswitch)
    {
        appswitch->setWindowTitle(QCoreApplication::translate("appswitch", "Form", nullptr));
        pushButton->setText(QString());
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class appswitch: public Ui_appswitch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPSWITCH_H
