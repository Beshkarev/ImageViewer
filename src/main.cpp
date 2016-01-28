#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "changedimages.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow W;
    W.show();
    //ChangedImages s;
    //s.show();
    return a.exec();
}
