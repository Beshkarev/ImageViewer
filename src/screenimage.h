#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>

class QImage;
class QLabel;
class QScrollArea;
class QString;

class ScreenImage : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenImage(QWidget *pWd = 0);
    bool isChanged() const;
    bool isEmpty() const;
    QString getFileName() const;
public:
    //File menu
    bool loadImage(const QString &filename);
    void saveImage(const QString &filename) const;
    void closeImage();
    //Edit menu
    void horizontalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    //View menu
    void fitImage(bool checked);
protected:
    void resizeEvent(QResizeEvent *);
private:
    void showImage();
    void imageWasChanged();
    void showSomeError(const QString &str);
    void bestImageGeometry();
    void zoomImage(const qreal zoomFactor);

    QScrollArea *_pScrollArea;
    QLabel *_pLabel;
    QImage m_Image;

    const qint32 clockwiseValue;
    const qint32 counterClockwiseValue;

    qint32 angle;

    bool imageChanged;
    const qreal zoomInValue;
    const qreal zoomOutValue;
    qreal zoomFactor;

    QString _fileName;
};

#endif // SCREENIMAGE

