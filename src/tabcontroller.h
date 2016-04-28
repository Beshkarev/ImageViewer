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
    ~TabController();
    static void destroyInstance();

    TabController(const TabController &) = delete;
    void operator=(const TabController &) = delete;
public:
    static TabController *instance();

    void createTab();
    void loadFiletoTab(const QString &file);
    void closeImage();
    bool currentTabIsEmpty();
public slots:
    void closeTab(const int index);
signals:
    void tabClosed();
    void tabCreated();
    void tabStateChanged();
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
    void updateTabText(const int index, const QString &text);
    void deleteTab(const qint32 index);
};

#endif // TABCONTROLLER
