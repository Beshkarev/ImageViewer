#ifndef TABWIDGET
#define TABWIDGET

#include <QTabWidget>

class ScreenImage;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = 0);
    void createTab();
    void loadFiletoTab();
    void saveFileOpenedInTab();
public slots:
    void closeTab(const int index);
public:
    void horizontalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    void fitImage(bool);
private:
    ScreenImage *getImageWidget();



    //QTabWidget *_pTabWidget;
};

#endif // TABWIDGET
