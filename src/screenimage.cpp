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
    zoomInValue(0.8), zoomOutValue(1.25),
    zoomFactor(1.0)

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
    //qDebug() << filename;
    m_Image.load(filename);
    if(m_Image.isNull())
    {
        showSomeError(tr("Something went wrong!\nMaybe, not supported the file format."));
        return false;
    }
    bestImageGeometry();
    showImage();

    _fileName = QFileInfo(filename).fileName();
    return true;
}

void ScreenImage::saveImage(const QString &filename) const
{
   bool save = m_Image.save(filename);
   //qDebug() << save;
}

void ScreenImage::closeImage()
{
    _pLabel->clear();
    _pScrollArea->hide();
    m_Image = QImage();
}

void ScreenImage::horizontalFlip()
{
    flipImge(true, false);;
    if(!isEmpty())
    {
        imageWasChanged();
        showImage();
    }
}

void ScreenImage::verticalFlip()
{
    flipImge(false, true);
    if(!isEmpty())
    {
        imageWasChanged();
        showImage();
    }
}

void ScreenImage::clockwiseRotate()
{
    QTransform transform;
    transform.rotate(angle);

    m_Image = m_Image.transformed(transform);
    if(!isEmpty())
    {
        imageWasChanged();
        showImage();
        angle = clockwiseValue;
        bestImageGeometry();
    }
}

void ScreenImage::counterClockwiseRotate()
{
    angle = counterClockwiseValue;
    clockwiseRotate();
    if(!isEmpty())
    {
        imageWasChanged();
        bestImageGeometry();
    }
}

void ScreenImage::zoomInImage()
{
    //qDebug("zoomIn");

    zoomFactor *= zoomInValue;

    //qDebug() << "zoomFactor in zoomIn" << zoomFactor;

    zoomImage(zoomFactor);
}

void ScreenImage::zoomOutImage()
{
    zoomFactor *= zoomOutValue;

    //qDebug() << "zoomFactor in zoomOut" << zoomFactor;

    zoomImage(zoomFactor);
}

void ScreenImage::fitImage()
{
    bestImageGeometry();
}

void ScreenImage::resizeEvent(QResizeEvent *)
{
    bestImageGeometry();
    //showImage();
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
    qreal imgWidth = static_cast<qreal>(m_Image.width());
    qreal imgHeight = static_cast<qreal>(m_Image.height());
    qreal screenWidth = static_cast<qreal>(width());
    qreal screenHeight = static_cast<qreal>(height());

    if(imgWidth < screenWidth && imgHeight < screenHeight)
        return;
    if(imgWidth > screenWidth)
    {
        //m_Image.setDotsPerMeterX(screenWidth);
        zoomFactor = imgWidth / screenWidth;
    }
    if(imgHeight > screenHeight)
    {
        //m_Image.setDotsPerMeterY(screenHeight);
        zoomFactor = imgHeight / screenHeight;
    }
    //qDebug() << zoomfacor;
    zoomImage(zoomFactor);
}

void ScreenImage::zoomImage(const qreal zoomFactor)
{
    m_Image.setDevicePixelRatio(zoomFactor);

    showImage();
}

void ScreenImage::flipImge(const bool horizontal, const bool vertical)
{
    m_Image = m_Image.mirrored(horizontal, vertical);
}
