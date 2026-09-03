/********************************************************************************
** Form generated from reading UI file 'key.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEY_H
#define UI_KEY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_down;
    QPushButton *pushButton_right;
    QPushButton *pushButton_up;
    QPushButton *pushButton_esc;
    QPushButton *pushButton_left;
    QPushButton *pushButton_quit;
    QPushButton *pushButton_ok;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        pushButton_down = new QPushButton(Widget);
        pushButton_down->setObjectName(QString::fromUtf8("pushButton_down"));
        pushButton_down->setGeometry(QRect(300, 190, 200, 100));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        pushButton_down->setFont(font);
        pushButton_down->setCheckable(true);
        pushButton_right = new QPushButton(Widget);
        pushButton_right->setObjectName(QString::fromUtf8("pushButton_right"));
        pushButton_right->setGeometry(QRect(550, 40, 200, 100));
        pushButton_right->setCheckable(true);
        pushButton_up = new QPushButton(Widget);
        pushButton_up->setObjectName(QString::fromUtf8("pushButton_up"));
        pushButton_up->setGeometry(QRect(300, 40, 200, 100));
        pushButton_up->setCheckable(true);
        pushButton_esc = new QPushButton(Widget);
        pushButton_esc->setObjectName(QString::fromUtf8("pushButton_esc"));
        pushButton_esc->setGeometry(QRect(550, 190, 200, 100));
        pushButton_esc->setCheckable(true);
        pushButton_left = new QPushButton(Widget);
        pushButton_left->setObjectName(QString::fromUtf8("pushButton_left"));
        pushButton_left->setGeometry(QRect(50, 40, 200, 100));
        pushButton_left->setCheckable(true);
        pushButton_quit = new QPushButton(Widget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(550, 340, 200, 100));
        pushButton_ok = new QPushButton(Widget);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(50, 190, 200, 100));
        pushButton_ok->setCheckable(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\347\211\251\350\201\224\347\275\221\350\256\276\345\244\207\347\256\241\347\220\206\345\256\242\346\210\267\347\253\257 2.1", nullptr));
        pushButton_down->setText(QString());
        pushButton_right->setText(QString());
        pushButton_up->setText(QString());
        pushButton_esc->setText(QString());
        pushButton_left->setText(QString());
        pushButton_quit->setText(QString());
        pushButton_ok->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEY_H
