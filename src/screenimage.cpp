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
   qDebug() << save;
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

void ScreenImage::fitImage(bool checked)
{
    _pLabel->setScaledContents(checked);
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
    /*qreal zoom = static_cast<qreal>(width()) / static_cast<qreal>(m_Image.height());
    qreal zoom1 = static_cast<qreal>(height()) / static_cast<qreal>(m_Image.width());

    zoom = zoom / zoom1;
    qDebug() << zoom;
    //zoomImage(zoom);
    */
}

void ScreenImage::zoomImage(const qreal zoomFactor)
{
    m_Image.setDevicePixelRatio(zoomFactor);

    showImage();
}
