#ifndef TABCONTROLLER
#define TABCONTROLLER

#include <QTabWidget>

class ScreenImage;
class QString;

class TabController : public QTabWidget
{
    Q_OBJECT

private:
    TabController(QWidget *parent = 0);

public:
    TabController(TabController const &) = delete;
    void operator=(TabController const &) = delete;
    static TabController *instance();

    void createTab();
    void loadFiletoTab(const QString &file);
    void closeImage();
public slots:
    void closeTab(const int index);
signals:
    void tabClosed();
    void tabCreated();
public:
    void horizontalFlip();
    void verticalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    void fitImage();
private:
    static TabController *_instance;

    ScreenImage *getImageWidget();
    void updateTabNumber();
    bool widgetIsNULL(ScreenImage* wdg) const;
    void updateTabText(const int index, const QString &text);
    void deleteTab(const qint32 index);
};

#endif // TABCONTROLLER
