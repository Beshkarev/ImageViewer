#ifndef APPPROPERTIES
#define APPPROPERTIES

#include <QStringList>
#include "basic_types.h"

class AppProperties
{
public:
    AppProperties(const AppProperties&) = delete;
    AppProperties &operator =(const AppProperties &) = delete;

    static const QString version();
    static const QString name();
    static const QStringList &supportedFormats();
    static const QString& lastWorkDirectory();
    static void changeLastWorkDirectory(const QString &directory);
    static const QString &tempLocation();
    static QStringList& recentFiles();
    static void addRecentFile(const QString &filename);

    static void saveSettings();
    static void readSettings();

private:
    static constexpr str_const _appVersion = "0.9";
    static const QStringList _supportedFormats;
    static constexpr str_const _name = "Image viewer";
    static QString _lastWorkDirectory;
    static const QString _tempLocation;
    static QStringList _recentFiles;
};

inline const QString AppProperties::version()
{
    return str_const_toString(_appVersion);
}

inline const QString AppProperties::name()
{
    return str_const_toString(_name);
}

inline const QStringList &AppProperties::supportedFormats()
{
    return _supportedFormats;
}

inline const QString &AppProperties::lastWorkDirectory()
{
    //_lastWorkDirectory = "/";
    return _lastWorkDirectory;
}

inline const QString &AppProperties::tempLocation()
{
    return _tempLocation;
}

inline QStringList &AppProperties::recentFiles()
{
    return _recentFiles;
}

#endif // APPPROPERTIES
