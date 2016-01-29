#include "mainwindow.h"
#include "tabcontroller.h"
#include "changedimages.h"
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include <QBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QCoreApplication>
#include <QToolBar>
#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabController(new TabController),
    currentPath(QDir::homePath() + "/Pictures")
{
    createActions();
    createMenu();
    addToolBar(createToolBar());
    createConnectToSlots();

    entryList();

    QCoreApplication::setApplicationVersion("0.6");
    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabController);
    updateListRecentFiles();
}

void MainWindow::createActions()
{
    _pNewTabAction = new QAction(tr("New Tab"), this);
    _pNewTabAction->setShortcut(Qt::CTRL + Qt::Key_T);
    _pNewTabAction->setStatusTip(tr("Creating new tab"));

    _pOpenAction = new QAction(tr("Open file"), this);
    _pOpenAction->setShortcut(QKeySequence::Open);
    _pOpenAction->setStatusTip(tr("Open new file"));

    _pSaveAction = new QAction(tr("Save file"), this);
    _pSaveAction->setShortcut(QKeySequence::Save);
    _pSaveAction->setStatusTip(tr("Save the file"));

    _pSaveAsAction = new QAction(tr("Save file as"), this);

    _pNextFileAction = new QAction(tr("Next file"), this);
    _pNextFileAction->setShortcut(Qt::Key_Right);

    _pPreviousFileAction = new QAction(tr("Previous file"), this);
    _pPreviousFileAction->setShortcut(Qt::Key_Left);

    _pCloseFileAction = new QAction(tr("Close image"), this);
    _pCloseFileAction->setStatusTip(tr("Closing image"));

    _pCloseTabAction = new QAction(tr("Close tab"), this);
    _pCloseTabAction->setShortcut(QKeySequence::Close);


    for(int i = 0; i < maxRecentFile; ++i)
    {
        _pRecentAction[i] = new QAction(this);
        _pRecentAction[i]->setVisible(false);
    }

    _pExitAction = new QAction(tr("Exit"), this);
    _pExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    _pVerticalFlipAction = new QAction("Vertical flip", this);
    _pHorizontalFlipAction = new QAction(tr("Horizontal flip"), this);
    _pClockwiseRotateAction = new QAction(tr("Rotate Clockwise"), this);
    _pCounterClockwiseRotateAction = new QAction(tr("Rotate Counter clockwise"), this);
    _pFitAction = new QAction(tr("Fit to window"), this);
    _pZoomInAction = new QAction(tr("Zoom in"), this);
    _pZoomInAction->setShortcut(QKeySequence::ZoomIn);
    _pZoomOutAction = new QAction(tr("Zoom out"), this);
    _pZoomOutAction->setShortcut(QKeySequence::ZoomOut);

    _pAboutAction = new QAction(tr("About"), this);
    _pQtAbout = new QAction(tr("Qt About"), this);
}

void MainWindow::createMenu()
{
    _pFileMenu = menuBar()->addMenu("&File");
    _pFileMenu->addAction(_pNewTabAction);
    _pFileMenu->addSeparator();

    _pFileMenu->addAction(_pOpenAction);
    _pFileMenu->addAction(_pSaveAction);
    _pFileMenu->addAction(_pSaveAsAction);
    _pFileMenu->addSeparator();

    _pFileMenu->addAction(_pCloseFileAction);
    _pFileMenu->addAction(_pCloseTabAction);
    _pSeparatorAction = _pFileMenu->addSeparator();
    for(int i = 0; i < maxRecentFile; ++i)
        _pFileMenu->addAction(_pRecentAction[i]);
    _pFileMenu->addSeparator();
    _pFileMenu->addAction(_pExitAction);

    _pEditMenu = menuBar()->addMenu("&Edit");
    _pEditMenu->addAction(_pVerticalFlipAction);
    _pEditMenu->addAction(_pHorizontalFlipAction);
    _pEditMenu->addSeparator();

    _pEditMenu->addAction(_pClockwiseRotateAction);
    _pEditMenu->addAction(_pCounterClockwiseRotateAction);
    _pEditMenu->addSeparator();

    _pEditMenu->addAction(_pZoomInAction);
    _pEditMenu->addAction(_pZoomOutAction);
    _pEditMenu->addSeparator();

    _pViewMenu = menuBar()->addMenu("&View");
    _pViewMenu->addAction(_pFitAction);

    _pHelpMenu = menuBar()->addMenu("&Help");
    _pHelpMenu->addAction(_pAboutAction);
    _pHelpMenu->addAction(_pQtAbout);
}

QToolBar *MainWindow::createToolBar()
{
    QToolBar *pToolBar = new QToolBar(this);
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    pToolBar->setAllowedAreas(Qt::TopToolBarArea
                              | Qt::BottomToolBarArea);

    pToolBar->addAction(_pPreviousFileAction);
    pToolBar->addAction(_pNextFileAction);
    pToolBar->insertSeparator(_pVerticalFlipAction);

    pToolBar->addAction(_pVerticalFlipAction);
    pToolBar->addAction(_pZoomInAction);
    pToolBar->addAction(_pZoomOutAction);
    pToolBar->addAction(_pFitAction);

    return pToolBar;
}

void MainWindow::createConnectToSlots()
{
    //File menu section
    connect(_pNewTabAction, SIGNAL(triggered(bool)),
            this, SLOT(newTab()));
    connect(_pOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));
    connect(_pSaveAction, SIGNAL(triggered(bool)),
            this, SLOT(saveFile()));
    connect(_pSaveAsAction, SIGNAL(triggered(bool)),
            this, SLOT(saveAs()));
    connect(_pNextFileAction, SIGNAL(triggered(bool)),
            this, SLOT(nextFile()));
    connect(_pPreviousFileAction, SIGNAL(triggered(bool)),
            this, SLOT(previousFile()));
    connect(_pCloseTabAction, SIGNAL(triggered(bool)),
            this, SLOT(closeTabRequest()));
    connect(_pCloseFileAction, SIGNAL(triggered(bool)),
            this, SLOT(closeFileRequest()));
    for(int i = 0; i < maxRecentFile; ++i)
        connect(_pRecentAction[i], SIGNAL(triggered(bool)),
                this, SLOT(openRecentFile()));
    connect(_pExitAction, SIGNAL(triggered(bool)),
            this, SLOT(close()));

    //Edit menu section
    connect(_pHorizontalFlipAction, SIGNAL(triggered(bool)),
            this, SLOT(horizontalFlip()));
    connect(_pVerticalFlipAction, SIGNAL(triggered(bool)),
            this, SLOT(verticalFlip()));
    connect(_pCounterClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(counterClockwiseRotate()));
    connect(_pClockwiseRotateAction, SIGNAL(triggered(bool)),
            this, SLOT(clockwiseRotate()));
    connect(_pZoomInAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomInImage()));
    connect(_pZoomOutAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomOutImage()));

    //View menu section
    connect(_pFitAction, SIGNAL(triggered(bool)),
            this, SLOT(fitImageRequest()));

    //About menu section
    connect(_pAboutAction, SIGNAL(triggered(bool)),
            this, SLOT(aboutApp()));
    connect(_pQtAbout, SIGNAL(triggered(bool)),
            qApp, SLOT(aboutQt()));
}

void MainWindow::showStatusBarMessage(const QString &message)
{
    statusBar()->showMessage(message, 2000);
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
            _pRecentAction[i]->setText(tr("&%1 %2").arg(i + 1)
                                      .arg(QFileInfo(recentFile[i]).fileName()));
            _pRecentAction[i]->setData(recentFile[i]);
            _pRecentAction[i]->setVisible(true);
        }
        else _pRecentAction[i]->setVisible(false);
    }
    _pSeparatorAction->setVisible(!recentFile.isEmpty());
}

void MainWindow::entryList()
{
    QStringList supportedFormats;
    supportedFormats << "*.jpg" << "*.bmp" << "*.png";
    QDir dir(currentPath);
    filesList = dir.entryList(supportedFormats, QDir::Files,
                              QDir::LocaleAware);

    it = filesList.begin();
}

QString MainWindow::getAbsolutePathToFile(const QString &file)
{
    QDir dir(currentPath);
    return dir.absoluteFilePath(file);
}

void MainWindow::fileForLoad(const QString &file)
{
    QString fileForLoad = getAbsolutePathToFile(file);
    loadFileRequest(fileForLoad);

    setRecentFile(fileForLoad);
    updateListRecentFiles();
}

void MainWindow::newTab()
{
    _pTabController->createTab();
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    currentPath,
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg;;"
                                                       "*.ppm;;*.xbm;;*.xpm"));
    if(!filename.isEmpty())
    {
        newTab();
        loadFileRequest(filename);
    }
    setRecentFile(filename);
    updateListRecentFiles();

}

void MainWindow::saveFile()
{
    _pTabController->saveFileOpenedInTab();
}

void MainWindow::saveAs()
{
    const QString filename = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                    currentPath,
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg;;"
                                                       "*.ppm;;*.xbm;;*.xpm"));
    _pTabController->saveAsFileOpenedInTab(filename);
}

void MainWindow::nextFile()
{
    if(*it != filesList.last())
    {
        qDebug() << "nextFile()" << "hasNext section";
        fileForLoad(*it);
        ++it;
    }
    else
    {
        qDebug() << "nextFile()" << "else section";

        fileForLoad(*it);
        it = filesList.cbegin();
        ++it;
    }
    //it++;
    /*if(it == filesList.cbegin())
    {
        fileForLoad(*it);
        ++it;
    }
    else if(*it == filesList.last())
    {
        fileForLoad(*it);
        it = filesList.cbegin();
    }
    else
        fileForLoad(*(it++));*/
}

void MainWindow::previousFile()
{
    if(it != filesList.cbegin())
    {
        qDebug() << "previous file" << "hasPrevous section";
        fileForLoad(*(--it));
    }
    else
    {
        qDebug() << "previous file" << "else section";
        it = filesList.cend();
        fileForLoad(*(--it));
    }
}

void MainWindow::closeFileRequest()
{
    _pTabController->closeImage();
}

qint32 MainWindow::closeTabRequest()
{
    return _pTabController->closeTab(_pTabController->currentIndex());
}

void MainWindow::openRecentFile()
{
    QAction *pAction = qobject_cast<QAction *>(sender());
    loadFileRequest(pAction->data().toString());
}

void MainWindow::horizontalFlip()
{
    _pTabController->horizontalFlip();
}

void MainWindow::verticalFlip()
{
    _pTabController->verticalFlip();
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
    _pTabController->fitImage();
}

void MainWindow::aboutApp()
{
    QMessageBox::about(this, tr("About the application"),
                       tr("A small application for view image.\n"
                          "The programm developed for education and personal satisfaction.\n"));
}

void MainWindow::closeEvent(QCloseEvent *pClose)
{
    /*qint32 userChoise;
    while(_pTabController->count() != 0)
    {
        userChoise = closeTabRequest();
        if(userChoise == QMessageBox::Cancel)
        {
            pClose->ignore();
            return;
        }
    }
    pClose->accept();*/

      ChangedImages *pChanges = new ChangedImages;
      if(!pChanges->isEmpty())
      {
          pChanges->show();
      }
//    if(!pChanges->isEmpty())
//    {
//        pChanges->showChangedImages();
//    }
//    setCentralWidget(pChanges);
//    _pTabController->deleteLater();
}

void MainWindow::loadFileRequest(const QString &file)
{
    _pTabController->loadFiletoTab(file);
}
