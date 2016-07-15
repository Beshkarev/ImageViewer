#include "mainwindow.h"
#include "error.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();

    //QTranslator translate;
    //translate.load(":/translate/ru_UA.qm");
    //a.installTranslator(&translate);

//    qDebug() << QLocale::system().name();
    //QTranslator qtTranslator;
    //qtTranslator.load("qt_" + QString("ru_RU"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    //a.installTranslator(&qtTranslator);

    std::unique_ptr<MainWindow> W(nullptr);
    try
    {
        if(args.size() == 2)
            W = std::unique_ptr<MainWindow> (new MainWindow(nullptr,
                                                            args.at(1)));
        else
            W = std::unique_ptr<MainWindow> (new MainWindow);
        W->show();
    }
    catch (std::runtime_error &err)
    {
        Error::showError(err.what());
    }

    return a.exec();
}
