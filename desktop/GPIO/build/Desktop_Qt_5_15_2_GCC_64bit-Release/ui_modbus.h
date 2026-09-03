/********************************************************************************
** Form generated from reading UI file 'modbus.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODBUS_H
#define UI_MODBUS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_temp;
    QPushButton *pushButton_vr;
    QPushButton *pushButton_beep;
    QPushButton *pushButton_led2;
    QPushButton *pushButton_cpu;
    QPushButton *pushButton_hum;
    QPushButton *pushButton_led1;
    QPushButton *pushButton_vol;
    QPushButton *pushButton_relay;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        pushButton_temp = new QPushButton(Widget);
        pushButton_temp->setObjectName(QString::fromUtf8("pushButton_temp"));
        pushButton_temp->setGeometry(QRect(300, 190, 200, 100));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        pushButton_temp->setFont(font);
        pushButton_vr = new QPushButton(Widget);
        pushButton_vr->setObjectName(QString::fromUtf8("pushButton_vr"));
        pushButton_vr->setGeometry(QRect(300, 340, 200, 100));
        pushButton_beep = new QPushButton(Widget);
        pushButton_beep->setObjectName(QString::fromUtf8("pushButton_beep"));
        pushButton_beep->setGeometry(QRect(550, 40, 200, 100));
        pushButton_beep->setCheckable(true);
        pushButton_led2 = new QPushButton(Widget);
        pushButton_led2->setObjectName(QString::fromUtf8("pushButton_led2"));
        pushButton_led2->setGeometry(QRect(300, 40, 200, 100));
        pushButton_led2->setCheckable(true);
        pushButton_cpu = new QPushButton(Widget);
        pushButton_cpu->setObjectName(QString::fromUtf8("pushButton_cpu"));
        pushButton_cpu->setGeometry(QRect(50, 340, 200, 100));
        pushButton_hum = new QPushButton(Widget);
        pushButton_hum->setObjectName(QString::fromUtf8("pushButton_hum"));
        pushButton_hum->setGeometry(QRect(550, 190, 200, 100));
        pushButton_led1 = new QPushButton(Widget);
        pushButton_led1->setObjectName(QString::fromUtf8("pushButton_led1"));
        pushButton_led1->setGeometry(QRect(50, 40, 200, 100));
        pushButton_led1->setCheckable(true);
        pushButton_vol = new QPushButton(Widget);
        pushButton_vol->setObjectName(QString::fromUtf8("pushButton_vol"));
        pushButton_vol->setGeometry(QRect(550, 340, 200, 100));
        pushButton_relay = new QPushButton(Widget);
        pushButton_relay->setObjectName(QString::fromUtf8("pushButton_relay"));
        pushButton_relay->setGeometry(QRect(50, 190, 200, 100));
        pushButton_relay->setCheckable(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\347\211\251\350\201\224\347\275\221\350\256\276\345\244\207\347\256\241\347\220\206\345\256\242\346\210\267\347\253\257 2.1", nullptr));
        pushButton_temp->setText(QString());
        pushButton_vr->setText(QString());
        pushButton_beep->setText(QString());
        pushButton_led2->setText(QString());
        pushButton_cpu->setText(QString());
        pushButton_hum->setText(QString());
        pushButton_led1->setText(QString());
        pushButton_vol->setText(QString());
        pushButton_relay->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODBUS_H
