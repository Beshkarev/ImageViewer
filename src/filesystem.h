#ifndef FILESYSTEM
#define FILESYSTEM

#include <QHash>
#include <memory>
#include "tabcontroller.h"
#include "entry.h"

class QString;
class QWidget;

class FileSystem : public QObject
{
    Q_OBJECT

    FileSystem(QObject *parent = nullptr);
    FileSystem(const FileSystem &) = delete;
    FileSystem &operator = (const FileSystem &) = delete;

public:
    static FileSystem *instance();
    static QString absoluteFilePath(const QString &dir);
    static QString fileName(const QString &file);

    void destroyEntry(QWidget *widg);
    QString openFile();
    QString nextFile();
    QString previousFile();
    bool saveFile();
    bool saveAs();

private:
    static FileSystem *_pInstance;
    std::unique_ptr<TabController> _pTabs;

    void setWorkDirectory(const QString &directory);
    void createEntry(const QString &dir);
    bool entryIsExist(const QString &dir) const;
    void checkSelectedFileIsSupported(const QString &selectedFile) const;

    QString getCurrentAbsoluteFileName();
    bool saveToDisk(const QString &locationForSaving);

    QHash<QString, QWidget*> _directories;
    QHash<QWidget*, Entry> _entries;
};

#endif
