#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>
//#include <QFileInfoList>

class QString;
class TabController;
class QFileInfo;
class QWidget;
//class QFileInfoList;

class FileSystem
{
public:
    FileSystem();

    static QString absolutePath(const QString &dir);
    static QString fileName(const QString &file);

    void newDirectory();
    void deleteDirectory();

    QString openFile();
    QString nextFile();
    QString previousFile();
    bool saveFile();
    bool saveAs();

private:
    TabController *_pTabs;

    void addWorkDirectory(const QString &dir);
    bool workDirIsChanged(const QString &dir);
    void entryList(const QString &dir);
    bool entryIsExist(const QString &dir);
    void createIterator(const QString &file);
    QString workDirectory() const;

    QString getCurrentFileName();
    bool saveToDisk(const QString &locationForSaving);

    QHash<QWidget*, QString> directorys;
    QHash<QString, QList<QFileInfo>> _entries;
    QHash<QWidget*, QList<QFileInfo>::const_iterator> _iteratots;
};

#endif
