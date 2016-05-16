#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>
#include <memory>

class QString;
class TabController;
class QFileInfo;
class QWidget;
class Entry;

class FileSystem
{
    FileSystem();
    ~FileSystem();
    FileSystem(const FileSystem &) = delete;
    FileSystem &operator = (const FileSystem &) = delete;

    static void destroyInstance();
public:
    static FileSystem *instance();
    static QString absoluteFilePath(const QString &dir);
    static QString fileName(const QString &file);

    QString openFile();
    QString nextFile() const;
    QString previousFile() const;
    bool saveFile();
    bool saveAs();

private:
    static FileSystem *_pInstance;
    TabController *_pTabs;
    QString lastDir;

    void setWorkDirectory(const QString &directory);
    void createEntry(const QString &dir);
    bool entryIsExist(const QString &dir) const;
    const QString &workDirectory() const;

    QString getCurrentFileName() const;
    bool saveToDisk(const QString &locationForSaving);

    QHash<QWidget*, QString> _directorys;
    QHash<QString, std::shared_ptr<Entry> > _entries;
};

#endif
