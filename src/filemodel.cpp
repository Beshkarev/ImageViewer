#include "filemodel.h"
#include "tabcontroller.h"
#include <QStringList>
#include <QDir>
#include <QHash>

FileModel *FileModel::_instance = nullptr;

FileModel::FileModel() : _tabController(new TabController)
{}

FileModel *FileModel::instance()
{
    if(_instance == nullptr)
        _instance = new FileModel;
    return _instance;
}

QString FileModel::getFileForLoad() const
{

}

QString FileModel::nextFile()
{

}

bool FileModel::isCurrentPathChanged(const QString &path)
{
    if(_currentPath != path)
        entryList(path);
}

void FileModel::entryList(const QString &path)
{
    QStringList supportedFormats;
    QStringList filesList;
    supportedFormats << "*.jpg" << "*.bmp" << "*.png";
    QDir dir(path);
    filesList = dir.entryList(supportedFormats, QDir::Files,
                              QDir::LocaleAware);

    _entrysList.insert(_tabController->currentIndex(),
                       filesList);
}
