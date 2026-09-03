#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStringList>
#include <QThread>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_connect_clicked(bool checked);

    void on_pushButton_quit_clicked();

private:
    Ui::Widget *ui;
    QString wifi_name;
    QString wifi_password;
};
#endif // WIDGET_H
