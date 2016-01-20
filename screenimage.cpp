#include "screenimage.h"
#include <QFileDialog>
#include <QImage>
#include <QBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QWidget(pWd),
    _pScrollArea(new QScrollArea(this)),
    _pLabel(new QLabel(this)),
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
    if(m_Image.isNull())
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
    m_Image.load(filename);
    if(m_Image.isNull())
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

void ScreenImage::saveImage(const QString &filename) const
{
    m_Image.save(filename);
}

void ScreenImage::closeImage()
{
    _pLabel->clear();
    _pScrollArea->hide();
    m_Image = QImage();
}

void ScreenImage::horizontalFlip()
{
    m_Image = m_Image.mirrored(true, false);
    showImage();
    if(!isEmpty())
        imageWasChanged();
}

void ScreenImage::clockwiseRotate()
{
    QTransform transform;
    transform.rotate(angle);

    m_Image = m_Image.transformed(transform);
    showImage();
    if(!isEmpty())
        imageWasChanged();
    angle = clockwiseValue;
}

void ScreenImage::counterClockwiseRotate()
{
    angle = counterClockwiseValue;
    clockwiseRotate();
    if(!isEmpty())
        imageWasChanged();
}

void ScreenImage::zoomInImage()
{
    qint32 width = m_Image.width();
    qint32 height = m_Image.height();

    m_Image = m_Image.scaled(QSize(width * scale, height * scale),
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

void ScreenImage::showImage()
{
    _pLabel->setPixmap(QPixmap::fromImage(m_Image));
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
    m_Image = m_Image.scaled(width(), height(),
                                 Qt::KeepAspectRatio, Qt::SmoothTransformation);
    showImage();
}
