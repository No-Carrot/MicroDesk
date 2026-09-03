#include "key.h"
#include "ui_key.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initUi(); // 初始化窗体
}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key() ;
    //qDebug() << "keyPressEvent:" << key ;
    //qDebug() << "Key_Up:" << Qt::Key_Up ;
    if (key == Qt::Key_Up)
    {
        qDebug() << "UP Key Pressed";
        ui->pushButton_up->setStyleSheet("QPushButton{background-image: url(:/images/upon.png);"
                                         "border: none; }");
        ui->pushButton_up->setChecked(true); // 让按键按下
    }
    else if (event->key() == Qt::Key_Down)
    {
        qDebug() << "DOWN Key Pressed";
        ui->pushButton_down->setStyleSheet("QPushButton{background-image: url(:/images/downon.png);"
                                           "border: none; }");
        ui->pushButton_down->setChecked(true); // 让按键按下
    }
    else if (event->key() == Qt::Key_Left)
    {
        qDebug() << "LEFT Key Pressed";
        ui->pushButton_left->setStyleSheet("QPushButton{background-image: url(:/images/lefton.png);"
                                           "border: none; }");
        ui->pushButton_left->setChecked(true); // 让按键按下
    }
    else if (event->key() == Qt::Key_Right)
    {
        qDebug() << "RIGHT Key Pressed";
        ui->pushButton_right->setStyleSheet("QPushButton{background-image: url(:/images/righton.png);"
                                            "border: none; }");
        ui->pushButton_right->setChecked(true); // 让按键按下
    }
    else if (event->key() == Qt::Key_Return)
    {
        qDebug() << "OK Key Pressed";
        ui->pushButton_ok->setStyleSheet("QPushButton{background-image: url(:/images/okon.png);"
                                         "border: none; }");
        ui->pushButton_ok->setChecked(true); // 让按键按下
    }
    else if (event->key() == Qt::Key_Escape)
    {
        qDebug() << "ESC Key Pressed";
        ui->pushButton_esc->setStyleSheet("QPushButton{background-image: url(:/images/escon.png);"
                                          "border: none; }");
        ui->pushButton_esc->setChecked(true); // 让按键按下
    }
    // 事件继续传递
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "keyReleaseEvent:" << event->key() ;
    if (event->key() == Qt::Key_Left)
    {
        ui->pushButton_left->setStyleSheet("QPushButton{background-image: url(:/images/leftoff.png);"
                                           "border: none; }");
        ui->pushButton_left->setChecked(false); // 让按键松开
    }
    else if (event->key() == Qt::Key_Up)
    {
        ui->pushButton_up->setStyleSheet("QPushButton{background-image: url(:/images/upoff.png);"
                                         "border: none; }");
        ui->pushButton_up->setChecked(false); // 让按键松开
    }
    else if (event->key() == Qt::Key_Down)
    {
        ui->pushButton_down->setStyleSheet("QPushButton{background-image: url(:/images/downoff.png);"
                                           "border: none; }");
        ui->pushButton_down->setChecked(false); // 让按键松开
    }
    else if (event->key() == Qt::Key_Right)
    {
        ui->pushButton_right->setStyleSheet("QPushButton{background-image: url(:/images/rightoff.png);"
                                            "border: none; }");
        ui->pushButton_right->setChecked(false); // 让按键松开
    }
    else if (event->key() == Qt::Key_Return) // enter 键
    {
        ui->pushButton_ok->setStyleSheet("QPushButton{background-image: url(:/images/okoff.png);"
                                         "border: none; }");
        ui->pushButton_ok->setChecked(false); // 让按键松开
    }
    else if (event->key() == Qt::Key_Escape)
    {
        ui->pushButton_esc->setStyleSheet("QPushButton{background-image: url(:/images/escoff.png);"
                                          "border: none; }");
        ui->pushButton_esc->setChecked(false); // 让按键松开
    }
    // 事件继续传递
    QWidget::keyReleaseEvent(event);
}

void Widget::paintEvent(QPaintEvent *event)
{
    qDebug() << "paintEvent";
}

void Widget::initUi()
{
    // 绘制背景图片
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(QString::fromUtf8(":/images/back.png"));
    QImage pix = ImgAllbackground.scaled(this->size(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(pix));
    this->setPalette(PAllbackground);

    ui->pushButton_left->setStyleSheet("QPushButton{background-image: url(:/images/leftoff.png);"
                                       "border: none; }");

    ui->pushButton_up->setStyleSheet("QPushButton{background-image: url(:/images/upoff.png);"
                                     "border: none; }");

    ui->pushButton_right->setStyleSheet("QPushButton{background-image: url(:/images/rightoff.png);"
                                        "border: none; }");

    ui->pushButton_ok->setStyleSheet("QPushButton{background-image: url(:/images/okoff.png);"
                                     "border: none; }");

    ui->pushButton_down->setStyleSheet("QPushButton{background-image: url(:/images/downoff.png);"
                                       "border: none; }");

    ui->pushButton_esc->setStyleSheet("QPushButton{background-image: url(:/images/escoff.png);"
                                      "border: none; }");

    ui->pushButton_quit->setStyleSheet("QPushButton{background-image: url(:/images/quit.png);"
                                       "border: none; }");

    // 不写下面这两行，会导致键盘事件失效
    setFocusPolicy(Qt::StrongFocus); // 设置焦点策略
    this->setFocus(); // 确保窗口获得焦点
}

void Widget::on_pushButton_quit_clicked()
{
    close();
    qApp->exit();
}
