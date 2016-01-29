#include "saveconfirmation.h"
#include "screenimage.h"
#include <QBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QDialogButtonBox>

QMap<QString, QImage> SaveConfirmation::images;

SaveConfirmation::SaveConfirmation(QWidget *pWdg):
    QDialog(pWdg)
{
    QTreeWidget *pTreeWidget = new QTreeWidget(this);
    pTreeWidget->setColumnCount(2);
    pTreeWidget->setIconSize(QSize(100, 80));
    pTreeWidget->setColumnWidth(0, 176);
    pTreeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    QTreeWidgetItem *pTreeItem = new QTreeWidgetItem(pTreeWidget);
    pTreeItem->setFlags(Qt::ItemIsSelectable |
                        Qt::ItemIsUserCheckable |
                        Qt::ItemIsEnabled);

    pTreeItem->setText(0, tr("Image"));
    pTreeItem->setText(1, tr("Name"));

    QMap<QString, QImage>::const_iterator it;
    it = images.cbegin();

    while(it != images.cend())
    {
        addTreeItem(pTreeItem, it.key(), it.value());
        ++it;
    }

    QDialogButtonBox *pButtons = new QDialogButtonBox(this);
    pButtons->addButton(QDialogButtonBox::SaveAll);
    pButtons->addButton(QDialogButtonBox::Cancel);
    pButtons->addButton(QDialogButtonBox::Close);

    connect(pButtons, SIGNAL(accepted()),
            this, SLOT(saveImages()));
    connect(pButtons, SIGNAL(rejected()),
            this, SLOT(reject()));

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addWidget(pTreeWidget);
    pMainLayout->addWidget(pButtons);

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
    accept();
}

void SaveConfirmation::addTreeItem(QTreeWidgetItem *parent,
                                const QString &name,
                                const QImage &image)
{
    QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
    pTreeItem->setSizeHint(0, QSize(100, 80));
    pTreeItem->setIcon(0, QIcon(QPixmap::fromImage(image)));
    pTreeItem->setText(1, name);
    parent->addChild(pTreeItem);
}
