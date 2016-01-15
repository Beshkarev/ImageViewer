#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QDir>
#include "screenimage.h"
#include <QBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabWidget(new QTabWidget(this))
{

    createActions();
    createMenu();
    createConnectToSlots();
    qDebug() << "constructor";
    _pTabWidget->setAttribute(Qt::WA_DeleteOnClose);
    _pTabWidget->setMovable(true);
    _pTabWidget->setTabsClosable(true);

    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabWidget);
}

void MainWindow::createActions()
{
    _pNewTabAction = new QAction(tr("New Tab"), this);
    _pNewTabAction->setShortcut(Qt::CTRL + Qt::Key_T);
    _pNewTabAction->setStatusTip("Creating new tab");

    _pOpenAction = new QAction(tr("Open file"), this);
    _pOpenAction->setShortcut(QKeySequence::Open);
    _pOpenAction->setStatusTip("Opend new file");

    _pSaveAction = new QAction(tr("Save file"), this);
    _pSaveAction->setShortcut(QKeySequence::Save);
    _pSaveAction->setStatusTip(tr("Save the file"));


    _pCloseImageAction = new QAction(tr("Close image"), this);
    _pCloseImageAction->setShortcut(QKeySequence::Close);


    for(int i = 0; i < maxRecentFile; ++i)
    {
        pRecentAction[i] = new QAction(this);
        pRecentAction[i]->setVisible(false);
        //connect(pRecentAction[i], SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
    }
    //RecentFileUpdate();

    _pExitAction = new QAction(tr("Exit"), this);
    _pExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    _pHorizontalFlipAction = new QAction("Horizontal flip", this);
    _pClockwiseRotateAction = new QAction("Rotate Clockwise", this);
    _pCounterClockwiseRotateAction = new QAction("Rotate Counter clockwise", this);
    _pFitAction = new QAction("Fit to window", this);
    _pFitAction->setCheckable(true);
    _pZoomIn = new QAction("Zoom in", this);
    _pZoomIn->setShortcut(Qt::CTRL + Qt::Key_Plus);
    _pZoomOut = new QAction("Zoom out", this);
    _pZoomOut->setShortcut(Qt::CTRL + Qt::Key_Minus);

    _pAboutAction = new QAction(tr("About"), this);
    _pQtAbout = new QAction(tr("Qt About"), this);
}

void MainWindow::createMenu()
{
    pFileMenu = menuBar()->addMenu("&File");
    pFileMenu->addAction(_pNewTabAction);
    pFileMenu->addAction(_pOpenAction);
    pFileMenu->addAction(_pSaveAction);
    pFileMenu->addAction(_pCloseImageAction);
    pSeparatorAction = pFileMenu->addSeparator();
    for(int i = 0; i < maxRecentFile; ++i)
        pFileMenu->addAction(pRecentAction[i]);
    pFileMenu->addSeparator();
    pFileMenu->addAction(_pExitAction);

    pEditMenu = menuBar()->addMenu("&Edit");
    pEditMenu->addAction(_pHorizontalFlipAction);
    pEditMenu->addAction(_pClockwiseRotateAction);
    pEditMenu->addAction(_pCounterClockwiseRotateAction);
    pEditMenu->addAction(_pFitAction);
    pEditMenu->addAction(_pZoomIn);
    pEditMenu->addAction(_pZoomOut);

    pHelpMenu = menuBar()->addMenu("&Help");
    pHelpMenu->addAction(_pAboutAction);
    pHelpMenu->addAction(_pQtAbout);
}

void MainWindow::createConnectToSlots()
{
    qDebug("connect");
    //File menu section
    connect(_pNewTabAction, SIGNAL(triggered(bool)),
            this, SLOT(newTab()));
    connect(_pOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(open()));
    connect(_pSaveAction, SIGNAL(triggered(bool)),
            this, SLOT(save()));
    connect(_pTabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
    //connect(_pCloseImageAction, SIGNAL(triggered(bool)),
      //      this, SLOT(closeTab(int)));
    connect(_pExitAction, SIGNAL(triggered(bool)),
            this, SLOT(close()));

    //Edit menu section
    connect(_pHorizontalFlipAction, SIGNAL(triggered(bool)),
            this, SLOT(horizontalFlip()));
    connect(_pCounterClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(counterClockwiseRotate()));
    connect(_pClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(clockwiseRotate()));
    //connect(pFitAction, SIGNAL(triggered(bool)), this, SLOT(fitToWindow()));
    connect(_pZoomIn, SIGNAL(triggered(bool)),
            this, SLOT(zoomInImage()));
    connect(_pZoomOut, SIGNAL(triggered(bool)),
            this, SLOT(zoomOutImage()));

    //About menu section
    connect(_pAboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutApp()));
    connect(_pQtAbout, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
}

void MainWindow::setRecentFile(const QString &filename)
{
    if(!filename.isEmpty())
    {
        recentFile.removeAll(filename);
        recentFile.prepend(filename);
    }
}

void MainWindow::updateListRecentFiles()
{
    QMutableStringListIterator iter = recentFile;
    while(iter.hasNext())
    {
        if(!QFile::exists(iter.next()))
            iter.remove();
    }
    for(int i = 0; i < maxRecentFile; ++i)
    {
        if(i < recentFile.count())
        {
            pRecentAction[i]->setText(tr("&%1 %2").arg(i + 1)
                                      .arg(QFileInfo(recentFile[i]).fileName()));
            pRecentAction[i]->setData(recentFile[i]);
            pRecentAction[i]->setVisible(true);
        }
        else pRecentAction[i]->setVisible(false);
    }
    pSeparatorAction->setVisible(!recentFile.isEmpty());
}

void MainWindow::newTab()
{
//    QMessageBox::information(this, "Sorry",
//                             "Not implemented yet",
//                             QMessageBox::Ok);
//
    qDebug() << "new Tab";
    ScreenImage *widget = new ScreenImage;
    _pTabWidget->addTab(widget, "tab");
    _pTabWidget->setCurrentWidget(widget);
}

void MainWindow::open()
{
    qDebug() << objectName() << "open() slot";
    //if(getImageWidget()== nullptr && _pTabWidget->count() == 0)
        newTab();
    getImageWidget()->loadImage();
    if(getImageWidget()->isEmpty())
        closeTab(_pTabWidget->currentIndex());
}

void MainWindow::save()
{
    getImageWidget()->saveImage();
}

void MainWindow::closeTab(const int index)
{
    qDebug() << "MainWindow" << "closeTab() slot";
   QWidget *deletedTab = _pTabWidget->widget(index);

    _pTabWidget->removeTab(index);
   // _pTabWidget->tabRemoved(index);
    deletedTab->deleteLater();
}

void MainWindow::horizontalFlip()
{
    getImageWidget()->horizontalFlip();
}

void MainWindow::clockwiseRotate()
{
    getImageWidget()->clockwiseRotate();
}

void MainWindow::counterClockwiseRotate()
{
    getImageWidget()->counterClockwiseRotate();
}

void MainWindow::zoomInImage()
{
    getImageWidget()->zoomInImage();
}

void MainWindow::zoomOutImage()
{
    getImageWidget()->zoomOutImage();
}

void MainWindow::aboutApp()
{
    QMessageBox::about(this, tr("About the application"),
                       tr("A small application for view image.\n"
                          "The programm developed for education and personal satisfaction.\n"));
}

void MainWindow::closeEvent(QCloseEvent *pClose)
{
    //_pTabWidget->count()
    while(_pTabWidget->currentIndex() != -1)
    {
        if(getImageWidget()->isChanged())
        {
            qint32 chose;
            chose = QMessageBox::warning(this, tr("Save changes"),
                                     tr("The image has been modified.\n"
                                        "Do you want to save your changes?"),
                                        QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);

            if(chose == QMessageBox::Yes)
            {
                emit _pSaveAction->trigger();
                closeTab(_pTabWidget->currentIndex());
            }
            else if(chose == QMessageBox::No)
                closeTab(_pTabWidget->currentIndex());
            else if(chose == QMessageBox::Cancel)
                pClose->ignore();
        }
        else
            closeTab(_pTabWidget->currentIndex());
    }
    pClose->accept();

}

ScreenImage *MainWindow::getImageWidget()
{
    return static_cast<ScreenImage*>(_pTabWidget->currentWidget());
}
