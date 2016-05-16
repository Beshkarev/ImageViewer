#include "filesystem.h"
#include "tabcontroller.h"
#include "saveconfirmation.h"
#include "entry.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QFileDialog>
#include <QObject>
#include <QImage>

FileSystem *FileSystem::_pInstance;

FileSystem::FileSystem(): _pTabs(TabController::instance())
{}
FileSystem::~FileSystem()
{}
void FileSystem::destroyInstance()
{
    qDebug() << "file system destroy";

    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

FileSystem *FileSystem::instance()
{
    if(!_pInstance)
    {
        _pInstance = new FileSystem;
        atexit(& FileSystem::destroyInstance);
    }

    return _pInstance;
}

QString FileSystem::absoluteFilePath(const QString &dir)
{
    QFileInfo path(dir);
    return path.absolutePath();
}

QString FileSystem::fileName(const QString &file)
{
    return QFileInfo(file).fileName();
}

QString FileSystem::openFile()
{
    if(lastDir.isEmpty())
        lastDir = QDir::homePath() + "/Pictures";
    QString filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open file"),
                                                    lastDir,
                                                    QObject::tr("All (*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg;;"));
    if(filename.isEmpty())
        return QString();

    setWorkDirectory(filename);
    createEntry(filename);

    lastDir = FileSystem::absoluteFilePath(filename);

    return filename;
}

QString FileSystem::nextFile() const
{
    Entry *en = _entries.find(workDirectory()).value().get();

    return en->next();
}

QString FileSystem::previousFile() const
{
      Entry *en = _entries.find(workDirectory()).value().get();

      return en->previous();
}

bool FileSystem::saveFile()
{
    return saveToDisk(getCurrentFileName());
}

bool FileSystem::saveAs()
{
    const QString filename = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save file"),
                                                    getCurrentFileName(),
                                                    QObject::tr("All(*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg;;"));
    return saveToDisk(filename);
}

void FileSystem::setWorkDirectory(const QString &directory)
{
    _directorys.insert(_pTabs->currentWidget(),
                       FileSystem::absoluteFilePath(directory));
}

void FileSystem::createEntry(const QString &dir)
{
    if(!entryIsExist(dir))
    {
        qDebug() <<"entry don't exist";
        _entries.insert(workDirectory(),
                        std::make_shared<Entry>(dir));
    }
    else
    {
        _entries.insert(workDirectory(),
                        _entries.find(workDirectory()).value());
//        qDebug() << _entries.find(workDirectory()).value().use_count();
    }
}

bool FileSystem::entryIsExist(const QString &dir) const
{
    auto it = _entries.find(FileSystem::absoluteFilePath(dir));
    if (it == _entries.end())
        return false;
    else
        return true;
}

const QString &FileSystem::workDirectory() const
{
    return _directorys.find(_pTabs->currentWidget()).value();
}

QString FileSystem::getCurrentFileName() const
{
    Entry *en = _entries.find(workDirectory()).value().get();

    return en->fileName();
}

bool FileSystem::saveToDisk(const QString &locationForSaving)
{
    QImage img(getCurrentFileName());
    bool success;
    if(SaveConfirmation::imageWasChanged(getCurrentFileName()))
    {
        img = SaveConfirmation::getChagedImage(getCurrentFileName());
        success = img.save(locationForSaving);
        if(success)
            SaveConfirmation::deleteImage(getCurrentFileName());
    }
    else
        success = img.save(locationForSaving);

    return success;
}
