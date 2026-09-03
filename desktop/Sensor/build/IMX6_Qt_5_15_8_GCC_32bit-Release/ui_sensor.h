/********************************************************************************
** Form generated from reading UI file 'sensor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENSOR_H
#define UI_SENSOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_cur;
    QPushButton *pushButton_vr;
    QPushButton *pushButton_hum;
    QPushButton *pushButton_pw;
    QPushButton *pushButton_temp;
    QPushButton *pushButton_quit;
    QPushButton *pushButton_vol;
    QPushButton *pushButton_cpu;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        pushButton_cur = new QPushButton(Widget);
        pushButton_cur->setObjectName(QString::fromUtf8("pushButton_cur"));
        pushButton_cur->setGeometry(QRect(300, 190, 200, 100));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        pushButton_cur->setFont(font);
        pushButton_cur->setCheckable(true);
        pushButton_vr = new QPushButton(Widget);
        pushButton_vr->setObjectName(QString::fromUtf8("pushButton_vr"));
        pushButton_vr->setGeometry(QRect(550, 40, 200, 100));
        pushButton_vr->setCheckable(true);
        pushButton_hum = new QPushButton(Widget);
        pushButton_hum->setObjectName(QString::fromUtf8("pushButton_hum"));
        pushButton_hum->setGeometry(QRect(300, 40, 200, 100));
        pushButton_hum->setCheckable(true);
        pushButton_pw = new QPushButton(Widget);
        pushButton_pw->setObjectName(QString::fromUtf8("pushButton_pw"));
        pushButton_pw->setGeometry(QRect(550, 190, 200, 100));
        pushButton_pw->setCheckable(true);
        pushButton_temp = new QPushButton(Widget);
        pushButton_temp->setObjectName(QString::fromUtf8("pushButton_temp"));
        pushButton_temp->setGeometry(QRect(50, 40, 200, 100));
        pushButton_temp->setCheckable(true);
        pushButton_quit = new QPushButton(Widget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(550, 340, 200, 100));
        pushButton_vol = new QPushButton(Widget);
        pushButton_vol->setObjectName(QString::fromUtf8("pushButton_vol"));
        pushButton_vol->setGeometry(QRect(50, 190, 200, 100));
        pushButton_vol->setCheckable(true);
        pushButton_cpu = new QPushButton(Widget);
        pushButton_cpu->setObjectName(QString::fromUtf8("pushButton_cpu"));
        pushButton_cpu->setGeometry(QRect(300, 340, 200, 100));
        pushButton_cpu->setCheckable(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\347\211\251\350\201\224\347\275\221\350\256\276\345\244\207\347\256\241\347\220\206\345\256\242\346\210\267\347\253\257 2.1", nullptr));
        pushButton_cur->setText(QString());
        pushButton_vr->setText(QString());
        pushButton_hum->setText(QString());
        pushButton_pw->setText(QString());
        pushButton_temp->setText(QString());
        pushButton_quit->setText(QString());
        pushButton_vol->setText(QString());
        pushButton_cpu->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSOR_H
