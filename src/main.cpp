#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTranslator translate;
    //translate.load(":/translate/ru_UA.qm");
    //a.installTranslator(&translate);
    qDebug() << QLocale::system().name();
    MainWindow W;
    W.show();

    return a.exec();
}
