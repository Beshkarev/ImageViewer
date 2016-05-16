#include "entry.h"
#include "filesystem.h"
#include <QDir>
#include "app_properties.h"

Entry::Entry(const QString &directoryWithFileName) :
    m_dir(FileSystem::absoluteFilePath(directoryWithFileName))
{
    entryList(directoryWithFileName);
}

QString Entry::next()
{
    ++it;
    if(it == m_etryList.cend())
    {
        it = m_etryList.cbegin();
        return (*(it)).absoluteFilePath();
    }
    else
        return (*(it)).absoluteFilePath();
}

QString Entry::previous()
{
    if(it == m_etryList.cbegin())
    {
       it = m_etryList.cend();
       return (*(--it)).absoluteFilePath();
    }
    else
        return (*(--it)).absoluteFilePath();
}

void Entry::entryList(const QString &pathWithFileName)
{
    QDir dir(FileSystem::absoluteFilePath(pathWithFileName));
    m_etryList = dir.entryInfoList(AppProrepties::supportedFormats(),
                                   QDir::Files,
                                   QDir::LocaleAware);
    if (m_etryList.isEmpty())
        throw std::runtime_error("Selected directory is empty.");

    it = std::find(m_etryList.cbegin(), m_etryList.cend(), pathWithFileName);
}
