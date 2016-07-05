#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <memory>
#include <QGraphicsView>

class QImage;
class QString;
class QPoint;
class QMovie;
class QLabel;

class ScreenImage : public QGraphicsView
//class ScreenImage : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenImage(QWidget *pWd = nullptr);
    ~ScreenImage();
    //true if image don't loaded
    bool isEmpty() const;

public:
    //File menu
    void loadImage(const QString &name);
    void loadGIF(const QString &name);
    void closeImage();
    //Edit menu
    void horizontalFlip();
    void verticalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    //View menu
    void fitImage();

protected:
    void resizeEvent(QResizeEvent *) override;
    void wheelEvent(QWheelEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

signals:
    void showImageSignal();
    void imageLoaded();

private slots:
    void showImage();

private:
    void imageWasChanged();
    void bestImageGeometry();
    void zoomImage(const qreal zoomFactor);
    void flipImge(const bool horizontal, const bool vertical);
    void rotateImage(qreal angle);

    std::unique_ptr<QGraphicsScene> _pScene;
    std::unique_ptr<QGraphicsPixmapItem> _pImageItem;
    std::unique_ptr<QMovie> _pGIFMovie;
    std::unique_ptr<QLabel> _pLabelForGIF;
    QImage m_Image;

    const qint32 clockwiseValue;
    const qint32 counterClockwiseValue;

    qint32 angle;

    bool imageChanged;
    const qreal zoomInValue;
    const qreal zoomOutValue;
    qreal zoomFactor;

    QString _fileName;
    QPoint _mousePosition;
};

#endif // SCREENIMAGE
