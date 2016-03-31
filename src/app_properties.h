#ifndef APPPROPERTIES
#define APPPROPERTIES

#include <QStringList>

class QString;

class AppProrepties
{
public:
    AppProrepties();
    QString version();

private:
    QString _appVersion;
    QStringList _supportedFormats;

    void addRecentFile(const QString &filename);
    void updateListRecentFiles();
    void openRecentFile();
};

#endif // APPPROPERTIES
