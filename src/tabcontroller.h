#ifndef TABCONTROLLER
#define TABCONTROLLER

#include <QTabWidget>
#include <QHash>
#include <QFileInfoList>

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
    void next();
    void previous();
    void open();
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

    ScreenImage *getImageWidget();
    void updateTabNumber();
    bool widgetIsNULL(ScreenImage* wdg) const;
    void updateTabText(const int index, const QString &text);
    void deleteTab(const qint32 index);

    void addWorkDirectory(const QString &dir);
    bool workDirIsChanged(const QString &dir);
    void entryList(const QString &dir);
    bool entryIsExist(const QString &dir);
    void createIterator();
    QString workDirectory() const;

    QHash<QWidget*, QString> directorys;
    QHash<QString, QFileInfoList> _entries;
    QHash<QWidget*, QList<QFileInfo>::const_iterator> _iteratots;
};

#endif // TABCONTROLLER
