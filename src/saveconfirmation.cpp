#include "saveconfirmation.h"
#include "screenimage.h"
#include <QBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QListWidget>

QMap<QString, QImage> SaveConfirmation::images;

SaveConfirmation::SaveConfirmation(QWidget *pWdg):
    QDialog(pWdg), _pListWidget(new QListWidget(this))
{
    _pListWidget->setIconSize(QSize(100, 80));
    _pListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    QMap<QString, QImage>::const_iterator it;
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

    connect(pButtonSaveAll, SIGNAL(released()),
            this, SLOT(saveImages()));
    connect(pButtonNo, SIGNAL(released()),
            this, SLOT(closeWithoutSaving()));
    connect(pButtonCancel, SIGNAL(released()),
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
}

void SaveConfirmation::addImage(const QString &name,
                        const QImage &image)
{
    QString str;
    str = name;
    QImage img;
    img = image.copy();
    images.insert(str, img);
}

bool SaveConfirmation::isEmpty()
{
    return images.empty();
}

void SaveConfirmation::saveImages()
{
    QList<QListWidgetItem *> selectedItems;
    selectedItems = _pListWidget->selectedItems();
    QList<QListWidgetItem *>::const_iterator itItems;
    itItems = selectedItems.cbegin();

    QImage img;
    QMap<QString, QImage>::const_iterator itFinded;
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

void SaveConfirmation::closeWithoutSaving()
{

    done(QDialogButtonBox::Close);
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
