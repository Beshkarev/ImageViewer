#include "screenimage.h"
#include <QFileDialog>
#include <QImage>
#include <QBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

using namespace cv;

ScreenImage::ScreenImage(QWidget *pWd /*=0*/): QWidget(pWd),
    _pScrollArea(new QScrollArea(this)),
    _pLabel(new QLabel(this)),
    clockwiseValue(90), counterClockwiseValue(-90),
    angle(clockwiseValue), imageChanged(false),
    zoomInValue(1.25), zoomOutValue(0.8),
    zoomFactor(1.0)

{
    _pScrollArea->setWidget(_pLabel);
    _pScrollArea->setWidgetResizable(true);
    _pScrollArea->hide();

    _pLabel->setAlignment(Qt::AlignCenter);
    //_pLabel->setScaledContents(true);

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
    //bestImageGeometry();
    showImage(m_Image);

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
    showImage(m_Image);
    if(!isEmpty())
        imageWasChanged();
}

void ScreenImage::clockwiseRotate()
{
    QTransform transform;
    transform.rotate(angle);

    m_Image = m_Image.transformed(transform);
    showImage(m_Image);
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
    qDebug("zoomIn");
    //int width = m_Image.width();
    //int height = m_Image.height();

    //QImage img = m_Image.scaled(QSize(width * zoomInFactor, height * zoomInFactor),
    //                    Qt::IgnoreAspectRatio, Qt::FastTransformation);
    zoomFactor *= zoomInValue;
    qDebug() << "zoomFactor in zoomIn" << zoomFactor;
    zoomImage(zoomFactor);
    //Потому что каждый раз увеличивается один и тот же размер изображения
    //И скейл фактор все время один и тот же
    //showImage(img);
    //scale = zoomIn;
    qDebug("zoomIn out");
}

void ScreenImage::zoomOutImage()
{
    //scale = zoomOut;
    zoomFactor *= zoomOutValue;
    qDebug() << "zoomFactor in zoomOut" << zoomFactor;
    zoomImage(zoomFactor);
}

void ScreenImage::fitImage(bool checked)
{
    _pLabel->setScaledContents(checked);
}

void ScreenImage::resizeEvent(QResizeEvent *)
{
    //bestImageGeometry();
    //showImage();
}

void ScreenImage::showImage(const QImage &img)
{
    _pLabel->setPixmap(QPixmap::fromImage(img));
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
    //m_Image = m_Image.scaled(width(), height(),
      //                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //QSize size(1, 1);
    //size.scale(width(), height(), Qt::KeepAspectRatio);
    //_pLabel->resize(size);
}

void ScreenImage::zoomImage(const qreal zoomFactor)
{
    qDebug() << "zoomImage zoomFactor" << zoomFactor;
    const qint32 src_width = m_Image.width();
    const qint32 src_height = m_Image.height();

    QImage img = m_Image.scaled(QSize(src_width, src_height) * zoomFactor,
                                Qt::IgnoreAspectRatio, Qt::FastTransformation);

    //if(img.size() == m_Image.size())
        //showImage(m_Image);
    //else
    showImage(img);
}

QSize ScreenImage::computeScaledSize(const qint32 src_width, const qint32 src_height)
{
    return QSize(src_width, src_height) * zoomFactor;
}
