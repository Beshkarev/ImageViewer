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

void TabController::loadFiletoTab()
{
    qDebug() << objectName() << "open() slot";
    ScreenImage *wdg;
    bool successfullyImageLoad;
    if(count() == 0)
    {
        createTab();
        wdg = getImageWidget();
        successfullyImageLoad = wdg->loadImage();
    }
    else if(count() != 0 && getImageWidget()->isEmpty())
    {
        wdg = getImageWidget();
        successfullyImageLoad = wdg->loadImage();
    }
    else
    {
        createTab();
        wdg = getImageWidget();
        successfullyImageLoad = wdg->loadImage();
    }

    if(!successfullyImageLoad)
    {
        removeTab(currentIndex());
        wdg->deleteLater();
    }
    else
        setTabText(currentIndex(), wdg->getFileName());

}

void TabController::saveFileOpenedInTab()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->saveImage();
}

void TabController::closeTab(const int index)
{
    qDebug() << "TabWidget" << "closeTab()";
    if(count() == 0)
        return;
    ScreenImage *widg = static_cast<ScreenImage*>(widget(index));
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
    if(widget != nullptr)
        widget->loadImage();
}

void TabController::clockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->clockwiseRotate();
}

void TabController::counterClockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->counterClockwiseRotate();
}

void TabController::zoomInImage()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->zoomInImage();
}

void TabController::zoomOutImage()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->zoomOutImage();
}

void TabController::fitImage(bool checked)
{
    qDebug() << checked;
    ScreenImage *wdg = getImageWidget();
    if(wdg != nullptr)
        wdg->fitImage(checked);
}

ScreenImage *TabController::getImageWidget()
{
    return dynamic_cast<ScreenImage*>(currentWidget());
}

void TabController::updateTabNumber()
{
    for(int i = 0; i < count(); ++i)
        setTabText(i, "Tab" + QString::number(i + 1));
}
