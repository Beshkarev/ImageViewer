#ifndef APPPROPERTIES
#define APPPROPERTIES

#include <QStringList>
#include "basic_types.h"


struct AppProrepties
{
public:
    AppProrepties(const AppProrepties&) = delete;
    AppProrepties &operator =(const AppProrepties &) = delete;

    static const QString version();
    static const QString name();
    static const QStringList &supportedFormats();
    static const QString& lastWorkDirectory();
    static void changeLastWorkDirectory(const QString &directory);
    static const QString &tempLocation();
    static QStringList& recentFiles();
    static void addRecentFile(const QString &filename);

private:
    static constexpr str_const _appVersion = "0.9";
    static const QStringList _supportedFormats;
    static constexpr str_const _name = "Image viewer";
    static QString _lastWorkDirectory;
    static const QString _tempLocation;
    static QStringList _recentFiles;
};

inline const QString AppProrepties::version()
{
    return str_const_toString(_appVersion);
}

inline const QString AppProrepties::name()
{
    return str_const_toString(_name);
}

inline const QStringList &AppProrepties::supportedFormats()
{
    return _supportedFormats;
}

inline const QString &AppProrepties::lastWorkDirectory()
{
    return _lastWorkDirectory;
}

inline const QString &AppProrepties::tempLocation()
{
    return _tempLocation;
}

inline QStringList &AppProrepties::recentFiles()
{
    return _recentFiles;
}

#endif // APPPROPERTIES
