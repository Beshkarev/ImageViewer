#include "screenimage.h"
#include <QFileDialog>
#include <QPixmap>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

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

bool ScreenImage::isChanged() const
{
    return imageChanged;
}

bool ScreenImage::isEmpty() const
{
    if(_pPixmap->isNull())
        return true;
    else
        return false;
}

QString ScreenImage::getFileName() const
{
    return _fileName;
}

bool ScreenImage::loadImage(const QString &filename)
{

    qDebug() << filename;
    _pPixmap->load(filename);
    if(_pPixmap->isNull())
    {
        showSomeError("Something went wrong!\nMaybe, not supported the file format.");
        return false;
    }
    else
        showImage();
        bestImageGeometry();
    _fileName = QFileInfo(filename).fileName();
    return true;
}

void ScreenImage::saveImage()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Save file"),
                                                    "/home/evgeniy/Pictures",
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;"));
    _pPixmap->save(filename);
}

void ScreenImage::closeImage()
{
    _pLabel->clear();
    _pScrollArea->hide();
}

void ScreenImage::horizontalFlip()
{
    QImage img = createQImage();
    if(img.isNull())
        return;
    img = img.mirrored(true, false);
    _pPixmap->convertFromImage(img);
    showImage();
    imageWasChanged();
}

void ScreenImage::clockwiseRotate()
{
    QImage img = createQImage();
    if(img.isNull())
        return;
    QTransform transform;
    transform.rotate(angle);

    img = img.transformed(transform);
    _pPixmap->convertFromImage(img);
    showImage();
    imageWasChanged();
    angle = clockwiseValue;
}

void ScreenImage::counterClockwiseRotate()
{
    angle = counterClockwiseValue;
    clockwiseRotate();
    imageWasChanged();
}

void ScreenImage::zoomInImage()
{
    qint32 width = _pPixmap->width();
    qint32 height = _pPixmap->height();

    *_pPixmap = _pPixmap->scaled(QSize(width * scale, height * scale),
                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    showImage();
    scale = zoomIn;
}

void ScreenImage::zoomOutImage()
{
    scale = zoomOut;
    zoomInImage();
}

void ScreenImage::fitImage(bool checked)
{
    _pLabel->setScaledContents(checked);
}

void ScreenImage::resizeEvent(QResizeEvent *)
{
    bestImageGeometry();
}

QImage ScreenImage::createQImage()
{
    QImage img(_pPixmap->toImage());
    return img;
}

void ScreenImage::showImage()
{
    _pLabel->setPixmap(*_pPixmap);
    if(_pScrollArea->isHidden())
        _pScrollArea->show();
}

void ScreenImage::imageWasChanged()
{
    imageChanged = true;
}

void ScreenImage::showSomeError(const QString &str)
{
    QMessageBox::warning(this, tr("Oops"), str,
                         QMessageBox::Ok);
}

void ScreenImage::bestImageGeometry()
{
    *_pPixmap = _pPixmap->scaled(width(), height(),
                                 Qt::KeepAspectRatio);
    showImage();
}
