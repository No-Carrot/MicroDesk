/********************************************************************************
** Form generated from reading UI file 'gpio.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GPIO_H
#define UI_GPIO_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_extio2;
    QPushButton *pushButton_beep;
    QPushButton *pushButton_led2;
    QPushButton *pushButton_extio3;
    QPushButton *pushButton_led1;
    QPushButton *pushButton_quit;
    QPushButton *pushButton_extio1;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        pushButton_extio2 = new QPushButton(Widget);
        pushButton_extio2->setObjectName(QString::fromUtf8("pushButton_extio2"));
        pushButton_extio2->setGeometry(QRect(300, 190, 200, 100));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        pushButton_extio2->setFont(font);
        pushButton_extio2->setCheckable(true);
        pushButton_beep = new QPushButton(Widget);
        pushButton_beep->setObjectName(QString::fromUtf8("pushButton_beep"));
        pushButton_beep->setGeometry(QRect(550, 40, 200, 100));
        pushButton_beep->setCheckable(true);
        pushButton_led2 = new QPushButton(Widget);
        pushButton_led2->setObjectName(QString::fromUtf8("pushButton_led2"));
        pushButton_led2->setGeometry(QRect(300, 40, 200, 100));
        pushButton_led2->setCheckable(true);
        pushButton_extio3 = new QPushButton(Widget);
        pushButton_extio3->setObjectName(QString::fromUtf8("pushButton_extio3"));
        pushButton_extio3->setGeometry(QRect(550, 190, 200, 100));
        pushButton_extio3->setCheckable(true);
        pushButton_led1 = new QPushButton(Widget);
        pushButton_led1->setObjectName(QString::fromUtf8("pushButton_led1"));
        pushButton_led1->setGeometry(QRect(50, 40, 200, 100));
        pushButton_led1->setCheckable(true);
        pushButton_quit = new QPushButton(Widget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(550, 340, 200, 100));
        pushButton_extio1 = new QPushButton(Widget);
        pushButton_extio1->setObjectName(QString::fromUtf8("pushButton_extio1"));
        pushButton_extio1->setGeometry(QRect(50, 190, 200, 100));
        pushButton_extio1->setCheckable(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\347\211\251\350\201\224\347\275\221\350\256\276\345\244\207\347\256\241\347\220\206\345\256\242\346\210\267\347\253\257 2.1", nullptr));
        pushButton_extio2->setText(QString());
        pushButton_beep->setText(QString());
        pushButton_led2->setText(QString());
        pushButton_extio3->setText(QString());
        pushButton_led1->setText(QString());
        pushButton_quit->setText(QString());
        pushButton_extio1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GPIO_H
