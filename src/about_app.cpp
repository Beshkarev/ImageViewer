#include "about_app.h"
#include "config.h"

#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

About::About(QWidget *parent) : QDialog(parent),
    _pTabs(new QTabWidget),
    _pAboutWidg(new QWidget),
    _pCreditsWidg(new QWidget)
{
    createAboutSpace();
    createCreditsSpace();

    QVBoxLayout *pVMainLayout = new QVBoxLayout(this);
    pVMainLayout->addWidget(_pTabs.get());

    setLayout(pVMainLayout);
    setWindowIcon(QIcon(":/icons/png-48px/business-card.png"));
    setWindowTitle(tr("About"));
    //setFixedSize(440, 274);
}

About::~About() = default;

void About::createAboutSpace()
{
    QLabel *pLabel = new QLabel(this);
    pLabel->setPixmap(QPixmap(":/icons/png-48px/image-outline.png"));
    pLabel->setAlignment(Qt::AlignCenter);

    QLabel *pTextEdit = new QLabel(this);
    pTextEdit->setAlignment(Qt::AlignCenter);
    pTextEdit->setText(tr("A small application for view image.") +
                       tr("<br>The programm developed for education and personal satisfaction.") +
                       tr("<br><br>This software is licensed under ") +
                       tr("<a href=\"https://github.com/Beshkarev/ImageViewer/blob/master/LICENSE\">GNU GPL</a> version 3.") +
                       tr("<br>Source code is available on ") +
                       tr("<a href=\"https://github.com/Beshkarev/ImageViewer\">GitHub</a>.") +
                       tr("<br>Version: %1.").arg(str_const_toString(Config::version)) +
                       tr("<br><br>Evgeniy Beshkarev") + "©");
    pTextEdit->setTextFormat(Qt::RichText);
    pTextEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);
    pTextEdit->setOpenExternalLinks(true);

    QVBoxLayout *pVBoxLayout = new QVBoxLayout(this);
    pVBoxLayout->addWidget(pLabel);
    pVBoxLayout->addWidget(pTextEdit);

    _pAboutWidg->setLayout(pVBoxLayout);
    _pTabs->addTab(_pAboutWidg.get(), tr("About"));
}

void About::createCreditsSpace()
{
    QFile file(":/about/CREDITS");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream textStream(&file);

    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setReadOnly(true);
    pTextEdit->setText(textStream.readAll());

    QVBoxLayout *pVBoxLayout = new QVBoxLayout(this);
    pVBoxLayout->addWidget(pTextEdit);

    _pCreditsWidg->setLayout(pVBoxLayout);
    _pTabs->addTab(_pCreditsWidg.get(), tr("Credits"));
}
