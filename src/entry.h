#ifndef ENTRY_H
#define ENTRY_H

#include <QFileInfoList>
#include <memory>

class QString;

class Entry
{
public:
    Entry(const QString &directoryWithFileName);

    QString next();
    QString previous();
    QString workDirectory() const;
    QString absoluteFileName() const;
    void setIterToSelectedFile(const QString &filename);
private:
    void entryList(const QString &pathWithFileName);

    QString m_dir;
    std::shared_ptr<QFileInfoList> m_etryList;
    QFileInfoList::const_iterator it;
};

inline QString Entry::workDirectory() const
{
    return m_dir;
}

inline QString Entry::absoluteFileName() const
{
    return (*it).absoluteFilePath();
}

#endif // ENTRY_H
