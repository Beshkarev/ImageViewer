#ifndef TABCONTROLLER
#define TABCONTROLLER

#include <QTabWidget>

class ScreenImage;
class QString;

class TabController : public QTabWidget
{
    Q_OBJECT

public:
    TabController(QWidget *parent = 0);
    void createTab();
    void loadFiletoTab(const QString &file);
    void saveFileOpenedInTab(const QString &file);
    void closeImage();
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
    void updateTabNumber();
    bool widgetIsNULL(ScreenImage* wdg) const;
    void updateTabText(const int index, const QString &text);
};

#endif // TABCONTROLLER