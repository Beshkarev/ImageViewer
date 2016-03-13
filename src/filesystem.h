#ifndef FILESYSTEM
#define FILESYSTEM

//#include <QObject>
#include <QHash>
#include <QLinkedList>

class QWidget;
class QString;

class FileSystem
{
public:
    static QString absolutePath(const QString &dir);

    void newDirectory();
    void deleteDirectory();

    QString nextFile();
    QString previousFile();

private:
    void entryList(const QString &dir);
    bool entryIsExist();


    QHash<QWidget*, QString> directorys;

    QLinkedList<QStringList> _filesInDirectory;
    QHash<QWidget*, QStringList::const_iterator> _itToEntryList;



};

#endif
