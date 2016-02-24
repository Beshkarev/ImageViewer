#include "filesystem.h"
#include "tabcontroller.h"
#include <QStringList>
#include <QDir>

FileSystem *FileSystem::_instance = nullptr;

FileSystem::FileSystem()
{}

FileSystem *FileSystem::instance()
{
    if(_instance == nullptr)
        _instance = new FileSystem;
    return _instance;
}

QString FileSystem::absolutePath(const QString &file)
{
    return QDir(file).absolutePath();
}

QString FileSystem::nextFile()
{



}

QString FileSystem::previousFile()
{

}
