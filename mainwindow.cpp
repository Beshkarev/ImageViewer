#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QDir>
#include "screenimage.h"
#include <QBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    //_pTabWidget(new QTabWidget(this)),
    _pScreenImage(new ScreenImage(this))
{
    ui->setupUi(this);
    createActions();
    createMenu();
    createConnectToSlots();
    //_pTabWidget->addTab(_pScreenImage, "tab1");
    //QHBoxLayout *pHLayout = new QHBoxLayout(this);
    //pHLayout->addWidget(_pScreenImage);
   // _pTabWidget->setAttribute(Qt::WA_DeleteOnClose);

    setCentralWidget(_pScreenImage);
    //setAttribute(Qt::WA_DeleteOnClose);
    //setCentralWidget(_pTabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    _pNewTabAction = new QAction(tr("New Tab"), this);
    _pNewTabAction->setShortcut(Qt::CTRL + Qt::Key_T);
    _pNewTabAction->setStatusTip("Creating new tab");

    pOpenAction = new QAction(tr("Open file"), this);
    pOpenAction->setShortcut(QKeySequence::Open);
    pOpenAction->setStatusTip("Opend new file");
    //connect(pOpenAction, SIGNAL(triggered(bool)), this, SLOT(openFileSlot()));

    pSaveAction = new QAction(tr("Save file"), this);
    pSaveAction->setShortcut(QKeySequence::Save);
    pSaveAction->setStatusTip(tr("Save the file"));
    //connect(pSaveAction, SIGNAL(triggered(bool)), this, SLOT(saveFileSlot()));
    //pSaveAction->setEnabled(false);

    _pCloseImageAction = new QAction(tr("Close image"), this);
    _pCloseImageAction->setShortcut(QKeySequence::Close);
    //connect(pCloseImageAction, SIGNAL(triggered(bool)), this, SLOT(closeFileSlot()));
    //pCloseImageAction->setEnabled(false);

    for(int i = 0; i < maxRecentFile; ++i)
    {
        pRecentAction[i] = new QAction(this);
        pRecentAction[i]->setVisible(false);
        //connect(pRecentAction[i], SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
    }
    //RecentFileUpdate();

    _pExitAction = new QAction(tr("Exit"), this);
    _pExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    //connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    pHorizontalFlipAction = new QAction("Horizontal flip", this);

    _pClockwiseRotateAction = new QAction("Rotate Clockwise", this);
    //connect(pClockwiseRotateAction, SIGNAL(triggered(bool)), this, SLOT(rotateImageSlot()));
    _pCounterClockwiseRotateAction = new QAction("Rotate Counter clockwise", this);
    //connect(pCounterClock_WiseRotateAction, SIGNAL(triggered(bool)), this, SLOT(rotateImageSlot()));

    pFitAction = new QAction("Fit to window", this);
    pFitAction->setCheckable(true);
    //connect(pFitAction, SIGNAL(triggered(bool)), this, SLOT(fitToWindow()));

    pZoomIn = new QAction("Zoom in", this);
    pZoomIn->setShortcut(Qt::CTRL + Qt::Key_Plus);
    //connect(pZoomIn, SIGNAL(triggered(bool)), this, SLOT(scaleImage()));

    pZoomOut = new QAction("Zoom out", this);
    pZoomOut->setShortcut(Qt::CTRL + Qt::Key_Minus);
    //connect(pZoomOut, SIGNAL(triggered(bool)), this, SLOT(scaleImage()));

    pAboutAction = new QAction(tr("About"), this);
   // connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));

    pQtAbout = new QAction(tr("Qt About"), this);
}

void MainWindow::createMenu()
{
    pFileMenu = menuBar()->addMenu("&File");
    pFileMenu->addAction(_pNewTabAction);
    pFileMenu->addAction(pOpenAction);
    pFileMenu->addAction(pSaveAction);
    pFileMenu->addAction(_pCloseImageAction);
    pSeparatorAction = pFileMenu->addSeparator();
    for(int i = 0; i < maxRecentFile; ++i)
        pFileMenu->addAction(pRecentAction[i]);
    pFileMenu->addSeparator();
    pFileMenu->addAction(_pExitAction);

    pEditMenu = menuBar()->addMenu("&Edit");
    pEditMenu->addAction(pHorizontalFlipAction);
    pEditMenu->addAction(_pClockwiseRotateAction);
    pEditMenu->addAction(_pCounterClockwiseRotateAction);
    pEditMenu->addAction(pFitAction);
    pEditMenu->addAction(pZoomIn);
    pEditMenu->addAction(pZoomOut);

    pHelpMenu = menuBar()->addMenu("&Help");
    pHelpMenu->addAction(pAboutAction);
    pHelpMenu->addAction(pQtAbout);
}

void MainWindow::createConnectToSlots()
{
    //File menu section
    connect(_pNewTabAction, SIGNAL(triggered(bool)),
            this, SLOT(newTab()));
    connect(pOpenAction, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(loadImage()));
    connect(pSaveAction, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(saveImage()));
    connect(_pCloseImageAction, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(closeImage()));
    connect(_pExitAction, SIGNAL(triggered(bool)),
            this, SLOT(close()));

    //Edit menu section
    connect(pHorizontalFlipAction, SIGNAL(triggered(bool)), _pScreenImage, SLOT(horizontalFlip()));
    connect(_pCounterClockwiseRotateAction, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(counterClockwiseRotate()));
    connect(_pClockwiseRotateAction, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(clockwiseRotate()));
//    connect(pFitAction, SIGNAL(triggered(bool)), this, SLOT(fitToWindow()));
    connect(pZoomIn, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(zoomInImage()));
    connect(pZoomOut, SIGNAL(triggered(bool)),
            _pScreenImage, SLOT(zoomOutImage()));

    //About menu section
    connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutApp()));
    connect(pQtAbout, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
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
    QMessageBox::information(this, "Sorry",
                             "Not implemented yet",
                             QMessageBox::Ok);
    //ScreenImage *pChildImage = new ScreenImage;
    //pChildImage->show();
}

void MainWindow::aboutApp()
{
    QMessageBox::about(this, tr("About the application"),
                       tr("A small application for view image.\n"
                          "The programm developed for education and personal satisfaction.\n"));
}

void MainWindow::closeEvent(QCloseEvent *pClose)
{
    if(_pScreenImage->isChanged())
    {
        qint32 chose;
        chose = QMessageBox::warning(this, tr("Save changes"),
                                     tr("The image has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);

        if(chose == QMessageBox::Yes)
        {
            emit pSaveAction->trigger();
            pClose->accept();
        }
        else if(chose == QMessageBox::No)
            pClose->accept();
        else if(chose == QMessageBox::Cancel)
            pClose->ignore();
    }
    else
        pClose->accept();
}

//ScreenImage *MainWindow::createScreenImage()
//{
//    ScreenImage *pImg = new ScreenImage;
//    _pTabWidget->addTab(pImg, "");
//}
