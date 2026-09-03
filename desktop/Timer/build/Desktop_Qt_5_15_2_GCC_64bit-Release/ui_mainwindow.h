/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_language;
    QWidget *centralwidget;
    QLCDNumber *lcdNumber;
    QPushButton *pushButton_start;
    QPushButton *pushButton_reset;
    QTableView *tableView;
    QPushButton *pushButton_quit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 800);
        MainWindow->setMinimumSize(QSize(400, 600));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        action_language = new QAction(MainWindow);
        action_language->setObjectName(QString::fromUtf8("action_language"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lcdNumber = new QLCDNumber(centralwidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(0, 0, 480, 120));
        lcdNumber->setSmallDecimalPoint(false);
        lcdNumber->setDigitCount(8);
        pushButton_start = new QPushButton(centralwidget);
        pushButton_start->setObjectName(QString::fromUtf8("pushButton_start"));
        pushButton_start->setGeometry(QRect(20, 150, 120, 60));
        pushButton_start->setCheckable(true);
        pushButton_start->setChecked(false);
        pushButton_reset = new QPushButton(centralwidget);
        pushButton_reset->setObjectName(QString::fromUtf8("pushButton_reset"));
        pushButton_reset->setGeometry(QRect(180, 150, 120, 60));
        pushButton_reset->setCheckable(false);
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(0, 235, 480, 541));
        pushButton_quit = new QPushButton(centralwidget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(340, 150, 120, 60));
        pushButton_quit->setCheckable(false);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\347\247\222\350\241\250\350\256\241\346\227\266\345\231\250 1.5", nullptr));
        action_language->setText(QCoreApplication::translate("MainWindow", "\345\210\207\346\215\242\345\210\260\350\213\261\346\226\207", nullptr));
        pushButton_start->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\350\256\241\346\227\266", nullptr));
        pushButton_reset->setText(QCoreApplication::translate("MainWindow", "\345\244\215\344\275\215\350\256\241\346\225\260", nullptr));
        pushButton_quit->setText(QCoreApplication::translate("MainWindow", " \351\200\200\345\207\272\347\250\213\345\272\2171", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
