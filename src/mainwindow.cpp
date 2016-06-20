#include "mainwindow.h"
#include "tabcontroller.h"
#include "saveconfirmation.h"
#include "app_properties.h"
#include "about_app.h"
#include "error.h"

#include <memory>
#include <QStringList>
#include <QBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QCoreApplication>
#include <QToolBar>
#include <QStatusBar>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _pTabController(TabController::instance()),
    _pFileSystem(FileSystem::instance())
{
    AppProperties::readSettings();

    createActions();
    createMenu();
    addToolBar(createToolBar());
    createConnectToSlots();
    showStatusBarMessage(QString());

    QCoreApplication::setApplicationVersion(AppProperties::version());
    setGeometry(QRect(200, 200, 800, 500));
    setCentralWidget(_pTabController);
    updateListRecentFiles();

    //init button state
    setButtonsEnabled(false, false);
    setWindowIcon(QIcon(":/icons/png-48px/image-outline.png"));
    QCoreApplication::setApplicationName(AppProperties::name());
}

void MainWindow::createActions()
{
    _pNewTabAction = new QAction(tr("New Tab"), this);
    _pNewTabAction->setShortcut(Qt::CTRL + Qt::Key_T);
    _pNewTabAction->setStatusTip(tr("Create new empty tab"));
    _pNewTabAction->setIcon(QIcon(":/icons/png-48px/plus.png"));

    _pOpenAction = new QAction(tr("Open file"), this);
    _pOpenAction->setShortcut(QKeySequence::Open);
    _pOpenAction->setStatusTip(tr("Open new file"));
    _pOpenAction->setIcon(QIcon(":/icons/png-48px/image-outline.png"));

    _pSaveAction = new QAction(tr("Save file"), this);
    _pSaveAction->setShortcut(QKeySequence::Save);
    _pSaveAction->setStatusTip(tr("Save the file"));
    _pSaveAction->setIcon(QIcon(":/icons/png-48px/download-outline.png"));

    _pSaveAsAction = new QAction(tr("Save file as"), this);
    _pSaveAsAction->setIcon(QIcon(":/icons/png-48px/th-list.png"));

    _pNextFileAction = new QAction(tr("Next file"), this);
    _pNextFileAction->setShortcut(Qt::Key_Right);
    _pNextFileAction->setIcon(QIcon(":/icons/png-48px/arrow-right-thick.png"));

    _pPreviousFileAction = new QAction(tr("Previous file"), this);
    _pPreviousFileAction->setShortcut(Qt::Key_Left);
    _pPreviousFileAction->setIcon(QIcon(":/icons/png-48px/arrow-left-thick.png"));

    _pCloseFileAction = new QAction(tr("Close image"), this);
    _pCloseFileAction->setStatusTip(tr("Closing image"));
    _pCloseFileAction->setIcon(QIcon(":/icons/png-48px/delete-outline.png"));

    _pCloseTabAction = new QAction(tr("Close tab"), this);
    _pCloseTabAction->setShortcut(Qt::CTRL + Qt::Key_W);
    _pCloseTabAction->setStatusTip(tr("Close current tab"));
    _pCloseTabAction->setIcon(QIcon(":/icons/png-48px/minus.png"));

    for(auto i = 0; i < _pRecentAction.size(); ++i)
    {
        _pRecentAction[i] = new QAction(this);
        _pRecentAction.at(i)->setVisible(false);
    }

    _pExitAction = new QAction(tr("Exit"), this);
    _pExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    _pExitAction->setStatusTip(tr("Close the application"));
    _pExitAction->setIcon(QIcon(":/icons/png-48px/power.png"));

    _pVerticalFlipAction = new QAction(tr("Vertical flip"), this);
    _pVerticalFlipAction->setIcon(QIcon(":/icons/png-48px/arrow-vertical-flip.png"));

    _pHorizontalFlipAction = new QAction(tr("Horizontal flip"), this);
    _pHorizontalFlipAction->setIcon(QIcon(":/icons/png-48px/arrow-horizontal-flip.png"));

    _pClockwiseRotateAction = new QAction(tr("Rotate Clockwise"), this);
    _pClockwiseRotateAction->setIcon(QIcon(":/icons/png-48px/Clockwise.png"));

    _pCounterClockwiseRotateAction = new QAction(tr("Rotate Counter clockwise"), this);
    _pCounterClockwiseRotateAction->setIcon(QIcon(":/icons/png-48px/CounterClockwise.png"));

    _pFitAction = new QAction(tr("Fit to window"), this);
    _pFitAction->setIcon(QIcon(":/icons/png-48px/zoom.png"));

    _pZoomInAction = new QAction(tr("Zoom in"), this);
    _pZoomInAction->setShortcut(QKeySequence::ZoomIn);
    _pZoomInAction->setIcon(QIcon(":/icons/png-48px/zoom-in.png"));

    _pZoomOutAction = new QAction(tr("Zoom out"), this);
    _pZoomOutAction->setShortcut(QKeySequence::ZoomOut);
    _pZoomOutAction->setIcon(QIcon(":/icons/png-48px/zoom-out.png"));

    _pAboutAction = new QAction(tr("About"), this);
    _pAboutAction->setIcon(QIcon(":/icons/png-48px/business-card.png"));

    _pQtAbout = new QAction(tr("About Qt"), this);
}

void MainWindow::createMenu()
{
    _pFileMenu = menuBar()->addMenu(tr("&File"));
    _pFileMenu->addAction(_pNewTabAction);
    _pFileMenu->addSeparator();

    _pFileMenu->addAction(_pOpenAction);
    _pFileMenu->addAction(_pSaveAction);
    _pFileMenu->addAction(_pSaveAsAction);
    _pFileMenu->addSeparator();

    _pFileMenu->addAction(_pCloseFileAction);
    _pFileMenu->addAction(_pCloseTabAction);
    _pSeparatorAction = _pFileMenu->addSeparator();
    for(auto i = 0; i < _pRecentAction.size(); ++i)
        _pFileMenu->addAction(_pRecentAction.at(i));
    _pFileMenu->addSeparator();
    _pFileMenu->addAction(_pExitAction);

    _pEditMenu = menuBar()->addMenu(tr("&Edit"));
    _pEditMenu->addAction(_pVerticalFlipAction);
    _pEditMenu->addAction(_pHorizontalFlipAction);
    _pEditMenu->addSeparator();

    _pEditMenu->addAction(_pClockwiseRotateAction);
    _pEditMenu->addAction(_pCounterClockwiseRotateAction);
    _pEditMenu->addSeparator();

    _pEditMenu->addAction(_pZoomInAction);
    _pEditMenu->addAction(_pZoomOutAction);
    _pEditMenu->addSeparator();

    _pViewMenu = menuBar()->addMenu(tr("&View"));
    _pViewMenu->addAction(_pFitAction);

    _pHelpMenu = menuBar()->addMenu(tr("&Help"));
    _pHelpMenu->addAction(_pAboutAction);
    _pHelpMenu->addAction(_pQtAbout);
}

QToolBar *MainWindow::createToolBar()
{
    _pToolBar = new QToolBar(this);
    _pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    _pToolBar->setAllowedAreas(Qt::TopToolBarArea
                              | Qt::BottomToolBarArea);

    _pToolBar->addAction(_pPreviousFileAction);
    _pToolBar->addAction(_pNextFileAction);
    _pToolBar->addSeparator();

    _pToolBar->addAction(_pVerticalFlipAction);
    _pToolBar->addAction(_pHorizontalFlipAction);
    _pToolBar->addAction(_pClockwiseRotateAction);
    _pToolBar->addAction(_pCounterClockwiseRotateAction);
    _pToolBar->addSeparator();
    _pToolBar->addAction(_pZoomInAction);
    _pToolBar->addAction(_pZoomOutAction);
    _pToolBar->addAction(_pFitAction);

    return _pToolBar;
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
    for(auto i = 0; i < _pRecentAction.size(); ++i)
        connect(_pRecentAction.at(i), SIGNAL(triggered(bool)),
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

void MainWindow::updateListRecentFiles()
{
    QStringList &recentFile = AppProperties::recentFiles();
    QMutableStringListIterator iter = recentFile;
    while(iter.hasNext())
    {
        if(!QFile::exists(iter.next()))
            iter.remove();
    }
    for(auto i = 0; i < _pRecentAction.size(); ++i)
    {
        if(i < recentFile.count())
        {
            _pRecentAction.at(i)->setText(tr("&%1 %2").arg(i + 1)
                                      .arg(QFileInfo(recentFile.at(i)).fileName()));
            _pRecentAction.at(i)->setData(recentFile.at(i));
            _pRecentAction.at(i)->setVisible(true);
        }
        else _pRecentAction.at(i)->setVisible(false);
    }
    _pSeparatorAction->setVisible(!recentFile.isEmpty());
}

void MainWindow::newTab()
{
    _pTabController->createTab();
}

void MainWindow::openFile()
{
    QString file;
    try
    {
        file = _pFileSystem->openFile();
        loadFileRequest(file);
    }
    catch (std::runtime_error &err)
    {
        Error::showError(QString (err.what()));
    }
    catch (const QString &)
    {}
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
    bool success;
    try
    {
        success = _pFileSystem->saveAs();
    }
    catch (std::runtime_error &err)
    {
        Error::showError(QString (err.what()));
    }
    catch (const QString &)
    {}

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
    About about;
    about.exec();
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
    if(!SaveConfirmation::isEmpty())
    {
        std::unique_ptr<SaveConfirmation> pChanges(new SaveConfirmation);
        connect(pChanges.get(), SIGNAL(hide()),
                this, SLOT(hide()));
        qint32 ret = pChanges->exec();

        if(ret == QDialog::Accepted)
        {
            AppProperties::saveSettings();
            pClose->accept();
            qDebug("accept mainwin");
        }
        else if(ret == QDialog::Rejected)
            pClose->ignore();
    }
    else
    {
        AppProperties::saveSettings();
        pClose->accept();
        qDebug("else accept");
    }
}

void MainWindow::loadFileRequest(const QString &file)
{
    _pTabController->loadFiletoTab(file);
    AppProperties::addRecentFile(file);
    updateListRecentFiles();
}

void MainWindow::setButtonsEnabled(bool openButt, bool other)
{
    //if at leat one tab is exist
    _pOpenAction->setEnabled(openButt);
    _pCloseTabAction->setEnabled(openButt);

    _pSaveAction->setEnabled(other);
    _pSaveAsAction->setEnabled(other);
    _pNextFileAction->setEnabled(other);
    _pPreviousFileAction->setEnabled(other);

    for(auto i = 0; i < _pRecentAction.size(); ++i)
        _pRecentAction[i]->setEnabled(other);

    _pCloseFileAction->setEnabled(other);

    _pVerticalFlipAction->setEnabled(other);
    _pHorizontalFlipAction->setEnabled(other);
    _pClockwiseRotateAction->setEnabled(other);
    _pCounterClockwiseRotateAction->setEnabled(other);
    _pFitAction->setEnabled(other);
    _pZoomInAction->setEnabled(other);
    _pZoomOutAction->setEnabled(other);
}
