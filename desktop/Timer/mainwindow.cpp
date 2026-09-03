#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->display("00:00.00");

    timer =  new QTimer(this);

    // 参数1  :  发送信号者 timer
    // 参数2  :  发送信号者 发送什么信号 , &类名::信号
    // 参数3  :  接收信号者 this, 就是这个应用程序MainWindow
    // 参数4  :  信号处理函数  用于自定义
    connect(timer,&QTimer::timeout,this,&MainWindow::timeout_slot) ;

    model =  new QStandardItemModel(this);
    /*设置列字段名*/
    model->setColumnCount(2);   // 设置这个数据模型为2列 ,  设置 一行有2列
    model->setHeaderData(0,Qt::Horizontal, tr("序号"));  // 第一列名 序号
    model->setHeaderData(1,Qt::Horizontal, tr("数值"));  // 第二列名 数值



    // 把 数据模型和显示模型进行关联
    ui->tableView->setModel(model) ; // 给tableview 安装一个数据模型 model ， 数据此时就显示出来了
    ui->tableView->horizontalHeader()->setDefaultSectionSize(240-1) ; // 设置列宽度
    ui->tableView->verticalHeader()->setHidden(true); // 隐藏行号

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout_slot()
{
    //qDebug() << "timeout_slot";
    msec ++ ;
    if(msec >= 60)
    {
        msec = 0 ;
        sec ++;
        if(sec >= 60 )
        {
            sec = 0 ;
            min ++;
            if(min >=60 )
            {
                min = 0 ;
            }
        }
    }
    ui->lcdNumber->display(QString::asprintf("%02d:%02d.%02d",min,sec,msec)); // 拼接字符串
}

void MainWindow::on_pushButton_start_clicked(bool checked)
{
    qDebug() << "on_pushButton_start_clicked:" << checked ;
    if(checked) // true  按钮被按下
    {
        // 启动定时器， 10毫秒执行一次信号处理函数
        if(timer != nullptr)
        {
            timer->start(10); // 10ms 计数
        }

        if(language) // 显示中文
        {
            ui->pushButton_start->setText(tr("停止计数"));
            ui->pushButton_reset->setText(tr("计数"));
        }
        else
        {
            ui->pushButton_start->setText(tr("StopCount"));
            ui->pushButton_reset->setText(tr("Count"));
        }

    }
    else // 按钮抬起
    {
        if(timer != nullptr)
        {
            timer->stop(); // 关闭定时器
        }


        if(language) // 显示中文
        {
            ui->pushButton_start->setText(tr("开始计数"));
            ui->pushButton_reset->setText(tr("复位计数"));
        }
        else
        {
            ui->pushButton_start->setText(tr("StartCount"));
            ui->pushButton_reset->setText(tr("ResetCount"));
        }
    }


}


void MainWindow::on_pushButton_reset_clicked()
{

    // 检查按钮是否按下
    if(ui->pushButton_start->isChecked())  // 计数
    {
        qDebug() << QString::asprintf("%02d:%02d.%02d",min,sec,msec) ;
        /*设置一条数据*/
        // lineCount : 是数据的第几行
        // 0 : 第0列
        // 1 : 第1列
        model->setItem(lineCount,0,new QStandardItem(QString::asprintf("%03d",lineCount+1))) ;
        model->setItem(lineCount,1,new QStandardItem(QString::asprintf("%02d:%02d.%02d",min,sec,msec))) ;
        model->item(lineCount, 0)->setTextAlignment(Qt::AlignCenter); // 设置文本居中对齐
        model->item(lineCount, 1)->setTextAlignment(Qt::AlignCenter); // 设置文本居中对齐
        ui->tableView->setModel(model);// 给tableview 安装一个数据模型 model ， 数据此时就显示出来了

        lineCount++;

    }
    else  // 复位计数
    {
        msec = 0 , sec =0 , min = 0 ;
        ui->lcdNumber->display(QString::asprintf("%02d:%02d.%02d",min,sec,msec)); // 拼接字符串

        model->removeRows(0,lineCount);// 从0行 一直到 制定的行
        lineCount = 0 ;

    }


}


void MainWindow::on_action_language_triggered()
{

    QTranslator translator;
    translator.load("translations/en_US.qm");// 去当前目录下去查 , 05-Timer-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug
    if(language) //  true: 切换到英文
    {
        language = false ;
        // 静态方法
        QApplication::installTranslator(&translator);
        ui->retranslateUi(this); // 重新初始化界面
        model->clear();  // 把model中的所有内容清空
        model->setColumnCount(2); // 设置列数 ,一共是2列
        model->setHeaderData(0,Qt::Horizontal,tr("NO"));    // 第1列的列名
        model->setHeaderData(1,Qt::Horizontal,tr("Value")); // 第2列的列名
        ui->tableView->setModel(model);
    }
    else // 切换成中文
    {
        language = true;
        // 以下两种办法都可以, 静态方法更加适用
        QApplication::installTranslator(nullptr);
        ui->retranslateUi(this); // 重新初始化界面
        ui->retranslateUi(this);
        model->clear();  // 把model中的所有内容清空
        model->setColumnCount(2);   // 设置 一行有2列
        model->setHeaderData(0,Qt::Horizontal, tr("序号"));  // 第一列名 序号
        model->setHeaderData(1,Qt::Horizontal, tr("数值"));  // 第二列名 数值
        ui->tableView->setModel(model);

    }
}


void MainWindow::on_pushButton_quit_clicked()
{
    close();
    qApp->exit();
}
































