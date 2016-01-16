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
    clockwiseValue(90), counterClockwiseValue(-90),
    angle(clockwiseValue), imageChanged(false),
    zoomIn(1.25), zoomOut(0.8),
    scale(zoomIn)
{
    _pScrollArea->setWidget(_pLabel);
    _pScrollArea->setWidgetResizable(true);
    _pScrollArea->hide();

    _pLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(_pScrollArea);

    setLayout(pHLayout);
}

bool ScreenImage::isChanged()
{
    return imageChanged;
}

bool ScreenImage::isEmpty()
{
    if(_pPixmap->isNull())
        return true;
    else
        return false;
}

QString ScreenImage::getFileName()
{
    return _fileName;
}

bool ScreenImage::loadImage()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                             "/home/evgeniy/Pictures",
                                                tr("All (*.*);;*.jpg;;*.bmp;;*.png;;"));
    if(filename.isEmpty())
        return false;
    if(!filename.isEmpty())
    {
        _pPixmap->load(filename);
        if(_pPixmap->isNull())
        {
            showSomeError("Something went wrong!\nMaybe, not supported the file format.");
            return false;
        }
        else
            showImage();
    }
    _fileName = QFileInfo(filename).fileName();
    return true;
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

    img = img.scaled(QSize(width * scale, height * scale),
                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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

void ScreenImage::showSomeError(const QString &str)
{
    QMessageBox::warning(this, tr("Oops"), str,
                        QMessageBox::Ok);
}
