#include "screenimage.h"
#include "saveconfirmation.h"

#include <QImage>
#include <QScrollBar>
#include <QDebug>
#include <QMouseEvent>
#include <thread>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QMovie>

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QGraphicsView(pWd),
    _pScene(new QGraphicsScene),
    _pGIFScene(nullptr), _pGIFMovie(nullptr), _pLabelForGIF(nullptr),
    gifNeedShow(false), gifAlreadyShow(false),
    clockwiseValue(90), counterClockwiseValue(-90),
    angle(0), imageChanged(false),
    zoomInValue(1.1), zoomOutValue(1.1),
    zoomFactor(1.0)

{
    connect(this, SIGNAL(showImageSignal()),
            this, SLOT(showImage()));

    _pImageItem = std::unique_ptr<QGraphicsPixmapItem>(_pScene->addPixmap(QPixmap()));

    setMouseTracking(true);
    _pImageItem->setTransformationMode(Qt::SmoothTransformation);

    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setSceneTo(_pScene.get());
}

ScreenImage::~ScreenImage() = default;

bool ScreenImage::isEmpty() const
{
    return m_Image.isNull();
}

bool ScreenImage::isGIF() const
{
    return gifNeedShow;
}

void ScreenImage::loadImage(const QString &name)
{
    if(SaveConfirmation::imageWasChanged(name))
        m_Image = SaveConfirmation::getChagedImage(name);
    else
        m_Image.load(name);

    _fileName = name;
    imageChanged = false;
    gifNeedShow = false;

    emit imageLoaded();
    emit showImageSignal();
}

void ScreenImage::loadGIF(const QString &name)
{
    m_Image.load(name);
    if(!_pGIFScene)
        initGIFScene();  
    else
        _pGIFMovie->stop();

    _pGIFMovie->setFileName(name);
    _pLabelForGIF->resize(m_Image.size());

    gifNeedShow = true;

    emit imageLoaded();
    emit showImageSignal();
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
//    QApplication::processEvents();
    if(!gifAlreadyShow && gifNeedShow)
    {
        //qDebug("!gifAlreadyShow && gifNeedShow");
        setSceneTo(_pGIFScene.get());

        _pGIFScene->setSceneRect(0, 0, m_Image.width(), m_Image.height());
        _pGIFMovie->start();

        gifAlreadyShow = true;
    }
    else if(gifAlreadyShow && gifNeedShow)
    {
        //qDebug("gifAlreadyShow && gifNeedShow");;
        _pGIFScene->setSceneRect(0, 0, m_Image.width(), m_Image.height());

        _pGIFMovie->start();
    }
    else if(gifAlreadyShow && !gifNeedShow)
    {
        //qDebug("gifAlreadyShow && !gifNeedShow");
        setSceneTo(_pScene.get());
        resetGIFScene();

        _pScene->setSceneRect(0, 0, m_Image.width(), m_Image.height());
        _pImageItem->setPixmap(QPixmap::fromImage(m_Image));

        gifAlreadyShow = false;
    }
    else
    {
        //qDebug("else");
        _pScene->setSceneRect(0, 0, m_Image.width(), m_Image.height());
        _pImageItem->setPixmap(QPixmap::fromImage(m_Image));
    }

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
       fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
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

void ScreenImage::initGIFScene()
{
    _pGIFMovie = std::unique_ptr<QMovie>(new QMovie);
    _pLabelForGIF = std::unique_ptr<QLabel>(new QLabel);
    _pLabelForGIF->setAttribute(Qt::WA_NoSystemBackground);
    _pLabelForGIF->setAlignment(Qt::AlignCenter);
    _pLabelForGIF->setScaledContents(true);
    _pLabelForGIF->setMovie(_pGIFMovie.get());

    _pGIFScene = std::unique_ptr<QGraphicsScene>(new QGraphicsScene);
    _pGIFScene->addWidget(_pLabelForGIF.get());
}

void ScreenImage::resetGIFScene()
{
    _pGIFMovie.reset();
    _pLabelForGIF.reset();
    _pGIFScene->clear();
    _pGIFScene.reset();
}

void ScreenImage::setSceneTo(QGraphicsScene *pScene)
{
    qDebug("setScene");
    setScene(pScene);
}

void ScreenImage::setMovieToLabel(QMovie *pMovie)
{
    _pLabelForGIF->setMovie(pMovie);
}
