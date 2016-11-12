#ifndef CONFIG_H
#define CONFIG_H

#include <QStringList>
#include "basic_types.h"

namespace Config
{
    constexpr str_const appName = "Image viewer";
    constexpr str_const version = "1.0.1";

    extern const QString tempLocation;
    extern QStringList recentFiles;
    extern QString lastWorkDirectory;

    QStringList supportedReadMimeTypes();
    QStringList supportedSaveMimeTypes();
    QStringList supportedReadFormats();
    QStringList supportedSaveFormats();
    void saveSettings();
    void readSettings();
    void clearTempDir();

    void changeLastWorkDirectory(const QString &directory);
    void addRecentFile(const QString &filename);
}

#endif
