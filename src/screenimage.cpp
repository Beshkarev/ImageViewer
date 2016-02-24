#include "screenimage.h"
#include "saveconfirmation.h"
#include <QFileDialog>
#include <QImage>
#include <QBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

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
    _pScrollArea->setAttribute(Qt::WA_TransparentForMouseEvents);
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
    return m_Image.isNull();
}

QString ScreenImage::getFileName() const
{
    return QFileInfo(_fileName).fileName();
}

bool ScreenImage::loadImage(const QImage &img,
                            const QString &name)
{
    m_Image = img.copy();

    if(m_Image.isNull() &&
            m_Image.format() == QImage::Format_Invalid)
    {
        showSomeError(tr("Oops"), tr("Something went wrong!\n"
                                   "Maybe, not supported the file format."));
        return false;
    }
    bestImageGeometry();
    showImage();

    _fileName = name;
    imageChanged = false;
    return true;
}

void ScreenImage::saveImage(const QString &filename) const
{
   bool save = m_Image.save(filename);
   //qDebug() << save;
}

void ScreenImage::closeImage()
{
    imageChanged = false;
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
    zoomFactor *= zoomInValue;

    zoomImage(zoomFactor);
}

void ScreenImage::zoomOutImage()
{
    zoomFactor *= zoomOutValue;

    zoomImage(zoomFactor);
}

void ScreenImage::fitImage()
{
    bestImageGeometry();
}

void ScreenImage::resizeEvent(QResizeEvent *)
{
    bestImageGeometry();
}

void ScreenImage::wheelEvent(QWheelEvent *pEvent)
{
    if(pEvent->delta() > 0)
        zoomInImage();
    if(pEvent->delta() < 0)
        zoomOutImage();
}

void ScreenImage::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        _mousePosition = pEvent->localPos().toPoint();
        setCursor(Qt::ClosedHandCursor);
    }
}

void ScreenImage::mouseMoveEvent(QMouseEvent *pEvent)
{
    if(pEvent->buttons() == Qt::LeftButton)
    {
        QScrollBar *pScrollBar;
        _mousePosition -= pEvent->localPos().toPoint();

        pScrollBar = _pScrollArea->horizontalScrollBar();
        pScrollBar->setValue(_mousePosition.x() + pScrollBar->value());

        pScrollBar = _pScrollArea->verticalScrollBar();
        pScrollBar->setValue(_mousePosition.y() + pScrollBar->value());

        _mousePosition = pEvent->localPos().toPoint();

    }
}

void ScreenImage::mouseReleaseEvent(QMouseEvent *pEvent)
{
    setCursor(Qt::ArrowCursor);
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
    SaveConfirmation::addImage(_fileName, m_Image);
}

void ScreenImage::showSomeError(const QString &title, const QString &str)
{
    QMessageBox::warning(this, title, str,
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
        zoomFactor = imgWidth / screenWidth;

    if(imgHeight > screenHeight)
        zoomFactor = imgHeight / screenHeight;

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
