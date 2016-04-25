#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>

class QString;
class TabController;
class QFileInfo;
class QWidget;

class FileSystem
{
    FileSystem();
    ~FileSystem();

public:
    FileSystem(FileSystem const &) = delete;
    FileSystem& operator = (FileSystem const &) = delete;

    static FileSystem *instance();
    static QString absolutePath(const QString &dir);
    static QString fileName(const QString &file);

    QString openFile();
    QString nextFile();
    QString previousFile();
    bool saveFile();
    bool saveAs();

private:
    static FileSystem *_pInstance;
    TabController *_pTabs;
    QString lastDir;

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
