#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>
#include <QMap>

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
    bool loadImage(const QString &file);
    void saveImage(const QString &filename) const;
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
    void resizeEvent(QResizeEvent *);
private:
    void showImage();
    void imageWasChanged();
    void showSomeError(const QString &title, const QString &str);
    void bestImageGeometry();
    void zoomImage(const qreal zoomFactor);
    void flipImge(const bool horizontal, const bool vertical);
    //void addChangedImageToMemory(const QString &name,
    //                             const QImage &img);
    //bool thisImageWasChanged(const QString &key);

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
    static QMap<QString, QImage> imageChangedContainer;
};

#endif // SCREENIMAGE
