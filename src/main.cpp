#include "mainwindow.h"
#include "error.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load(":/translate/translations/ImageViewer_" + QLocale::system().name());
    a.installTranslator(&appTranslator);

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
