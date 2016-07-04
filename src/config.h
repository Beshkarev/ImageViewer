#ifndef CONFIG_H
#define CONFIG_H

#include <QStringList>
#include "basic_types.h"
#include <QDir>

namespace Config
{
    constexpr str_const appName = "Image viewer";
    constexpr str_const version = "0.9";

    const QStringList supportedFormats = QStringList() << "*.jpg" << "*.bmp"
                                                       << "*.png" << "*jpeg";
    const QString tempLocation = QDir::tempPath() + "/image_viewer_tmp";
    extern QStringList recentFiles;
    extern QString lastWorkDirectory;

    void saveSettings();
    void readSettings();
    void clearTempDir();

    void changeLastWorkDirectory(const QString &directory);
    void addRecentFile(const QString &filename);
}

#endif
