#ifndef TABCONTROLLER
#define TABCONTROLLER

#include <QTabWidget>
#include <QHash>

class ScreenImage;
class QString;

class TabController : public QTabWidget
{
    Q_OBJECT

public:
    TabController(QWidget *parent = 0);
    void createTab();
    void loadFiletoTab(const QString &file);
    void saveAsFileOpenedInTab(const QString &file);
    void saveFileOpenedInTab();
    void closeImage();
    QString workDirectory(const qint32 index)const;
    QWidget *getCurrentWidget();
public slots:
    void closeTab(const int index);
public:
    void horizontalFlip();
    void verticalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    void fitImage();
private:
    ScreenImage *getImageWidget();
    void updateTabNumber();
    bool widgetIsNULL(ScreenImage* wdg) const;
    void updateTabText(const int index, const QString &text);
    void deleteTab(const qint32 index);
    void addWorkDirectory(QWidget *wdg, const QString &file);

    QHash<QWidget*, QString> directorys;
};

#endif // TABCONTROLLER
