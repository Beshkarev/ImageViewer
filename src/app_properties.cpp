#include "app_properties.h"
#include <QString>
#include "filesystem.h"
#include <QDir>
#include <QSettings>

constexpr str_const AppProperties::_appVersion;

const QStringList AppProperties::_supportedFormats = QStringList() << "*.jpg" << "*.bmp"
                                                               << "*.png" << "*jpeg";

constexpr str_const AppProperties::_name;

QString AppProperties::_lastWorkDirectory;

const QString AppProperties::_tempLocation = QDir::tempPath() + "/image_viewer_tmp";

QStringList AppProperties::_recentFiles;

void AppProperties::changeLastWorkDirectory(const QString &directory)
{
    _lastWorkDirectory = FileSystem::absoluteFilePath(directory);
}

void AppProperties::addRecentFile(const QString &filename)
{
    if(!filename.isEmpty())
    {
        _recentFiles.removeAll(filename);
        _recentFiles.prepend(filename);
    }
}

void AppProperties::saveSettings()
{
    QSettings settings("Education", AppProperties::name());

    settings.setValue("files/recentFiles", AppProperties::recentFiles());
    settings.setValue("dir/lastDirectory", AppProperties::lastWorkDirectory());
}

void AppProperties::readSettings()
{
    QDir dir;
    dir.setPath(tempLocation());
    bool success =  dir.mkpath(tempLocation());
    if (!success)
        throw std::runtime_error("Can't create temp directory");

    dir.cd("image_viewer_tmp");

    QSettings settings("Education", name());

    _recentFiles = settings.value("files/recentFiles").toStringList();
    _lastWorkDirectory = settings.value("dir/lastDirectory").toString();
}
