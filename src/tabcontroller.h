#ifndef TABCONTROLLER
#define TABCONTROLLER

#include <QTabWidget>
#include <QStack>

class ScreenImage;
class QString;

class TabController : public QTabWidget
{
    Q_OBJECT

public:
    TabController(QWidget *parent = 0);
    void createTab();
    void loadFiletoTab(const QString &file);
    void nextFile();
    void previousFile();
    void saveAsFileOpenedInTab(const QString &file);
    void saveFileOpenedInTab();
    void closeImage();
public slots:
    qint32 closeTab(const int index);
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
    void deleteTab(const qint32 index, ScreenImage *wdg);
    void saveIterator();
    void putTochangedFiles(ScreenImage *img);
    ScreenImage *getChangedFiles();
    QStringList::const_iterator getIterator();
    bool hasChangedFiles();

    //QMap<qint32, QStringList::const_iterator> fileIterator;
    QStack<ScreenImage *> changedFiles;
};

#endif // TABCONTROLLER
