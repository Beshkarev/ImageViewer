#include "mainwindow.h"
#include "error.h"
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

    std::unique_ptr<MainWindow> W(nullptr);
    try
    {
        W = std::unique_ptr<MainWindow> (new MainWindow);
    }
    catch (std::runtime_error &err)
    {
        Error::showError(err.what());
    }

    W->show();
    return a.exec();
}
