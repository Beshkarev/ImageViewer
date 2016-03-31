#ifndef ABOUT_APP_H
#define ABOUT_APP_H

#include <QDialog>
#include <memory>

class QTabWidget;
class QWidget;

class About: public QDialog
{
public:
    About(QWidget *parent = 0);
private:
    void createAboutSpace();
    void createCreditsSpace();
    void createLicenseSpace();

    std::shared_ptr<QTabWidget> _pTabs;
    std::shared_ptr<QWidget> _pAboutWidg;
    std::shared_ptr<QWidget> _pCreditsWidg;
    std::shared_ptr<QWidget> _pLicenseWidg;
};

#endif // ABOUT_APP_H
