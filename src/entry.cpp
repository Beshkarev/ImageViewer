#include "entry.h"
#include "filesystem.h"
#include "config.h"
#include <QDir>
#include <QApplication>

Entry::Entry(const QString &directoryWithFileName) :
    m_dir(FileSystem::absoluteFilePath(directoryWithFileName)),
    m_etryList(std::make_shared<QFileInfoList>())
{
    entryList(directoryWithFileName);
}

QString Entry::next()
{
    ++it;
    if(it == m_etryList->cend())
    {
        it = m_etryList->cbegin();
        return (*(it)).absoluteFilePath();
    }
    else
        return (*(it)).absoluteFilePath();
}

QString Entry::previous()
{
    if(it == m_etryList->cbegin())
    {
       it = m_etryList->cend();
       return (*(--it)).absoluteFilePath();
    }
    else
        return (*(--it)).absoluteFilePath();
}

void Entry::setIterToSelectedFile(const QString &filename)
{
    it = std::find(m_etryList->cbegin(), m_etryList->cend(), filename);
}

void Entry::entryList(const QString &pathWithFileName)
{
    QApplication::processEvents();

    QDir dir(FileSystem::absoluteFilePath(pathWithFileName));
    *m_etryList = dir.entryInfoList(Config::supportedFormats,
                                   QDir::Files,
                                   QDir::LocaleAware);
    if (m_etryList->isEmpty())
        throw std::runtime_error(QObject::tr("Selected directory is empty.").toStdString());

    setIterToSelectedFile(pathWithFileName);
}
