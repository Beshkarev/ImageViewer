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
    bool isChanged() const;

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
    //View menu
    void zoomInImage();
    void zoomOutImage();    
    void fitImage();

protected:
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

    void initGIFScene();
    void resetGIFScene();
    void setSceneTo(QGraphicsScene *pScene);
    void setMovieToLabel(QMovie *pMovie);

    std::unique_ptr<QGraphicsScene> _pScene;
    std::unique_ptr<QGraphicsScene> _pGIFScene;
    std::unique_ptr<QGraphicsPixmapItem> _pImageItem;
    std::unique_ptr<QMovie> _pGIFMovie;
    std::unique_ptr<QLabel> _pLabelForGIF;

    QImage m_Image;

    bool gifNeedShow;
    bool gifAlreadyShow;
    bool imageIsLoaded;

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
