#include "mainwindow.h"
#include "tabcontroller.h"
#include "saveconfirmation.h"
#include "filesystem.h"
#include "app_properties.h"

#include <memory>
#include <QStringList>
#include <QBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QCoreApplication>
#include <QToolBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabController(TabController::instance()),
    _pFileSystem(new FileSystem)
{
    createActions();
    createMenu();
    addToolBar(createToolBar());
    createConnectToSlots();

    AppProrepties app;
    QCoreApplication::setApplicationVersion(app.version());
    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabController);
    updateListRecentFiles();

    //init button state
    setButtonsEnabled(false, false);
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
    _pQtAbout = new QAction(tr("About Qt"), this);
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

    //For buttons enable/disable
    connect(_pTabController, SIGNAL(tabClosed()),
            this, SLOT(checkTabState()));
    connect(_pTabController, SIGNAL(tabCreated()),
            this, SLOT(checkTabState()));
    connect(_pTabController, SIGNAL(currentChanged(int)),
            this, SLOT(checkTabState()));
    connect(_pTabController, SIGNAL(tabStateChanged()),
            this, SLOT(checkTabState()));
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

void MainWindow::newTab()
{
    _pTabController->createTab();

}

void MainWindow::openFile()
{
    QString file = _pFileSystem->openFile();
    loadFileRequest(file);
}

void MainWindow::saveFile()
{
    bool success = _pFileSystem->saveFile();
    if(success)
        showStatusBarMessage(tr("File was saved"));
    else if(!success)
        showStatusBarMessage(tr("File not saved"));
}

void MainWindow::saveAs()
{
    bool success = _pFileSystem->saveAs();
    if(success)
        showStatusBarMessage(tr("File was saved"));
    else if(!success)
        showStatusBarMessage(tr("File not saved"));
}

void MainWindow::nextFile()
{
    QString file = _pFileSystem->nextFile();
    loadFileRequest(file);
}

void MainWindow::previousFile()
{
    QString file = _pFileSystem->previousFile();
    loadFileRequest(file);
}

void MainWindow::closeFileRequest()
{
    _pTabController->closeImage();
}

void MainWindow::closeTabRequest()
{
    _pTabController->closeTab(_pTabController->currentIndex());
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

void MainWindow::checkTabState()
{
    int count = _pTabController->count();
    //if tabs not exist
    if(count == 0)
    {
        setButtonsEnabled(false, false);
        return;//else program has unexpectedly finished
    }

    bool tabEmpty = _pTabController->currentTabIsEmpty();
    //if exist at least one tab but the tab is empty
    if(count != 0 && tabEmpty)
        setButtonsEnabled(true, false);
    //if tabs is exist and the tab not expty
    else if(count != 0 && !tabEmpty)
        setButtonsEnabled(true, true);
}

void MainWindow::closeEvent(QCloseEvent *pClose)
{
    std::shared_ptr<SaveConfirmation> pChanges(new SaveConfirmation);
    if(!pChanges->isEmpty())
    {
        qint32 ret;
        ret = pChanges->exec();

        if(ret == QDialog::Accepted)
            pClose->accept();

        else if(ret == QDialog::Rejected)
            pClose->ignore();
    }
    else
        pClose->accept();
}

void MainWindow::loadFileRequest(const QString &file)
{
    _pTabController->loadFiletoTab(file);
    setRecentFile(file);
    updateListRecentFiles();
}

void MainWindow::setButtonsEnabled(bool openButt, bool other)
{
    //if each one tab is exist
    _pOpenAction->setEnabled(openButt);

    //if tab not empty(has visible image)
    _pSaveAction->setEnabled(other);
    _pSaveAsAction->setEnabled(other);
    _pNextFileAction->setEnabled(other);
    _pPreviousFileAction->setEnabled(other);
    _pCloseFileAction->setEnabled(other);
    _pCloseTabAction->setEnabled(other);

    _pVerticalFlipAction->setEnabled(other);
    _pHorizontalFlipAction->setEnabled(other);
    _pClockwiseRotateAction->setEnabled(other);
    _pCounterClockwiseRotateAction->setEnabled(other);
    _pFitAction->setEnabled(other);
    _pZoomInAction->setEnabled(other);
    _pZoomOutAction->setEnabled(other);
}
