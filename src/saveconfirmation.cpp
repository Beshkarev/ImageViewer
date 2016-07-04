#include "saveconfirmation.h"
#include "screenimage.h"
#include "config.h"
#include "filesystem.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QDir>
#include <QListWidget>
#include <thread>

SaveConfirmation::changedImages SaveConfirmation::images;

SaveConfirmation::SaveConfirmation(QWidget *pWdg):
    QDialog(pWdg), _pListWidget(new QListWidget(this))
{
    _pListWidget->setIconSize(QSize(100, 100));
    _pListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    auto it = images.keys();
    #pragma omp parallel for
    for(auto i = 0; i < it.size(); ++i)
        createItem(it.at(i), getChagedImage(it.at(i)));

    QPushButton *pButtonSaveAll = new QPushButton(this);
    QPushButton *pButtonNo = new QPushButton(this);
    QPushButton *pButtonCancel = new QPushButton(this);
    pButtonSaveAll->setText(tr("Save selected"));
    pButtonSaveAll->setDefault(true);
    pButtonNo->setText(tr("Close without saving"));
    pButtonCancel->setText(tr("Cancel"));

    connect(pButtonSaveAll, SIGNAL(clicked(bool)),
            this, SLOT(saveImages()));
    connect(pButtonNo, SIGNAL(clicked(bool)),
            this, SLOT(accept()));
    connect(pButtonCancel, SIGNAL(clicked(bool)),
            this, SLOT(reject()));

    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(pButtonNo);
    pHLayout->addWidget(pButtonCancel);
    pHLayout->addWidget(pButtonSaveAll);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addWidget(_pListWidget.get());
    pMainLayout->addLayout(pHLayout);

    setLayout(pMainLayout);
    setFixedSize(QSize(600, 400));
    setWindowTitle(tr("Unsaved changes"));
}

SaveConfirmation::~SaveConfirmation() = default;

void SaveConfirmation::addImage(const QString &imageName,
                                const QImage &image)
{
    QString tempLocationSave;
    tempLocationSave = Config::tempLocation + "/" + FileSystem::fileName(imageName);
    images.insert(imageName, tempLocationSave);
    image.save(tempLocationSave);
}

bool SaveConfirmation::isEmpty()
{
    return images.empty();
}

bool SaveConfirmation::imageWasChanged(const QString &name)
{
    return images.find(name) != images.end() ? true : false;
}

QImage SaveConfirmation::getChagedImage(const QString &name)
{
    auto it = images.find(name);
    if(it != images.cend())
    {
        QImage img(it.value());

        return img;
    }
    else
        throw std::runtime_error(tr("The image was not changed.").toStdString());
}

void SaveConfirmation::deleteImage(const QString &name)
{
    images.remove(name);
}

void SaveConfirmation::saveImages()
{
    emit hide();
    setVisible(false);
    auto itImageChanged = images.cbegin();

    QImage img;
    for(auto itSelectedItems : _pListWidget->selectedItems())
    {
        itImageChanged = images.find(itSelectedItems->text());
        img.load(itImageChanged.value());
        img.save(itImageChanged.key());
    }

    accept();
}

void SaveConfirmation::createItem(const QString &name,
                                  const QImage &image)
{
    QListWidgetItem *pListItem = new QListWidgetItem(_pListWidget.get());

    pListItem->setIcon(QIcon(QPixmap::fromImage(image).scaled(100, 100)));
    pListItem->setText(name);
    _pListWidget->insertItem(_pListWidget->count() + 1,
                             pListItem);
    pListItem->setSelected(true);
}
