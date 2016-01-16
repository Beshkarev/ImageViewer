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
    void closeTab(const int index);
    void horizontalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
private:
    ScreenImage *getImageWidget();



    //QTabWidget *_pTabWidget;
};

#endif // TABWIDGET
