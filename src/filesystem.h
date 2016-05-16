#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>
#include <memory>
#include "tabcontroller.h"

class QString;
class QWidget;
class Entry;

class FileSystem
{
    FileSystem();
    FileSystem(const FileSystem &) = delete;
    FileSystem &operator = (const FileSystem &) = delete;

public:
    static FileSystem *instance();
    static QString absoluteFilePath(const QString &dir);
    static QString fileName(const QString &file);

    void destroyEntry(QWidget *widg);
    QString openFile();
    QString nextFile() const;
    QString previousFile() const;
    bool saveFile();
    bool saveAs();

private:
    static FileSystem *_pInstance;
    std::unique_ptr<TabController> _pTabs;

    void setWorkDirectory(const QString &directory);
    void createEntry(const QString &dir);
    bool entryIsExist(const QString &dir) const;

    QString getCurrentAbsoluteFileName() const;
    bool saveToDisk(const QString &locationForSaving);

    QHash<QString, QWidget*> _directories;
    QHash<QWidget*, std::shared_ptr<Entry> > _entries;
};

#endif
