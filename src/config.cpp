#include "config.h"
#include "filesystem.h"

#include <QSettings>
#include <thread>
#include <QImageReader>
#include <QImageWriter>
#include <QDir>

const QString Config::tempLocation = QDir::tempPath() + "/image_viewer_tmp/";

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

QStringList Config::supportedReadMimeTypes()
{
    const auto &types = QImageReader::supportedMimeTypes();
    QStringList list;
    list.append("application/octet-stream");
    std::for_each(types.begin(), types.end(),
                  [&list](const QByteArray &name) mutable
    {
        list.append(name);
    });

    return list;
}

QStringList Config::supportedSaveMimeTypes()
{
    const auto &types = QImageWriter::supportedMimeTypes();
    QStringList list;
    std::for_each(types.cbegin(), types.cend(),
                  [&list](const QByteArray &name) mutable
    {
        list.append(name);
    });

    list.removeOne("image/jp2");

    return list;
}

QStringList Config::supportedReadFormats()
{
    const auto &types = QImageReader::supportedImageFormats();
    QStringList list;

    std::for_each(types.cbegin(), types.cend(),
                  [&list](const QByteArray &name) mutable
    {
        list.append("*." + name);
    });
    list.append("*.jpe");
    list.append("*.jpx");
    list.append("*.jpf");
    list.append("*.icb");
    list.append("*.tpic");
    list.append("*.vda");
    list.append("*.vst");

    list.removeOne("*.svgz");
    list.removeOne("*.cur");

    return list;
}

QStringList Config::supportedSaveFormats()
{
    const auto &types = QImageWriter::supportedImageFormats();
    QStringList list;
    std::for_each(types.cbegin(), types.cend(),
                  [&list](const QByteArray &name) mutable
    {
        list.append("*." + name);
    });

    list.removeOne("*.cur");
//    list.removeOne("*.ico");

    return list;
}
