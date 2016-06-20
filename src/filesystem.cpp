#include "filesystem.h"
#include "saveconfirmation.h"
#include "app_properties.h"

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
        _directories.remove(ptr.workDirectory());
        _entries.remove(widg);
    }
}

QString FileSystem::openFile()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open file"),
                                                    AppProperties::lastWorkDirectory(),
                                                    AppProperties::supportedFormats().join(";;") + ";;All (*.*)");
    if(filename.isEmpty())
        throw QString ("The open file dialog was just closed\n");

    checkSelectedFileIsSupported(filename);
    createEntry(filename);
    setWorkDirectory(filename);

    AppProperties::changeLastWorkDirectory(filename);

    return filename;
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
    return saveToDisk(getCurrentAbsoluteFileName());
}

bool FileSystem::saveAs()
{
    const QString filename = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save file"),
                                                    getCurrentAbsoluteFileName(),
                                                    AppProperties::supportedFormats().join(";;"));
    if (filename.isEmpty())
        throw QString ("The save file dialog was just closed\n");

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
    QFileInfo fi(selectedFile);
    QString str;
    str = "*." + fi.suffix();

    bool support = AppProperties::supportedFormats().contains(str,
                                                              Qt::CaseInsensitive);

    if (!support)
        throw std::runtime_error(QObject::tr("The file format is not supported.").toStdString());
}

QString FileSystem::getCurrentAbsoluteFileName()
{
    auto &en = _entries.find(_pTabs->currentWidget()).value();

    return en.absoluteFileName();
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
