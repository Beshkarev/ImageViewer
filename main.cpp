#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *pW = new MainWindow;
    pW->show();

    return a.exec();
}
