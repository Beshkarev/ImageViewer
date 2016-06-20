#include "tabcontroller.h"
#include "screenimage.h"
#include "saveconfirmation.h"
#include "filesystem.h"
#include <QTabWidget>
#include <QDebug>
#include <thread>
#include <functional>

TabController *TabController::_instance;

TabController::TabController(QWidget *parent/*=0*/) :
    QTabWidget(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    setMovable(true);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

/*
TabController::~TabController()
{
    if(_instance)
    {
    delete _instance;
        _instance = nullptr;
    }
}
void TabController::destroyInstance()
{
    qDebug() << "destroy TabController";
    if(_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}
*/
TabController *TabController::instance()
{
    if(!_instance)
    {
        _instance = new TabController;
        //atexit(& TabController::destroyInstance);
    }

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

    std::thread thr(&ScreenImage::loadImage, wdg,
                    std::move(file));
    thr.detach();
    emit tabStateChanged();
}

void TabController::closeImage()
{
    ScreenImage *wdg = getImageWidget();

    wdg->closeImage();
    updateTabNumber();
    emit tabStateChanged();
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
    widget->horizontalFlip();
}

void TabController::verticalFlip()
{
    ScreenImage *widget = getImageWidget();
    widget->verticalFlip();
}

void TabController::clockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    widget->clockwiseRotate();
}

void TabController::counterClockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    widget->counterClockwiseRotate();
}

void TabController::zoomInImage()
{
    ScreenImage *widget = getImageWidget();
    widget->zoomInImage();
}

void TabController::zoomOutImage()
{
    ScreenImage *widget = getImageWidget();
    widget->zoomOutImage();
}

void TabController::fitImage()
{
    ScreenImage *wdg = getImageWidget();
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
