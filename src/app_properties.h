#ifndef APPPROPERTIES
#define APPPROPERTIES

#include <QStringList>

class QString;

class AppProrepties
{
public:
    AppProrepties();
    QString version() const;
    void setVersion(const QString &ver);
    void setAppName(const QString &name);
    QString name() const;

private:
    QString _appVersion;
    QStringList _supportedFormats;
    QString _name;

    void addRecentFile(const QString &filename);
    void updateListRecentFiles();
    void openRecentFile();
};

inline QString AppProrepties::version() const
{
    return _appVersion;
}

inline QString AppProrepties::name() const
{
    return _name;
}

#endif // APPPROPERTIES
