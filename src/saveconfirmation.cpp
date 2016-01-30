#include "saveconfirmation.h"
#include "screenimage.h"
#include <QBoxLayout>
#include <QDebug>
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

    QDialogButtonBox *pButtonBox = new QDialogButtonBox(this);
    pButtonBox->addButton(QDialogButtonBox::SaveAll);
    pButtonBox->addButton(QDialogButtonBox::Cancel);
    pButtonBox->addButton(QDialogButtonBox::Close);

    connect(pButtonBox, SIGNAL(accepted()),
            this, SLOT(saveImages()));
    connect(pButtonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addWidget(_pListWidget);
    pMainLayout->addWidget(pButtonBox);

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
    qDebug() << "accept";
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
