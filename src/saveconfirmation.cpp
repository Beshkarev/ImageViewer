#include "saveconfirmation.h"
#include "screenimage.h"
#include "app_properties.h"
#include "filesystem.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QDir>

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

SaveConfirmation::~SaveConfirmation()
{}

void SaveConfirmation::addImage(const QString &imageName,
                                const QImage &image)
{
    QString tempLocationSave;
    tempLocationSave = AppProperties::tempLocation() + "/" + FileSystem::fileName(imageName);
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
    //QHash<QString, QImage>::const_iterator it;
    auto it = images.find(name);
    if(it != images.cend())
    {
        QImage img(it.value());

        return img;
    }
    else
        throw std::runtime_error("The image was not changed.");
}

void SaveConfirmation::deleteImage(const QString &name)
{
    images.remove(name);
}

void SaveConfirmation::saveImages()
{
    auto itImageChanged = images.cbegin();

    for(auto itSelectedItems : _pListWidget->selectedItems())
   // auto itSelectedItems = _pListWidget->selectedItems();
    //#pragma omp parallel for
    //for (auto i = 0; i < itSelectedItems.size(); ++i)
    {
        //itImageChanged = images.find((itSelectedItems.at(i)->text()));
        itImageChanged = images.find(itSelectedItems->text());
        QImage img(itImageChanged.value());

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
