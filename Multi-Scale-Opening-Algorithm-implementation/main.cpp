#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setSizeIncrement(100,100);
    w.show();
    return a.exec();
}
