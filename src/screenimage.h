#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>
#include <memory>
#include <QLabel>
#include <QScrollArea>

class QImage;
class QString;
class QPoint;

class ScreenImage : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenImage(QWidget *pWd = nullptr);
    //true if image don't loaded
    bool isEmpty() const;
    ~ScreenImage();
public:
    //File menu
    void loadImage(const QString &name);
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
    void imageLoaded();

private:
    void showImage();
    void imageWasChanged();
    void bestImageGeometry();
    void zoomImage(const qreal zoomFactor);
    void flipImge(const bool horizontal, const bool vertical);

    std::unique_ptr<QScrollArea> _pScrollArea;
    std::unique_ptr<QLabel> _pLabel;
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
