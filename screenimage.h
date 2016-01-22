#ifndef SCREENIMAGE
#define SCREENIMAGE

#include <QWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

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
    void showImage(const QImage &img);
    void imageWasChanged();
    void showSomeError(const QString &str);
    void bestImageGeometry();
    void zoomImage(const qreal zoomFactor);
    QSize computeScaledSize(const qint32 src_width,
                             const qint32 src_height);

    QScrollArea *_pScrollArea;
    QLabel *_pLabel;
    QImage m_Image;
    cv::Mat m_matImage;

    const qint32 clockwiseValue;
    const qint32 counterClockwiseValue;

    qint32 angle;

    bool imageChanged;
    const qreal zoomInValue;
    const qreal zoomOutValue;
    qreal zoomFactor;
    //qreal scale;

    QString _fileName;
};

#endif // SCREENIMAGE

