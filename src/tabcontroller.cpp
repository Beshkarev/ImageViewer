#include "tabcontroller.h"
#include "screenimage.h"
#include <QTabWidget>
#include <QDebug>

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
}

void TabController::loadFiletoTab(const QString &file)
{
    ScreenImage *wdg;
    bool successfullyImageLoad;

    if(count() == 0)
    {
        createTab();
        wdg = getImageWidget();
    }
    else
        wdg = getImageWidget();

    if(widgetIsNULL(wdg))
        return;

    successfullyImageLoad = wdg->loadImage(file);
    if(!successfullyImageLoad)
        deleteTab(currentIndex());
    else
        updateTabText(currentIndex(), wdg->getFileName());
}

void TabController::nextFile()
{
    /*if(it != filesList.cend())
    {
        qDebug() << "nextFile()" << "hasNext section";
        fileForLoad(*it);
        ++it;
    }
    else
    {
        qDebug() << "nextFile()" << "else section";

        it = filesList.cbegin();
        fileForLoad(*it);
        ++it;
    }*/
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

void TabController::closeTab(const int index)
{  
    //closeImage();
    deleteTab(index);
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
            qDebug() << "in updateTab";
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
