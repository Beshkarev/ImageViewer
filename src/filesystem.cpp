#include "filesystem.h"
#include "saveconfirmation.h"
#include "config.h"
#include "tabcontroller.h"
#include "entry.h"

#include <QFileDialog>
#include <QImage>
#include <QObject>
#include <QFile>

FileSystem *FileSystem::_pInstance;

FileSystem::FileSystem(QObject *parent) : QObject(parent),
    _pTabs(TabController::instance())
{
    connect(_pTabs.get(), SIGNAL(tabClosed(QWidget*)),
            this, SLOT(destroyEntry(QWidget*)));
}

FileSystem::~FileSystem() = default;

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

QString FileSystem::fileSuffix(const QString &file)
{
    return QFileInfo(file).suffix();
}

bool FileSystem::fileMayBeSave(const QString &file)
{
    return Config::supportedSaveFormats().contains("*." + fileSuffix(file),
                                                   Qt::CaseInsensitive);
}

bool FileSystem::isGIF(const QString &file)
{
    return QString("gif").contains(FileSystem::fileSuffix(file),
                                   Qt::CaseInsensitive);
}

QString FileSystem::openFileDialog()
{
    QFileDialog dialog;
    dialog.setMimeTypeFilters(Config::supportedReadMimeTypes());
    dialog.setDirectory(Config::lastWorkDirectory);

    if(!dialog.exec())
        throw QString ("The open file dialog was just closed\n");

    QStringList filename = dialog.selectedFiles();

    prepareBeforeLoadFile(filename.first());

    return filename.first();
}

QString FileSystem::nextFile()
{
    auto &en = _entries.find(_pTabs->currentWidget()).value();

    return en.next();
}

QString FileSystem::previousFile()
{
    auto &en = _entries.find(_pTabs->currentWidget()).value();

    return en.previous();
}

bool FileSystem::saveFile()
{
    QString file = getCurrentAbsoluteFileName();
    return moveFile(file,
                    Config::tempLocation + fileName(file));
}

bool FileSystem::saveAsDialog()
{
    QFileDialog dialog;
    dialog.setMimeTypeFilters(Config::supportedSaveMimeTypes());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile(getCurrentAbsoluteFileName());
    dialog.setDirectory(getCurrentAbsoluteFileName());
    dialog.selectMimeTypeFilter("image/jpeg");

    bool success;
    if(dialog.exec())
    {
        QImage img;
        if(SaveConfirmation::imageWasChanged(getCurrentAbsoluteFileName()))
        {
            img = SaveConfirmation::getChagedImage(getCurrentAbsoluteFileName());
            success = img.save(dialog.selectedFiles().first());
            if(success)
                SaveConfirmation::deleteImage(getCurrentAbsoluteFileName());
        }
        else
        {
            img.load(getCurrentAbsoluteFileName());
            success = img.save(dialog.selectedFiles().first());
        }
    }

    return success;
}

void FileSystem::openFile(const QString &file)
{
    prepareBeforeLoadFile(file);
}

void FileSystem::destroyEntry(QWidget *widg)
{
    auto it = _entries.find(widg);
    if (it != _entries.end())
    {
        auto &ptr = it.value();
        _directories.remove(ptr.workDirectory());
        _entries.remove(widg);
    }
}

void FileSystem::prepareBeforeLoadFile(const QString &filename)
{
    checkSelectedFileIsSupported(filename);
    createEntry(filename);
    setWorkDirectory(filename);

    Config::changeLastWorkDirectory(filename);
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
        _entries.insert(_pTabs->currentWidget(),
                        Entry(dir));
    }
    else
    {
        auto *wdg = _directories.find(FileSystem::absoluteFilePath(dir)).value();
        auto ptr = _entries.find(wdg).value();//<- excess copying
        ptr.setIterToSelectedFile(dir);

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

void FileSystem::checkSelectedFileIsSupported(const QString &selectedFile) const
{
    QString str;
    str = "*." + fileSuffix(selectedFile);

    bool support = Config::supportedReadFormats().contains(str, Qt::CaseInsensitive);

    if (!support)
        throw std::runtime_error(QObject::tr("The file format is not supported.").toStdString());
}

QString FileSystem::getCurrentAbsoluteFileName()
{
    auto &en = _entries.find(_pTabs->currentWidget()).value();

    return en.absoluteFileName();
}

bool FileSystem::moveFile(const QString &locationTo,
                          const QString &locationFrom)
{
//    QImage img(getCurrentAbsoluteFileName());
//    bool success;
//    if(SaveConfirmation::imageWasChanged(getCurrentAbsoluteFileName()))
//    {
//        img = SaveConfirmation::getChagedImage(getCurrentAbsoluteFileName());
//        success = img.save(locationForSaving);
//        if(success)
//            SaveConfirmation::deleteImage(getCurrentAbsoluteFileName());
//    }
//    else
//        success = img.save(locationForSaving);

    QFile file;
    if(file.exists(locationTo))
        file.remove(locationTo);

    bool success = file.copy(locationFrom, locationTo);
    if(success)
        SaveConfirmation::deleteImage(locationTo);

    return success;
}

QString FileSystem::currentAbsoluteFileName()
{
    auto &&entry = _entries.find(_pTabs->currentWidget());

    return entry != _entries.end() ? entry->absoluteFileName() : QString{};
}
