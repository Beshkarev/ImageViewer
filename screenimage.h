#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>

class QPixmap;
class QLabel;
class QScrollArea;
class QString;

class ScreenImage : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenImage(QWidget *pWd = 0);
    bool isChanged();
    bool isEmpty();
    QString getFileName();
public:
    //File menu
    bool loadImage();
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

    const qint32 clockwiseValue;
    const qint32 counterClockwiseValue;

    qint32 angle;

    bool imageChanged;
    const qreal zoomIn;
    const qreal zoomOut;
    qreal scale;

    QString _fileName;
};

#endif // SCREENIMAGE

