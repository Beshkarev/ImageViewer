#ifndef ENTRY_H
#define ENTRY_H

#include <QFileInfoList>

class QString;

class Entry
{
public:
    Entry(const QString &directoryWithFileName);

    QString next();
    QString previous();
    const QString& workDirectory() const;
    QString fileName() const;
private:
    void entryList(const QString &pathWithFileName);

    QString m_dir;
    QFileInfoList m_etryList;
    QFileInfoList::const_iterator it;
};

inline const QString& Entry::workDirectory() const
{
    return m_dir;
}

inline QString Entry::fileName() const
{
    return (*it).fileName();
}

#endif // ENTRY_H
