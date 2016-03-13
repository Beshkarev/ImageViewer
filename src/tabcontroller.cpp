#include "tabcontroller.h"
#include "screenimage.h"
#include "filesystem.h"
#include "saveconfirmation.h"
#include <QTabWidget>
#include <QDebug>
#include <QFileDialog>
#include <QApplication>

TabController::TabController(QWidget *parent/*=0*/) :
    QTabWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMovable(true);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

void TabController::createTab()
{
    ScreenImage *widget = new ScreenImage;
    addTab(widget, tr("Tab%1").arg(count()+1));
    setCurrentWidget(widget);
    addWorkDirectory("");

    emit tabCreated();
    //qDebug() << "Index just creadted tab" << currentIndex();
}

void TabController::loadFiletoTab(const QString &file)
{
    ScreenImage *wdg;
    wdg = getImageWidget();
    if(widgetIsNULL(wdg))
        return;

    QImage img;
    bool successfullyImageLoad;
    if(SaveConfirmation::imageIsExist(file))
    {
        img = SaveConfirmation::getChagedImage(file);
    }
    else
        img.load(file);

    successfullyImageLoad = wdg->loadImage(img, file);
    if(!successfullyImageLoad)
        deleteTab(currentIndex());
    else
    {
        updateTabText(currentIndex(), wdg->getFileName());
        addWorkDirectory(file);
    }
}

void TabController::saveFileOpenedInTab()
{
    ScreenImage *widg = getImageWidget();
    if(!widgetIsNULL(widg))
        widg->saveImage(widg->getFileName());
}

void TabController::saveAsFileOpenedInTab(const QString &file)
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->saveImage(file);
}

void TabController::closeImage()
{
    ScreenImage *wdg = getImageWidget();
    if(!widgetIsNULL(wdg))
    {
        wdg->closeImage();
        updateTabNumber();
    }
}

void TabController::next()
{
    QList<QFileInfo>::const_iterator it;
    it = _iteratots.find(currentWidget()).value();
    QFileInfoList list = _entries.find(workDirectory()).value();

    if(it != list.cend())
    {
        loadFiletoTab((*it).absoluteFilePath());
        ++it;
    }
    else
    {
        it = list.cbegin();
        loadFiletoTab((*it).absoluteFilePath());
        ++it;
    }

    _iteratots.insert(currentWidget(), it);
}

void TabController::open()
{
    QString dir = workDirectory();
    if(dir.isEmpty())
        dir = QDir::homePath() + "/Pictures";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    dir,
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg;;"
                                                       "*.ppm;;*.xbm;;*.xpm"));
    addWorkDirectory(filename);
    entryList(filename);
    createIterator();
}

void TabController::closeTab(const int index)
{  
    deleteTab(index);
    emit tabClosed();
}

void TabController::horizontalFlip()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->horizontalFlip();
}

void TabController::verticalFlip()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->verticalFlip();
}

void TabController::clockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->clockwiseRotate();
}

void TabController::counterClockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->counterClockwiseRotate();
}

void TabController::zoomInImage()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->zoomInImage();
}

void TabController::zoomOutImage()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->zoomOutImage();
}

void TabController::fitImage()
{
    ScreenImage *wdg = getImageWidget();
    if(!widgetIsNULL(wdg))
        wdg->fitImage();
}

ScreenImage *TabController::getImageWidget()
{
    return static_cast<ScreenImage*>(currentWidget());
}

void TabController::updateTabNumber()
{
    ScreenImage *wdg;
    for(int i = 0; i < count(); ++i)
    {
        wdg = static_cast<ScreenImage*>(widget(i));
        if(widgetIsNULL(wdg))
           return;
        if(wdg->isEmpty())
        {
            //qDebug() << "in updateTab";
            updateTabText(i, "Tab" + QString::number(i + 1));
        }
    }
}

bool TabController::widgetIsNULL(ScreenImage *wdg) const
{
    return wdg == nullptr ? true : false;
}

void TabController::updateTabText(const int index, const QString &text)
{
    setTabText(index, text);
}

void TabController::deleteTab(const qint32 index)
{
    QWidget *pWdg = widget(index);
    removeTab(index);
    pWdg->deleteLater();
    updateTabNumber();
}

void TabController::addWorkDirectory(const QString &dir)
{
    QHash<QWidget*, QString>::const_iterator it;
    it = directorys.find(currentWidget());
    //if unsaved directory
    if(it == directorys.cend())
    {
        qDebug() << "new dir";
        directorys.insert(currentWidget(),
                          FileSystem::absolutePath(dir));
    }
    //if saved directory was changed
    else if(workDirIsChanged(dir))
    {
        qDebug() << "work dir is changed";
        directorys.remove(currentWidget());
        directorys.insert(currentWidget(),
                          FileSystem::absolutePath(dir));
    }
    else
        qDebug() << "nothing changed";
}

bool TabController::workDirIsChanged(const QString &dir)
{
    QHash<QWidget*, QString>::const_iterator it;
    it = directorys.find(currentWidget());

    if(it.value() != FileSystem::absolutePath(dir))
        return true;
    else
        return false;
}

void TabController::entryList(const QString &dir)
{
    QDir directory(FileSystem::absolutePath(dir));

    if(!entryIsExist(dir))
    {
        QApplication::processEvents();

        QStringList supportedFormats;
        supportedFormats << "*.jpg" << "*.bmp" << "*.png";
        QFileInfoList list = directory.entryInfoList(supportedFormats, QDir::Files,
                                             QDir::LocaleAware);
        _entries.insert(directory.absolutePath(), list);
    }
}

bool TabController::entryIsExist(const QString &dir)
{
    QHash<QString, QFileInfoList>::const_iterator it;
    it = _entries.find(FileSystem::absolutePath(dir));

    if(it == _entries.cend())
        return false;
    else
        return true;
}

void TabController::createIterator()
{
    QString dir = workDirectory();

    QList<QFileInfo>::const_iterator it;
    it = _entries.find(dir).value().cbegin();

    _iteratots.insert(currentWidget(), it);
}

QString TabController::workDirectory() const
{
    QHash<QWidget*, QString>::const_iterator it;
    it = directorys.find(currentWidget());

    if(it != directorys.cend())
        return it.value();
    else
        return QString();
}
