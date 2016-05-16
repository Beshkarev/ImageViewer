#ifndef APPPROPERTIES
#define APPPROPERTIES

#include <QStringList>

class AppProrepties
{
public:
    AppProrepties(const AppProrepties&) = delete;
    AppProrepties &operator =(const AppProrepties &) = delete;

    static const QString &version();
    static const QString &name();
    static const QStringList &supportedFormats();
    static const QString& lastWorkDirectory();
    static void changeLastWorkDirectory(const QString &directory);

private:
    static const QString _appVersion;
    static const QStringList _supportedFormats;
    static const QString _name;
    static QString _lastWorkDirectory;

    void addRecentFile(const QString &filename);
    void updateListRecentFiles();
    void openRecentFile();
};

inline const QString &AppProrepties::version()
{
    return _appVersion;
}

inline const QString &AppProrepties::name()
{
    return _name;
}

inline const QStringList &AppProrepties::supportedFormats()
{
    return _supportedFormats;
}

inline const QString &AppProrepties::lastWorkDirectory()
{
    return _lastWorkDirectory;
}

#endif // APPPROPERTIES
