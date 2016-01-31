#ifndef FILEMODEL
#define FILEMODEL

#include <QHash>

class QString;
class TabController;

class FileModel
{
protected:
    FileModel();
public:
    static FileModel *instance();
    QString getFileForLoad() const;
    QString nextFile();
    QString previousFile();
private:
    bool isCurrentPathChanged(const QString &path);
    void entryList(const QString &path);

    static FileModel *_instance;
    QHash<int, QStringList> _entrysList;
    QHash<int, QStringList::const_iterator> _iteratorsContainer;
    QString _currentPath;
    TabController *_tabController;
};

#endif // FILEMODEL

