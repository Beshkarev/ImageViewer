#ifndef ABOUT_APP_H
#define ABOUT_APP_H

#include <QDialog>
#include <memory>
#include <QTabWidget>

class QTabWidget;
class QWidget;

class About: public QDialog
{
    Q_OBJECT

public:
    About(QWidget *parent = nullptr);

private:
    void createAboutSpace();
    void createCreditsSpace();

    std::unique_ptr<QTabWidget> _pTabs;
    std::unique_ptr<QWidget> _pAboutWidg;
    std::unique_ptr<QWidget> _pCreditsWidg;
};

#endif // ABOUT_APP_H
