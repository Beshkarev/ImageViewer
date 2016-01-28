#include "changedimages.h"
#include "screenimage.h"
#include <QGridLayout>
#include <QLabel>
#include <QBoxLayout>
#include <QCheckBox>
#include <QTextEdit>
#include <QTableWidget>

QMap<QString, QImage> ChangedImages::images;

ChangedImages::ChangedImages(QWidget *pWdg):
    QDialog(pWdg), _pTableWidget(new QTableWidget(this))
{
    //QGroupBox *pGroupBox = new QGroupBox(tr("Unsaved changes"), this);
    QMap<QString, QImage>::const_iterator it;
    it = images.cbegin();
    qint32 rown = 0;
    while(it != images.cend())
    {
        _pTableWidget->setCellWidget(rown++, 0,
                                     new TableWidgetItem(it.key(), it.value()));
        ++it;

    }
    //_pTableWidget->resize(200, 300);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(_pTableWidget);

    setLayout(pVLayout);
    //resize(100, 600);
    //show();
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

/*void ChangedImages::showChangedImages()
{
    if(isEmpty())
        return;
    QMap<QString, QImage>::const_iterator it;
    it = images.cbegin();
    QGridLayout *pGridLayout = new QGridLayout(this);
    QLabel *pLabel = new QLabel(this);
    //QScrollArea *pArea = new QScrollArea(this);
    //pArea->setWidget();

    //qint32 row = 0, column = 0;
    while(it != images.cend())
    {
        pLabel->setPixmap(QPixmap::fromImage(it.value()));
        pGridLayout->addWidget(pLabel);
    }
    setLayout(pGridLayout);
}*/

TableWidgetItem::TableWidgetItem(const QString &name,
                                 const QImage &img,
                                 QWidget *pWdg):
    QWidget(pWdg), _pCheckBox(new QCheckBox(this))
{
    QLabel *pLabel = new QLabel(this);
    pLabel->setPixmap(QPixmap::fromImage(img));
    pLabel->resize(50, 50);
    QTextEdit *pTextEdit = new QTextEdit(name, this);
    pTextEdit->setReadOnly(true);

    QHBoxLayout *pItemLayout = new QHBoxLayout(this);
    pItemLayout->addWidget(_pCheckBox);
    pItemLayout->addWidget(pLabel);
    pItemLayout->addWidget(pTextEdit);

    setLayout(pItemLayout);
    show();
}

bool TableWidgetItem::isChecked()
{
    return _pCheckBox->isChecked();
}
