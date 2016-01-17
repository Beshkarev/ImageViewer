#include "mainwindow.h"
#include "tabcontroller.h"
#include <QStringList>
#include <QDir>
#include <QBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabController(new TabController)
{
    createActions();
    createMenu();
    createConnectToSlots();
    qDebug() << "constructor";

    QCoreApplication::setApplicationVersion("0.4");
    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabController);
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
    _pFileMenu = menuBar()->addMenu("&File");
    _pFileMenu->addAction(_pNewTabAction);
    _pFileMenu->addAction(_pOpenAction);
    _pFileMenu->addAction(_pSaveAction);
    _pFileMenu->addAction(_pCloseImageAction);
    pSeparatorAction = _pFileMenu->addSeparator();
    for(int i = 0; i < maxRecentFile; ++i)
        _pFileMenu->addAction(pRecentAction[i]);
    _pFileMenu->addSeparator();
    _pFileMenu->addAction(_pExitAction);

    _pEditMenu = menuBar()->addMenu("&Edit");
    _pEditMenu->addAction(_pHorizontalFlipAction);
    _pEditMenu->addAction(_pClockwiseRotateAction);
    _pEditMenu->addAction(_pCounterClockwiseRotateAction);
    _pEditMenu->addAction(_pZoomIn);
    _pEditMenu->addAction(_pZoomOut);

    _pViewMenu = menuBar()->addMenu("&View");
    _pViewMenu->addAction(_pFitAction);

    _pHelpMenu = menuBar()->addMenu("&Help");
    _pHelpMenu->addAction(_pAboutAction);
    _pHelpMenu->addAction(_pQtAbout);
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
    connect(_pCloseImageAction, SIGNAL(triggered(bool)),
            this, SLOT(closeTabRequest()));
    connect(_pExitAction, SIGNAL(triggered(bool)),
            this, SLOT(close()));

    //Edit menu section
    connect(_pHorizontalFlipAction, SIGNAL(triggered(bool)),
            this, SLOT(horizontalFlip()));
    connect(_pCounterClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(counterClockwiseRotate()));
    connect(_pClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(clockwiseRotate()));
    connect(_pZoomIn, SIGNAL(triggered(bool)),
            this, SLOT(zoomInImage()));
    connect(_pZoomOut, SIGNAL(triggered(bool)),
            this, SLOT(zoomOutImage()));

    //View menu section
    connect(_pFitAction, SIGNAL(triggered(bool)),
            this, SLOT(fitImageRequest()));

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
    _pTabController->createTab();
}

void MainWindow::open()
{
    _pTabController->loadFiletoTab();
}

void MainWindow::save()
{
    _pTabController->saveFileOpenedInTab();
}

void MainWindow::closeTabRequest()
{
    _pTabController->closeTab(_pTabController->currentIndex());
}

void MainWindow::horizontalFlip()
{
    _pTabController->horizontalFlip();
}

void MainWindow::clockwiseRotate()
{
    _pTabController->clockwiseRotate();
}

void MainWindow::counterClockwiseRotate()
{
    _pTabController->counterClockwiseRotate();
}

void MainWindow::zoomInImage()
{
    _pTabController->zoomInImage();
}

void MainWindow::zoomOutImage()
{
    _pTabController->zoomOutImage();
}

void MainWindow::fitImageRequest()
{
    bool checked = _pFitAction->isChecked();
    _pTabController->fitImage(checked);
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
    //for(int i = 0; i < _pTabWidget->count(); ++i)
        //closeTabRequest();
        //_pTabWidget->closeTab(i);
    //pClose->accept();
    //while(_pTabWidget->currentIndex() != -1)
   // {
     //   _pTabWidget->closeTab();
        //        if(getImageWidget()->isChanged())
//        {
//            qint32 chose;
//            chose = QMessageBox::warning(this, tr("Save changes"),
//                                     tr("The image has been modified.\n"
//                                        "Do you want to save your changes?"),
//                                        QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);

//            if(chose == QMessageBox::Yes)
//            {
//                emit _pSaveAction->trigger();
//                closeTab(_pTabWidget->currentIndex());
//            }
//            else if(chose == QMessageBox::No)
//                closeTab(_pTabWidget->currentIndex());
//            else if(chose == QMessageBox::Cancel)
//                pClose->ignore();
//        }
//        else
//            closeTab(_pTabWidget->currentIndex());
    //}
   // pClose->accept();

}
