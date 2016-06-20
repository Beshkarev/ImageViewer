#include "screenimage.h"
#include "saveconfirmation.h"
#include <QFileDialog>
#include <QImage>
#include <QBoxLayout>
#include <QScrollBar>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include <thread>

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QWidget(pWd),
    _pScrollArea(new QScrollArea),
    _pLabel(new QLabel),
    clockwiseValue(90), counterClockwiseValue(-90),
    imageChanged(false),
    zoomInValue(0.8), zoomOutValue(1.25),
    zoomFactor(1.0)

{
    angle = clockwiseValue;
    _pScrollArea->setWidget(_pLabel.get());
    _pScrollArea->setWidgetResizable(true);
    _pScrollArea->setAttribute(Qt::WA_TransparentForMouseEvents);
    _pScrollArea->hide();

    _pLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(_pScrollArea.get());

    setLayout(pHLayout);
}

bool ScreenImage::isEmpty() const
{
    return m_Image.isNull();
}

ScreenImage::~ScreenImage()
{}

void ScreenImage::loadImage(const QString &name)
{
    if(SaveConfirmation::imageWasChanged(name))
    {
        m_Image = SaveConfirmation::getChagedImage(name);
    }
    else
        m_Image.load(name);

    bestImageGeometry();
    showImage();

    _fileName = name;
    imageChanged = false;

    emit imageLoaded();
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

void ScreenImage::mouseReleaseEvent(QMouseEvent*)
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
    std::thread thread(SaveConfirmation::addImage, _fileName, m_Image);
    thread.detach();
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
