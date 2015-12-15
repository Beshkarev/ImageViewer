#include "screenimage.h"
#include <QFileDialog>
#include <QPixmap>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QWidget(pWd),
    _pScrollArea(new QScrollArea(this)),
    _pLabel(new QLabel(this)),
    _pPixmap(new QPixmap),
    angle(clockwiseValue), imageChanged(false),
    zoomIn(1.25), zoomOut(0.8),
    scale(zoomIn)
{
    _pScrollArea->setWidget(_pLabel);
    _pScrollArea->setWidgetResizable(true);
    _pScrollArea->hide();

    _pLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(_pScrollArea);

    setLayout(pHLayout);
}

bool ScreenImage::isChanged()
{
    return imageChanged;
}

void ScreenImage::loadImage()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Save file"),
                                                             "/home/evgeniy/Pictures",
                                                tr("All (*.*);;*.jpg;;*.bmp;;*.png;;"));
    if(!filename.isEmpty())
    {
        _pPixmap->load(filename);
        if(_pPixmap->isNull())
        {
            QMessageBox::warning(this, tr("Oops"), tr("Something went wrong!\n"
                                                         "Maybe, not supported the file format."),
                                QMessageBox::Ok);
            return;
        }
        else
            showImage();
    }
}

void ScreenImage::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                    "/home/evgeniy/Pictures",
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;"));
    _pPixmap->save(filename);
}

void ScreenImage::closeImage()
{
    _pLabel->clear();
    _pScrollArea->hide();
    //_pScrollArea->isWindowModified()
}

void ScreenImage::horizontalFlip()
{
    QImage img = QimageFactory();
    img = img.mirrored(true, false);
    _pPixmap->convertFromImage(img);
    showImage();
    somethingChanged();
}

void ScreenImage::clockwiseRotate()
{
    QImage img = QimageFactory();
    QTransform transform;
    transform.rotate(angle);

    img = img.transformed(transform);
    _pPixmap->convertFromImage(img);
    showImage();
    somethingChanged();
    angle = clockwiseValue;
}

void ScreenImage::counterClockwiseRotate()
{
    angle = counterClockwiseValue;
    clockwiseRotate();
    somethingChanged();
}

void ScreenImage::zoomInImage()
{
    QImage img = QimageFactory();
    qint32 width = _pPixmap->width();
    qint32 height = _pPixmap->height();

    img = img.scaled(QSize(width * scale, height * scale));
    _pPixmap->convertFromImage(img);
    //_pLabel->setPixmap(QPixmap::fromImage(img));

    showImage();
    //somethingChanged();
    scale = zoomIn;
}

void ScreenImage::zoomOutImage()
{
    scale = zoomOut;
    zoomInImage();
}

QImage ScreenImage::QimageFactory()
{
    QImage img(_pPixmap->toImage());
    return img;
}

void ScreenImage::showImage()
{
    _pLabel->setPixmap(*_pPixmap);
    _pScrollArea->show();
}

void ScreenImage::somethingChanged()
{
    imageChanged = true;
}
