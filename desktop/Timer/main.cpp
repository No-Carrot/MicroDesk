#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));      // fusion 这个主题比较好看
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint) ; // 设置窗体无边框
    //w.show();

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *rw = scene->addWidget(&w);
    rw->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    //view->resize(GCInstance.ScreenHeight,GCInstance.ScreenWidth); // 调整窗体的大小
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();
    return a.exec();
}
