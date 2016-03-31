#include "about_app.h"
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>

About::About(QWidget *parent) : QDialog(parent),
    _pTabs(new QTabWidget),
    _pAboutWidg(new QWidget),
    _pCreditsWidg(new QWidget),
    _pLicenseWidg(new QWidget)
{
    createAboutSpace();
    createCreditsSpace();
    createLicenseSpace();

    QVBoxLayout *pVMainLayout = new QVBoxLayout(this);
    pVMainLayout->addWidget(_pTabs.get());

    setLayout(pVMainLayout);
    setMinimumSize(QSize(400, 200));
}

void About::createAboutSpace()
{
    QLabel *pLabel = new QLabel(this);
    pLabel->setPixmap(QPixmap(":/icons/png-48px/image-outline.png"));

    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setReadOnly(true);
    pTextEdit->setText(QObject::tr("A small application for view image.\n"
                        "The programm developed for education and personal satisfaction.\n"));

    QHBoxLayout *pHBoxLayout = new QHBoxLayout(this);
    pHBoxLayout->addWidget(pLabel);
    pHBoxLayout->addWidget(pTextEdit);

    _pAboutWidg->setLayout(pHBoxLayout);
    _pTabs->addTab(_pAboutWidg.get(), QObject::tr("About"));
}

void About::createCreditsSpace()
{
    QFile file(":/about/credits");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream textStream(&file);

    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setReadOnly(true);
    pTextEdit->setText(textStream.readAll());

    QHBoxLayout *pHBoxLayout = new QHBoxLayout(this);
    pHBoxLayout->addWidget(pTextEdit);

    _pCreditsWidg->setLayout(pHBoxLayout);
    _pTabs->addTab(_pCreditsWidg.get(), QObject::tr("Credits"));
}

void About::createLicenseSpace()
{
    QFile file(":/about/license");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream licenseFile(&file);

    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setReadOnly(true);
    pTextEdit->setText(licenseFile.readAll());

    QHBoxLayout *pHBoxLayout = new QHBoxLayout(this);
    pHBoxLayout->addWidget(pTextEdit);

    _pLicenseWidg->setLayout(pHBoxLayout);
    _pTabs->addTab(_pLicenseWidg.get(), QObject::tr("License"));
}
