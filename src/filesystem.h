#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>
#include <QFileInfoList>

class QString;
class TabController;

class FileSystem
{
public:
    FileSystem();

    static QString absolutePath(const QString &dir);

    void newDirectory();
    void deleteDirectory();

    QString openFile();
    QString nextFile();
    QString previousFile();
    QString saveAs();

private:
    TabController *_pTabs;

    void addWorkDirectory(const QString &dir);
    bool workDirIsChanged(const QString &dir);
    void entryList(const QString &dir);
    bool entryIsExist(const QString &dir);
    void createIterator();
    QString workDirectory() const;

    QString getCurrentFileName();

    QHash<QWidget*, QString> directorys;
    QHash<QString, QFileInfoList> _entries;
    QHash<QWidget*, QList<QFileInfo>::const_iterator> _iteratots;



};

#endif
