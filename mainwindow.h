#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class TabController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void createActions();
    void createMenu();
    void createConnectToSlots();
    void setRecentFile(const QString &filename);
    void updateListRecentFiles();
private slots:
    void newTab();
    void open();
    void save();
    void closeTabRequest();
    void horizontalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    void fitImageRequest();

    void aboutApp();
protected:
    void closeEvent(QCloseEvent *);
private:

    TabController *_pTabController;
    QStringList recentFile;
    enum{maxRecentFile = 5};

    QMenu *_pFileMenu;
    QMenu *_pEditMenu;
    QMenu *_pViewMenu;
    QMenu *_pHelpMenu;
    QAction *_pNewTabAction;
    QAction *_pOpenAction;
    QAction *_pSaveAction;
    QAction *_pCloseImageAction;
    QAction *pRecentAction[maxRecentFile];
    QAction *pSeparatorAction;
    QAction *_pExitAction;

    QAction *_pHorizontalFlipAction;
    QAction *_pClockwiseRotateAction;
    QAction *_pCounterClockwiseRotateAction;
    QAction *_pFitAction;
    QAction *_pNormalSizeAction;
    QAction *_pZoomIn;
    QAction *_pZoomOut;

    QAction *_pAboutAction;
    QAction *_pQtAbout;
};

#endif // MAINWINDOW_H
