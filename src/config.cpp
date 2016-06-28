#include "config.h"
#include "filesystem.h"

#include <QDir>
#include <QSettings>
#include <thread>

QStringList Config::recentFiles;

QString Config::lastWorkDirectory;

void Config::saveSettings()
{
    std::thread thread(clearTempDir);
    thread.detach();

    QSettings settings("Beshkarev", str_const_toString(appName));

    settings.setValue("files/recentFiles", recentFiles);
    settings.setValue("dir/lastDirectory", lastWorkDirectory);
}

void Config::readSettings()
{
    QDir dir;
    dir.setPath(tempLocation);
    bool success =  dir.mkpath(tempLocation);
    if (!success)
        throw std::runtime_error(QObject::tr("Can't create temp directory").toStdString());

    QSettings settings("Beshkarev", str_const_toString(appName));

    recentFiles = settings.value("files/recentFiles").toStringList();
    lastWorkDirectory = settings.value("dir/lastDirectory").toString();
}

void Config::clearTempDir()
{
    QDir dir(tempLocation);

    auto allFiles = dir.entryList();
    for (auto file : allFiles)
        dir.remove(file);
}

void Config::changeLastWorkDirectory(const QString &directory)
{
    lastWorkDirectory = FileSystem::absoluteFilePath(directory);
}

void Config::addRecentFile(const QString &filename)
{
    recentFiles.removeAll(filename);
    recentFiles.prepend(filename);
}
