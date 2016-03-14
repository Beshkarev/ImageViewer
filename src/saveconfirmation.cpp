#include "saveconfirmation.h"
#include "screenimage.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QListWidget>

QHash<QString, QImage> SaveConfirmation::images;

SaveConfirmation::SaveConfirmation(QWidget *pWdg):
    QDialog(pWdg), _pListWidget(new QListWidget(this))
{
    _pListWidget->setIconSize(QSize(100, 80));
    _pListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    QHash<QString, QImage>::const_iterator it;
    it = images.cbegin();
    while(it != images.cend())
    {
        createItem(it.key(), it.value());
        ++it;
    }

    QPushButton *pButtonSaveAll = new QPushButton(this);
    QPushButton *pButtonNo = new QPushButton(this);
    QPushButton *pButtonCancel = new QPushButton(this);
    pButtonSaveAll->setText(tr("Save all"));
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
    pMainLayout->addWidget(_pListWidget);
    pMainLayout->addLayout(pHLayout);

    setLayout(pMainLayout);
    setFixedSize(QSize(600, 400));
    setWindowTitle(tr("Unsaved changes"));
}

void SaveConfirmation::addImage(const QString name,
                        const QImage image)
{
    QHash<QString, QImage>::iterator it;
    it = images.find(name);
    //if image already exist
    if(it != images.end())
    {
        images.remove(name);
        images.insert(name, image);
    }
    //if the new image
    else
        images.insert(name, image);
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
    QHash<QString, QImage>::const_iterator it;
    it = images.find(name);
    if(it != images.cend())
        return it.value();
    else
        return QImage();
}

void SaveConfirmation::deleteImage(const QString &name)
{
    images.remove(name);
}

void SaveConfirmation::saveImages()
{
    QList<QListWidgetItem *> selectedItems;
    selectedItems = _pListWidget->selectedItems();
    QList<QListWidgetItem *>::const_iterator itItems;
    itItems = selectedItems.cbegin();

    QImage img;
    QHash<QString, QImage>::const_iterator itFinded;
    bool successSaved;
    while(itItems != selectedItems.cend())
    {
        itFinded = images.find((*itItems)->text());
        img = itFinded.value().copy();
        successSaved = img.save(itFinded.key());

        if(successSaved)
            _pListWidget->removeItemWidget(*itItems);
        ++itItems;
    }

    accept();
}

void SaveConfirmation::createItem(const QString &name,
                                  const QImage &image)
{
    QListWidgetItem *pListItem = new QListWidgetItem;

    pListItem->setIcon(QIcon(QPixmap::fromImage(image)));
    pListItem->setText(name);
    _pListWidget->insertItem(_pListWidget->count() + 1,
                             pListItem);
    pListItem->setSelected(true);
}
