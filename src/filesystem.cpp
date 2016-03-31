#include "filesystem.h"
#include "tabcontroller.h"
#include "saveconfirmation.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QFileDialog>
#include <QObject>
#include <QImage>

FileSystem::FileSystem(): _pTabs(TabController::instance())
{}

QString FileSystem::absolutePath(const QString &dir)
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

    addWorkDirectory(filename);
    entryList(filename);
    createIterator(filename);

    if(workDirIsChanged(lastDir))
        lastDir = workDirectory();

    return filename;
}

QString FileSystem::nextFile()
{
    QApplication::processEvents();
    QList<QFileInfo>::const_iterator it;
    it = _iteratots.find(_pTabs->currentWidget()).value();

    QFileInfoList list = _entries.find(workDirectory()).value();

    QString nameFile;
    ++it;
    if(it == list.cend())
    {
        it = list.cbegin();
        nameFile = (*(it)).absoluteFilePath();
    }
    else
    {
        nameFile = (*(it)).absoluteFilePath();
    }

    _iteratots.insert(_pTabs->currentWidget(), it);
    return nameFile;
}

QString FileSystem::previousFile()
{
    QApplication::processEvents();
    QList<QFileInfo>::const_iterator it;
    it = _iteratots.find(_pTabs->currentWidget()).value();
    QFileInfoList list = _entries.find(workDirectory()).value();

    QString nameFile;
    if(it == list.cbegin())
    {
       it = list.cend();
       nameFile = (*(--it)).absoluteFilePath();
    }
    else
    {
        nameFile = (*(--it)).absoluteFilePath();
    }

    _iteratots.insert(_pTabs->currentWidget(), it);
    return nameFile;
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

void FileSystem::addWorkDirectory(const QString &dir)
{
    directorys.insert(_pTabs->currentWidget(),
                      FileSystem::absolutePath(dir));
}

bool FileSystem::workDirIsChanged(const QString &dir)
{
    QHash<QWidget*, QString>::const_iterator it;
    it = directorys.find(_pTabs->currentWidget());

    if(it.value() != FileSystem::absolutePath(dir))
        return true;
    else
        return false;
}

void FileSystem::entryList(const QString &dir)
{
    QDir directory(FileSystem::absolutePath(dir));

    if(!entryIsExist(dir))
    {
        qDebug() <<"entry don't exist";
        QApplication::processEvents();

        QStringList supportedFormats;
        supportedFormats << "*.jpg" << "*.bmp" << "*.png" << "*jpeg";
        QFileInfoList list = directory.entryInfoList(supportedFormats, QDir::Files,
                                             QDir::LocaleAware);
        _entries.insert(directory.absolutePath(), list);
    }
}

bool FileSystem::entryIsExist(const QString &dir)
{
    QHash<QString, QFileInfoList>::const_iterator it;
    it = _entries.find(FileSystem::absolutePath(dir));

    if(it == _entries.cend())
        return false;
    else
        return true;
}

void FileSystem::createIterator(const QString &file)
{
    QString dir = workDirectory();

    QList<QFileInfo>::const_iterator it;
    QFileInfoList list = _entries.find(dir).value();

    it = std::find(list.cbegin(), list.cend(), file);

    _iteratots.insert(_pTabs->currentWidget(), it);
}

QString FileSystem::workDirectory() const
{
    QHash<QWidget*, QString>::const_iterator it;
    it = directorys.find(_pTabs->currentWidget());

    if(it != directorys.cend())
        return it.value();
    else
        return QString();
}

QString FileSystem::getCurrentFileName()
{
    QList<QFileInfo>::const_iterator it;
    it = _iteratots.find(_pTabs->currentWidget()).value();

    return (*it).absoluteFilePath();
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
    {
        success = img.save(locationForSaving);
    }

    return success;
}
