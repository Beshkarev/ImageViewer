#include "tabcontroller.h"
#include "screenimage.h"
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>
#include <QHBoxLayout>

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
    qDebug() << "new Tab" << count();

    ScreenImage *widget = new ScreenImage;
    addTab(widget, tr("Tab%1").arg(count()+1));
    setCurrentWidget(widget);

    qDebug() << count();
}

void TabController::loadFiletoTab(const QString &file)
{
    qDebug() << objectName() << "open() slot";
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
    {
        removeTab(currentIndex());
        wdg->deleteLater();
    }
    else
        updateTabText(currentIndex(), wdg->getFileName());
}

void TabController::saveFileOpenedInTab()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->saveImage();
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
    qDebug() << "TabWidget" << "closeTab()";

    ScreenImage *widg = static_cast<ScreenImage*>(widget(index));
    if(widgetIsNULL(widg))
        return;

    if(widg->isChanged())
    {
        qint32 chose;
        chose = QMessageBox::warning(this, tr("Save changes"),
                                 tr("The image %1 has been modified.\n"
                                    "Do you want to save your changes?").arg(widg->getFileName()),
                                    QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);
        if(chose == QMessageBox::Yes)
        {
            saveFileOpenedInTab();
            removeTab(index);
            widg->deleteLater();
            updateTabNumber();
        }
        else if(chose == QMessageBox::No)
        {
            removeTab(index);
            widg->deleteLater();
            updateTabNumber();
        }
        else if(chose == QMessageBox::Cancel)
            return;
    }
    else
        removeTab(index);
        widg->deleteLater();
        updateTabNumber();
}

void TabController::horizontalFlip()
{
    ScreenImage *widget = getImageWidget();
    if(!widgetIsNULL(widget))
        widget->horizontalFlip();
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

void TabController::fitImage(bool checked)
{
    qDebug() << checked;
    ScreenImage *wdg = getImageWidget();
    if(!widgetIsNULL(wdg))
        wdg->fitImage(checked);
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
