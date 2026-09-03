#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")


#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QStandardItemModel>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void timeout_slot();  // 定时时间到的处理函数

    void on_pushButton_start_clicked(bool checked);

    void on_pushButton_reset_clicked();

    void on_action_language_triggered();

    void on_pushButton_quit_clicked();

private:
    Ui::MainWindow *ui;
    qint32 msec=0,sec=0,min=0; // 定义毫秒，秒， 分钟
    QTimer *timer =  nullptr ; // 定义1个类指针
    QStandardItemModel *model ; // QStandardItemModel 这是一个标准的model/view的框架
    qint32 lineCount = 0 ; // 记录行数
    bool language = true; // true : 英文状体  , false : 中文

};
#endif // MAINWINDOW_H
