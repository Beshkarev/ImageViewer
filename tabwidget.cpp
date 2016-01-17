#include "tabwidget.h"
#include <QTabWidget>
#include "screenimage.h"
#include <QMessageBox>
#include <QDebug>
#include <QHBoxLayout>

TabWidget::TabWidget(QWidget *parent/*=0*/) :
    QTabWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMovable(true);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

void TabWidget::createTab()
{
    qDebug() << "new Tab" << count();

    ScreenImage *widget = new ScreenImage;
    addTab(widget, tr("Tab%1").arg(count()+1));
    setCurrentWidget(widget);

    qDebug() << count();
}

void TabWidget::loadFiletoTab()
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
    {
        setTabText(currentIndex(), wdg->getFileName());
    }
}

void TabWidget::saveFileOpenedInTab()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->saveImage();
}

void TabWidget::closeTab(const int index)
{
    qDebug() << "TabWidget" << "closeTab()";
    ScreenImage *widg = static_cast<ScreenImage*>(widget(index));
    if(widg->isChanged())
    {
        qint32 chose;
        chose = QMessageBox::warning(this, tr("Save changes"),
                                 tr("The image has been modified.\n"
                                    "Do you want to save your changes?"),
                                    QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);
        if(chose == QMessageBox::Yes)
        {
            saveFileOpenedInTab();
            removeTab(index);
            widg->deleteLater();
        }
        else if(chose == QMessageBox::No)
        {
            removeTab(index);
            widg->deleteLater();
        }
        else if(chose == QMessageBox::Cancel)
            return;
    }
    else
        removeTab(index);
        widg->deleteLater();
}

void TabWidget::horizontalFlip()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->loadImage();
}

void TabWidget::clockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->clockwiseRotate();
}

void TabWidget::counterClockwiseRotate()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->counterClockwiseRotate();
}

void TabWidget::zoomInImage()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->zoomInImage();
}

void TabWidget::zoomOutImage()
{
    ScreenImage *widget = getImageWidget();
    if(widget != nullptr)
        widget->zoomOutImage();
}

void TabWidget::fitImage(bool checked)
{
    qDebug() << checked;
    ScreenImage *wdg = getImageWidget();
    if(wdg != nullptr)
        wdg->fitImage(checked);
}

ScreenImage *TabWidget::getImageWidget()
{
    return dynamic_cast<ScreenImage*>(currentWidget());
}
