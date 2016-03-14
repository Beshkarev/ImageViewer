#include "tabcontroller.h"
#include "screenimage.h"
#include "saveconfirmation.h"
#include "filesystem.h"
#include <QTabWidget>
#include <QDebug>

TabController *TabController::_instance = nullptr;

TabController::TabController(QWidget *parent/*=0*/) :
    QTabWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMovable(true);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

TabController *TabController::instance()
{
    if(_instance == nullptr)
        _instance = new TabController;
    return _instance;
}

void TabController::createTab()
{
    ScreenImage *widget = new ScreenImage;
    addTab(widget, tr("Tab%1").arg(count()+1));
    setCurrentWidget(widget);

    emit tabCreated();
}

void TabController::loadFiletoTab(const QString &file)
{
    ScreenImage *wdg;
    wdg = getImageWidget();
    if(widgetIsNULL(wdg))
        return;

    QImage img;
    bool successfullyImageLoad;
    if(SaveConfirmation::imageWasChanged(file))
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
        updateTabText(currentIndex(),
                      FileSystem::fileName(file));
        emit tabStateChanged();
    }
}

void TabController::closeImage()
{
    ScreenImage *wdg = getImageWidget();
    if(!widgetIsNULL(wdg))
    {
        wdg->closeImage();
        updateTabNumber();
        emit tabStateChanged();
    }
}

bool TabController::currentTabIsEmpty()
{
    return getImageWidget()->isEmpty() ? true : false;
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

    emit tabClosed();
}
