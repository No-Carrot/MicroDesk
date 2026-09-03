/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLineEdit *lineEdit_ssid;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_passwd;
    QPushButton *pushButton_connect;
    QPushButton *pushButton_quit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        QFont font;
        font.setPointSize(14);
        Widget->setFont(font);
        lineEdit_ssid = new QLineEdit(Widget);
        lineEdit_ssid->setObjectName(QString::fromUtf8("lineEdit_ssid"));
        lineEdit_ssid->setGeometry(QRect(280, 30, 301, 40));
        lineEdit_ssid->setFont(font);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 30, 101, 40));
        label->setFont(font);
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 100, 101, 40));
        label_2->setFont(font);
        lineEdit_passwd = new QLineEdit(Widget);
        lineEdit_passwd->setObjectName(QString::fromUtf8("lineEdit_passwd"));
        lineEdit_passwd->setGeometry(QRect(280, 100, 301, 40));
        lineEdit_passwd->setFont(font);
        pushButton_connect = new QPushButton(Widget);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setGeometry(QRect(280, 170, 301, 60));
        pushButton_quit = new QPushButton(Widget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(620, 390, 150, 60));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "WIFI\350\264\246\345\217\267:", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "WIFI\345\257\206\347\240\201:", nullptr));
        pushButton_connect->setText(QCoreApplication::translate("Widget", "\351\223\276\346\216\245WIFI", nullptr));
        pushButton_quit->setText(QCoreApplication::translate("Widget", "\351\200\200\345\207\272\347\250\213\345\272\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
