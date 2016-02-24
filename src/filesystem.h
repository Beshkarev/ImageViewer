#ifndef FILESYSTEM
#define FILESYSTEM

#include <QObject>
#include <QHash>
#include <QLinkedList>

class QString;

class FileSystem : public QObject
{
    Q_OBJECT

private:
    FileSystem();
    FileSystem& operator =(FileSystem&);
public:
    static FileSystem *instance();
    static QString absolutePath(const QString &file);
    QString nextFile();
    QString previousFile();

private:
    void entryList(const QString &dir);
    bool entryIsExist();

    static FileSystem *_instance;


    QLinkedList<QStringList> _filesInDirectory;
    QHash<QWidget*, QStringList::const_iterator> _itToEntryList;



};

#endif
