#include "filesystem.h"
#include "saveconfirmation.h"
#include "entry.h"
#include "app_properties.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QImage>

FileSystem *FileSystem::_pInstance;

FileSystem::FileSystem(): _pTabs(TabController::instance())
{}

FileSystem *FileSystem::instance()
{
    if(!_pInstance)
        _pInstance = new FileSystem;

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

void FileSystem::destroyEntry(QWidget *widg)
{
    auto it = _entries.find(widg);
    if (it != _entries.end())
    {
        auto &ptr = it.value();
//        qDebug() << "delete before" << ptr.use_count();
        _directories.remove(ptr->workDirectory());
        _entries.remove(widg);
//        qDebug() << "delete after" << ptr.use_count();
    }
}

QString FileSystem::openFile()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open file"),
                                                    AppProrepties::lastWorkDirectory(),
                                                    "All (*.*);;" +
                                                    AppProrepties::supportedFormats().join(";;"));
    if(filename.isEmpty())
        return QString();

    createEntry(filename);
    setWorkDirectory(filename);

    AppProrepties::changeLastWorkDirectory(filename);

    return filename;
}

QString FileSystem::nextFile() const
{
    Entry *en = _entries.find(_pTabs->currentWidget()).value().get();

    return en->next();
}

QString FileSystem::previousFile() const
{
      Entry *en = _entries.find(_pTabs->currentWidget()).value().get();

      return en->previous();
}

bool FileSystem::saveFile()
{
    return saveToDisk(getCurrentAbsoluteFileName());
}

bool FileSystem::saveAs()
{
    const QString filename = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save file"),
                                                    getCurrentAbsoluteFileName(),
                                                    "All (*.*);;" +
                                                    AppProrepties::supportedFormats().join(";;"));
    return saveToDisk(filename);
}

void FileSystem::setWorkDirectory(const QString &directory)
{
    _directories.insert(FileSystem::absoluteFilePath(directory),
                       _pTabs->currentWidget());
}

void FileSystem::createEntry(const QString &dir)
{
    if(!entryIsExist(dir))
    {
        qDebug() <<"entry don't exist";
        _entries.insert(_pTabs->currentWidget(),
                        std::make_shared<Entry>(dir));
    }
    else
    {
        QWidget *wdg = _directories.find(FileSystem::absoluteFilePath(dir)).value();
        auto &ptr = _entries.find(wdg).value();

        _entries.insert(_pTabs->currentWidget(), ptr);
    }
}

bool FileSystem::entryIsExist(const QString &dir) const
{
    auto it = _directories.find(FileSystem::absoluteFilePath(dir));
    if (it == _directories.end())
        return false;
    else
        return true;
}

QString FileSystem::getCurrentAbsoluteFileName() const
{
    Entry *en = _entries.find(_pTabs->currentWidget()).value().get();

    return en->absoluteFileName();
}

bool FileSystem::saveToDisk(const QString &locationForSaving)
{
    QImage img(getCurrentAbsoluteFileName());
    bool success;
    if(SaveConfirmation::imageWasChanged(getCurrentAbsoluteFileName()))
    {
        img = SaveConfirmation::getChagedImage(getCurrentAbsoluteFileName());
        success = img.save(locationForSaving);
        if(success)
            SaveConfirmation::deleteImage(getCurrentAbsoluteFileName());
    }
    else
        success = img.save(locationForSaving);

    return success;
}
