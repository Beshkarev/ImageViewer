#include "changedimages.h"
#include "screenimage.h"
#include <QBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>

QMap<QString, QImage> ChangedImages::images;

ChangedImages::ChangedImages(QWidget *pWdg):
    QDialog(pWdg)
{
    QTreeWidget *pTreeWidget = new QTreeWidget(this);
    pTreeWidget->setColumnCount(2);
    pTreeWidget->setIconSize(QSize(100, 80));
    QTreeWidgetItem *pTreeItem = new QTreeWidgetItem(pTreeWidget);
    //pTreeItem->setFlags(Qt::ItemIsUserCheckable);
    pTreeItem->setText(0, tr("Image"));
    //pTreeItem->setTextAlignment(0, Qt::Align);
    pTreeItem->setText(1, tr("Name"));

    QMap<QString, QImage>::const_iterator it;
    it = images.cbegin();

    while(it != images.cend())
    {
        addTreeItem(pTreeItem, it.key(), it.value());
        ++it;
    }
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    pMainLayout->addWidget(pTreeWidget);

    setLayout(pMainLayout);

    //setFixedSize(QSize(400, 500));
}


void ChangedImages::addImage(const QString &name,
                        const QImage &image)
{
    QString str;
    str = name;
    QImage img;
    img = image.copy();
    images.insert(str, img);
}

bool ChangedImages::isEmpty()
{
    return images.empty();
}

void ChangedImages::addTreeItem(QTreeWidgetItem *parent,
                                const QString &name,
                                const QImage &image)
{
    QTreeWidgetItem *pTreeItem = new QTreeWidgetItem;
    pTreeItem->setSizeHint(0, QSize(100, 80));
    pTreeItem->setIcon(0, QIcon(QPixmap::fromImage(image)));
    pTreeItem->setText(1, name);
    parent->addChild(pTreeItem);
}
