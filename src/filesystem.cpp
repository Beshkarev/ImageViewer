#include "filesystem.h"

#include <QDebug>
#include <QFileInfo>

QString FileSystem::absolutePath(const QString &dir)
{
    QFileInfo path(dir);

    //qDebug() << "path:" << path.absolutePath();

    return path.absolutePath();
}

void FileSystem::newDirectory()
{

}

QString FileSystem::nextFile()
{



}

QString FileSystem::previousFile()
{

}
