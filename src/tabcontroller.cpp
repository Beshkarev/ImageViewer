#include "tabcontroller.h"
#include "screenimage.h"
#include "saveconfirmation.h"
#include "filesystem.h"
#include <QTabWidget>
#include <QDebug>
#include <thread>

TabController *TabController::_instance;

TabController::TabController(QWidget *parent/*=0*/) :
    QTabWidget(parent)
{
    setMovable(true);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

TabController *TabController::instance()
{
    if(!_instance)
        _instance = new TabController;

    return _instance;
}

void TabController::createTab()
{
    ScreenImage *widget = new ScreenImage;
    addTab(widget, tr("Tab%1").arg(count()+1));
    setCurrentWidget(widget);

    connect(widget, SIGNAL(imageLoaded()),
            this, SIGNAL(tabStateChanged()));

    emit tabCreated();
}

void TabController::loadFiletoTab(const QString &file)
{
    ScreenImage *wdg = getImageWidget();

    updateTabText(currentIndex(),
                  FileSystem::fileName(file));
    bool fileISGIF = FileSystem::isGIF(file);
    if(fileISGIF)
        wdg->loadGIF(file);
    else
    {
        std::thread thr(&ScreenImage::loadImage, wdg,
                        std::move(file));

        thr.detach();
    }
}

void TabController::closeImage()
{
    ScreenImage *wdg = getImageWidget();

    wdg->closeImage();
    updateTabNumber();
    emit tabStateChanged();
}

bool TabController::currentTabIsEmpty() const
{
    return getImageWidget()->isEmpty();
}

QString TabController::fileNameInTab() const
{
    return tabText(currentIndex());
}

bool TabController::fileInTabIsChanged() const
{
    return getImageWidget()->isChanged();
}

void TabController::closeTab(const int index)
{  
    deleteTab(index);
    emit tabClosed();
}

void TabController::horizontalFlip()
{
    ScreenImage *widget = getImageWidget();
    std::thread thread(&ScreenImage::horizontalFlip, widget);
    thread.detach();
}

void TabController::verticalFlip()
{
    ScreenImage *widget = getImageWidget();
    std::thread thread(&ScreenImage::verticalFlip, widget);
    thread.detach();
}

void TabController::clockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    std::thread trd(&ScreenImage::clockwiseRotate, widget);
    trd.detach();
}

void TabController::counterClockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    std::thread trd(&ScreenImage::counterClockwiseRotate, widget);
    trd.detach();
}

void TabController::zoomInImage()
{
    ScreenImage *widget = getImageWidget();
    std::thread trd(&ScreenImage::zoomInImage, widget);
    trd.detach();
}

void TabController::zoomOutImage()
{
    ScreenImage *widget = getImageWidget();
    std::thread trd(&ScreenImage::zoomOutImage, widget);
    trd.detach();
}

void TabController::fitImage()
{
    ScreenImage *wdg = getImageWidget();

    std::thread trd(&ScreenImage::fitImage, wdg);
    trd.detach();
}

ScreenImage *TabController::getImageWidget() const
{
    return static_cast<ScreenImage*>(currentWidget());
}

void TabController::updateTabNumber()
{
    ScreenImage *wdg;
    for(int i = 0; i < count(); ++i)
    {
        wdg = static_cast<ScreenImage*>(widget(i));
        if(wdg->isEmpty())
        {
            updateTabText(i, tr("Tab%1").arg(QString::number(i + 1)));
        }
    }
}

void TabController::updateTabText(const int index, const QString &text)
{
    setTabText(index, text);
}

void TabController::deleteTab(const qint32 index)
{
    QWidget *pWdg = widget(index);
    emit tabClosed(pWdg);
    removeTab(index);
    disconnect(pWdg, SIGNAL(imageLoaded()), this, SIGNAL(tabStateChanged()));
    delete pWdg;
    updateTabNumber();
}
