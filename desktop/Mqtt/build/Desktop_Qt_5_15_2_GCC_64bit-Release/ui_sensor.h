/********************************************************************************
** Form generated from reading UI file 'sensor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENSOR_H
#define UI_SENSOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
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
    QPushButton *pushButton_led1;
    QPushButton *pushButton_led2;
    QPushButton *pushButton_extio1;
    QPushButton *pushButton_beep;
    QLabel *label;
    QComboBox *comboBox_server;
    QPushButton *pushButton_open;
    QFrame *frame_ota;
    QVBoxLayout *verticalLayout_ota;
    QLabel *label_ota_status;
    QProgressBar *progressBar_ota;
    QLabel *label_ota;

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
        pushButton_cur->setGeometry(QRect(30, 360, 170, 85));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        pushButton_cur->setFont(font);
        pushButton_cur->setCheckable(true);
        pushButton_vr = new QPushButton(Widget);
        pushButton_vr->setObjectName(QString::fromUtf8("pushButton_vr"));
        pushButton_vr->setGeometry(QRect(410, 240, 170, 85));
        pushButton_vr->setCheckable(true);
        pushButton_hum = new QPushButton(Widget);
        pushButton_hum->setObjectName(QString::fromUtf8("pushButton_hum"));
        pushButton_hum->setGeometry(QRect(220, 240, 170, 85));
        pushButton_hum->setCheckable(true);
        pushButton_pw = new QPushButton(Widget);
        pushButton_pw->setObjectName(QString::fromUtf8("pushButton_pw"));
        pushButton_pw->setGeometry(QRect(220, 360, 170, 85));
        pushButton_pw->setCheckable(true);
        pushButton_temp = new QPushButton(Widget);
        pushButton_temp->setObjectName(QString::fromUtf8("pushButton_temp"));
        pushButton_temp->setGeometry(QRect(30, 240, 170, 85));
        pushButton_temp->setCheckable(true);
        pushButton_quit = new QPushButton(Widget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(600, 360, 170, 85));
        pushButton_vol = new QPushButton(Widget);
        pushButton_vol->setObjectName(QString::fromUtf8("pushButton_vol"));
        pushButton_vol->setGeometry(QRect(600, 240, 170, 85));
        pushButton_vol->setCheckable(true);
        pushButton_cpu = new QPushButton(Widget);
        pushButton_cpu->setObjectName(QString::fromUtf8("pushButton_cpu"));
        pushButton_cpu->setGeometry(QRect(410, 360, 170, 85));
        pushButton_cpu->setCheckable(true);
        pushButton_led1 = new QPushButton(Widget);
        pushButton_led1->setObjectName(QString::fromUtf8("pushButton_led1"));
        pushButton_led1->setGeometry(QRect(30, 120, 170, 85));
        pushButton_led1->setCheckable(true);
        pushButton_led2 = new QPushButton(Widget);
        pushButton_led2->setObjectName(QString::fromUtf8("pushButton_led2"));
        pushButton_led2->setGeometry(QRect(220, 120, 170, 85));
        pushButton_led2->setCheckable(true);
        pushButton_extio1 = new QPushButton(Widget);
        pushButton_extio1->setObjectName(QString::fromUtf8("pushButton_extio1"));
        pushButton_extio1->setGeometry(QRect(410, 120, 170, 85));
        pushButton_extio1->setCheckable(true);
        pushButton_beep = new QPushButton(Widget);
        pushButton_beep->setObjectName(QString::fromUtf8("pushButton_beep"));
        pushButton_beep->setGeometry(QRect(600, 120, 170, 85));
        pushButton_beep->setCheckable(true);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 91, 50));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        comboBox_server = new QComboBox(Widget);
        comboBox_server->addItem(QString());
        comboBox_server->addItem(QString());
        comboBox_server->setObjectName(QString::fromUtf8("comboBox_server"));
        comboBox_server->setGeometry(QRect(130, 40, 451, 50));
        QFont font2;
        font2.setPointSize(18);
        comboBox_server->setFont(font2);
        pushButton_open = new QPushButton(Widget);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));
        pushButton_open->setGeometry(QRect(600, 40, 171, 50));
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(true);
        pushButton_open->setFont(font3);
        pushButton_open->setCheckable(true);
        frame_ota = new QFrame(Widget);
        frame_ota->setObjectName(QString::fromUtf8("frame_ota"));
        frame_ota->setGeometry(QRect(100, 180, 600, 120));
        frame_ota->setStyleSheet(QString::fromUtf8("QFrame { background-color: rgba(0,0,0,180); border-radius: 8px; }"));
        frame_ota->setFrameShape(QFrame::StyledPanel);
        frame_ota->setFrameShadow(QFrame::Raised);
        verticalLayout_ota = new QVBoxLayout(frame_ota);
        verticalLayout_ota->setObjectName(QString::fromUtf8("verticalLayout_ota"));
        label_ota_status = new QLabel(frame_ota);
        label_ota_status->setObjectName(QString::fromUtf8("label_ota_status"));
        QFont font4;
        font4.setPointSize(14);
        font4.setBold(true);
        label_ota_status->setFont(font4);
        label_ota_status->setStyleSheet(QString::fromUtf8("color: white;"));
        label_ota_status->setAlignment(Qt::AlignCenter);

        verticalLayout_ota->addWidget(label_ota_status);

        progressBar_ota = new QProgressBar(frame_ota);
        progressBar_ota->setObjectName(QString::fromUtf8("progressBar_ota"));
        progressBar_ota->setMinimum(0);
        progressBar_ota->setMaximum(100);
        progressBar_ota->setValue(0);
        progressBar_ota->setTextVisible(true);

        verticalLayout_ota->addWidget(progressBar_ota);

        label_ota = new QLabel(Widget);
        label_ota->setObjectName(QString::fromUtf8("label_ota"));
        label_ota->setGeometry(QRect(5, 0, 120, 40));
        label_ota->setFont(font1);
        label_ota->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

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
        pushButton_led1->setText(QString());
        pushButton_led2->setText(QString());
        pushButton_extio1->setText(QString());
        pushButton_beep->setText(QString());
        label->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250", nullptr));
        comboBox_server->setItemText(0, QCoreApplication::translate("Widget", "xthings.cloud", nullptr));
        comboBox_server->setItemText(1, QCoreApplication::translate("Widget", "broker.emqx.io", nullptr));

        pushButton_open->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        label_ota_status->setText(QCoreApplication::translate("Widget", "OTA \344\270\213\350\275\275\344\270\255...", nullptr));
        label_ota->setText(QCoreApplication::translate("Widget", "OTA 1.1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSOR_H
