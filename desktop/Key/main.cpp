#include "key.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));      // fusion 这个主题比较好看
    Widget w;
    w.show();
    return a.exec();
}
