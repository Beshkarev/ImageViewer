#include "mainwindow.h"
#include "tabcontroller.h"
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
#include <QStringListIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabController(new TabController),
    filesLisIterator(QStringListIterator(filesList)),
    _pToolBar(new QToolBar)
{
    createActions();
    createMenu();
    createToolBar();
    createConnectToSlots();
    qDebug() << "constructor";

    entryList();

    QCoreApplication::setApplicationVersion("0.5");
    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabController);
    updateListRecentFiles();
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
        pRecentAction[i] = new QAction(this);
        pRecentAction[i]->setVisible(false);
    }

    _pExitAction = new QAction(tr("Exit"), this);
    _pExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    _pHorizontalFlipAction = new QAction("Horizontal flip", this);
    _pClockwiseRotateAction = new QAction("Rotate Clockwise", this);
    _pCounterClockwiseRotateAction = new QAction("Rotate Counter clockwise", this);
    _pFitAction = new QAction("Fit to window", this);
    _pFitAction->setCheckable(true);
    _pZoomIn = new QAction("Zoom in", this);
    _pZoomIn->setShortcut(QKeySequence::ZoomIn);
    _pZoomOut = new QAction("Zoom out", this);
    _pZoomOut->setShortcut(QKeySequence::ZoomOut);

    _pAboutAction = new QAction(tr("About"), this);
    _pQtAbout = new QAction(tr("Qt About"), this);
}

void MainWindow::createMenu()
{
    _pFileMenu = menuBar()->addMenu("&File");
    _pFileMenu->addAction(_pNewTabAction);
    _pFileMenu->addAction(_pOpenAction);
    _pFileMenu->addAction(_pSaveAction);
    _pFileMenu->addAction(_pNextFileAction);
    _pFileMenu->addAction(_pPreviousFileAction);
    _pFileMenu->addAction(_pCloseFileAction);
    _pFileMenu->addAction(_pCloseTabAction);
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

void MainWindow::createToolBar()
{
    _pToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    _pToolBar = addToolBar("file");

    _pToolBar->addAction(_pPreviousFileAction);
    _pToolBar->addAction(_pNextFileAction);
}

void MainWindow::createConnectToSlots()
{
    qDebug("connect");
    //File menu section
    connect(_pNewTabAction, SIGNAL(triggered(bool)),
            this, SLOT(newTab()));
    connect(_pOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));
    connect(_pSaveAction, SIGNAL(triggered(bool)),
            this, SLOT(saveFile()));
    connect(_pNextFileAction, SIGNAL(triggered(bool)),
            this, SLOT(nextFile()));
    connect(_pPreviousFileAction, SIGNAL(triggered(bool)),
            this, SLOT(previousFile()));
    connect(_pCloseTabAction, SIGNAL(triggered(bool)),
            this, SLOT(closeTabRequest()));
    connect(_pCloseFileAction, SIGNAL(triggered(bool)),
            this, SLOT(closeFileRequest()));
    for(int i = 0; i < maxRecentFile; ++i)
        connect(pRecentAction[i], SIGNAL(triggered(bool)),
                this, SLOT(openRecentFile()));
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
    connect(_pAboutAction, SIGNAL(triggered(bool)),
            this, SLOT(aboutApp()));
    connect(_pQtAbout, SIGNAL(triggered(bool)),
            qApp, SLOT(aboutQt()));
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

void MainWindow::entryList()
{
    QStringList supportedFormats;
    supportedFormats << "*.jpg" << "*.bmp" << ".png";
    QDir dir(QDir::homePath() + "/Pictures/");
    filesList = dir.entryList(supportedFormats, QDir::Files,
                              QDir::LocaleAware);

    filesLisIterator = QStringListIterator(filesList);
}

QString MainWindow::getAbsolutePathToFile(const QString &file)
{
    QDir dir(QDir::homePath() + "/Pictures");
    return dir.absoluteFilePath(file);
}

void MainWindow::newTab()
{
    _pTabController->createTab();
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    QDir::homePath() + "/Pictures",
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;*.jpeg"
                                                       "*.ppm;;*.xbm;;*.xpm"));
    if(!filename.isEmpty())
        loadFileRequest(filename);

    setRecentFile(filename);
    updateListRecentFiles();
}

void MainWindow::saveFile()
{
    const QString filename = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                    QDir::homePath() + "/Pictures",
                                                    tr("All (*.*);;*.jpg;;*.bmp;;*.png;;"));
    _pTabController->saveFileOpenedInTab(filename);
}
//Переделать
void MainWindow::nextFile()
{
    QString file;

    if(filesLisIterator.hasNext())
    {
        file = getAbsolutePathToFile(filesLisIterator.next());
        loadFileRequest(file);
    }
    else
    {
        filesLisIterator.toFront();
        file = getAbsolutePathToFile(filesLisIterator.next());
        loadFileRequest(file);
    }
    setRecentFile(file);
    updateListRecentFiles();
}
//Переделать
void MainWindow::previousFile()
{
    QString file;

    if(filesLisIterator.hasPrevious())
    {
        file = getAbsolutePathToFile(filesLisIterator.previous());
        loadFileRequest(file);
    }
    else
    {
        filesLisIterator.toBack();
        file = getAbsolutePathToFile(filesLisIterator.previous());
        loadFileRequest(file);
    }
    setRecentFile(file);
    updateListRecentFiles();
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

}

void MainWindow::loadFileRequest(const QString &file)
{
    _pTabController->loadFiletoTab(file);
}
