#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>

class QPixmap;
class QLabel;
class QScrollArea;

class ScreenImage : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenImage(QWidget *pWd = 0);
    bool isChanged();
    bool isEmpty();
public slots:
    //File menu
    void loadImage();
    void saveImage();
    void closeImage();
    //Edit menu
    void horizontalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
private:
    QImage QimageFactory();
    void showImage();
    void somethingChanged();
    void showSomeError(const QString &str);

    QScrollArea *_pScrollArea;
    QLabel *_pLabel;
    QPixmap *_pPixmap;

    qint32 angle;
    enum anglevalue
    {
        clockwiseValue = 90,
        counterClockwiseValue = -90
    };
    bool imageChanged;
    const qreal zoomIn;
    const qreal zoomOut;
    qreal scale;
};

#endif // SCREENIMAGE

