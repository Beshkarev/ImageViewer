#include "screenimage.h"
#include "saveconfirmation.h"

#include <QImage>
#include <QScrollBar>
//#include <QDebug>
#include <QMouseEvent>
#include <thread>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QMovie>
#include <QGraphicsProxyWidget>

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QGraphicsView(pWd),
    _pScene(new QGraphicsScene(this)),
    clockwiseValue(90), counterClockwiseValue(-90),
    angle(0), imageChanged(false),
    zoomInValue(1.1), zoomOutValue(1.1),
    zoomFactor(1.0)

{
    connect(this, SIGNAL(showImageSignal()),
            this, SLOT(showImage()));

    _pImageItem = std::unique_ptr<QGraphicsPixmapItem>(_pScene->addPixmap(QPixmap::fromImage(m_Image)));

    setMouseTracking(true);
    _pImageItem->setTransformationMode(Qt::SmoothTransformation);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setScene(_pScene.get());
}

ScreenImage::~ScreenImage() = default;

bool ScreenImage::isEmpty() const
{
    return m_Image.isNull();
}

void ScreenImage::loadImage(const QString &name)
{
    if(SaveConfirmation::imageWasChanged(name))
        m_Image = SaveConfirmation::getChagedImage(name);
    else
        m_Image.load(name);

    _fileName = name;
    imageChanged = false;

    emit imageLoaded();
    emit showImageSignal();
}

void ScreenImage::loadGIF(const QString &name)
{

}

void ScreenImage::closeImage()
{
    imageChanged = false;
    _pImageItem->setPixmap(QPixmap());
    m_Image = QImage();
}

void ScreenImage::horizontalFlip()
{
    flipImge(true, false);;

    imageWasChanged();
}

void ScreenImage::verticalFlip()
{
    flipImge(false, true);

    imageWasChanged();
}

void ScreenImage::clockwiseRotate()
{
    angle = clockwiseValue;
    rotateImage(angle);
    imageWasChanged();
}

void ScreenImage::counterClockwiseRotate()
{
    angle = counterClockwiseValue;
    rotateImage(angle);
    imageWasChanged();
}

void ScreenImage::zoomInImage()
{
    resetTransform();
    zoomFactor *= zoomInValue;

    zoomImage(zoomFactor);
}

void ScreenImage::zoomOutImage()
{
    resetTransform();
    zoomFactor /= zoomOutValue;

    zoomImage(zoomFactor);
}

void ScreenImage::fitImage()
{
    bestImageGeometry();
}

void ScreenImage::resizeEvent(QResizeEvent *event)
{
//    fitImage();
    //bestImageGeometry();
    //qDebug() << event->type();
//    if(event->type() == QEvent::Resize)
//        //event->ignore();
//    //else
//        bestImageGeometry();
//    QGraphicsView::resizeEvent(event);

}

void ScreenImage::wheelEvent(QWheelEvent *pEvent)
{
    if(pEvent->delta() > 0)
        zoomInImage();
    if(pEvent->delta() < 0)
        zoomOutImage();

    pEvent->accept();
}

void ScreenImage::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        _mousePosition = pEvent->localPos().toPoint();
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(pEvent);
}

void ScreenImage::mouseMoveEvent(QMouseEvent *pEvent)
{
    if(pEvent->buttons() == Qt::LeftButton)
    {
        QScrollBar *pScrollBar;
        _mousePosition -= pEvent->localPos().toPoint();

        pScrollBar = horizontalScrollBar();
        pScrollBar->setValue(_mousePosition.x() + pScrollBar->value());

        pScrollBar = verticalScrollBar();
        pScrollBar->setValue(_mousePosition.y() + pScrollBar->value());

        _mousePosition = pEvent->localPos().toPoint();
    }

    QGraphicsView::mouseMoveEvent(pEvent);
}

void ScreenImage::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsView::mouseReleaseEvent(event);
}

void ScreenImage::showImage()
{
    _pScene->setSceneRect(0, 0, m_Image.width(), m_Image.height());
    //QMovie *gif = new QMovie(_fileName);

    //_pImageItem->setPixmap(QPixmap::fromImage(m_Image));
    //QLabel *label = new QLabel;
    //QGraphicsProxyWidget *Widget = new QGraphicsProxyWidget(_pImageItem.get());
    //label->setMovie(gif);
    //QGraphicsScene.add
   // label->setAttribute(Qt::WA_NoSystemBackground);
    //Widget->setWidget(label);
    //_pScene->addWidget(label);
   // gif->start();
//    _pScene->addItem(_pImageItem.get());
//    _pScene->setSceneRect(_pImageItem->boundingRect());
    bestImageGeometry();
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

    if(imgHeight < screenHeight &&
            imgWidth < screenWidth)
    {
        resetTransform();
        zoomFactor = transform().m11();
    }
   else
   {
       fitInView(_pScene->sceneRect(), Qt::KeepAspectRatio);
       zoomFactor = transform().m11();
   }
}

void ScreenImage::zoomImage(const qreal zoomFactor)
{
    scale(zoomFactor, zoomFactor);
}

void ScreenImage::flipImge(const bool horizontal, const bool vertical)
{
    m_Image = m_Image.mirrored(horizontal, vertical);

    emit showImageSignal();
}

void ScreenImage::rotateImage(qreal angle)
{
    QTransform transform;
    transform.rotate(angle);
    m_Image = m_Image.transformed(transform);

    emit showImageSignal();
}
