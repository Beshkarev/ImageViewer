#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QAction;
class TabController;
class FileSystem;
class QStringList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void createActions();
    void createMenu();
    QToolBar *createToolBar();
    void createConnectToSlots();
    void showStatusBarMessage(const QString &message);
    void setRecentFile(const QString &filename);
    void updateListRecentFiles();
private slots:
    void newTab();
    void openFile();
    void saveFile();
    void saveAs();
    void nextFile();
    void previousFile();
    void closeFileRequest();
    void closeTabRequest();
    void openRecentFile();
    void horizontalFlip();
    void verticalFlip();
    void clockwiseRotate();
    void counterClockwiseRotate();
    void zoomInImage();
    void zoomOutImage();
    void fitImageRequest();

    void aboutApp();

    void checkTabState();
protected:
    void closeEvent(QCloseEvent *);
private:

    void loadFileRequest(const QString &file);
    void setButtonsEnabled(bool openButt, bool other);

    TabController *_pTabController;
    FileSystem *_pFileSystem;
    QStringList recentFile;

    const size_t maxRecentFile = 5;

    QMenu *_pFileMenu;
    QMenu *_pEditMenu;
    QMenu *_pViewMenu;
    QMenu *_pHelpMenu;
    QAction *_pNewTabAction;
    QAction *_pOpenAction;
    QAction *_pSaveAction;
    QAction *_pSaveAsAction;
    QAction *_pNextFileAction;
    QAction *_pPreviousFileAction;
    QAction *_pCloseFileAction;
    QAction *_pCloseTabAction;
    QVector<QAction*> _pRecentAction;
    QAction *_pSeparatorAction;
    QAction *_pExitAction;

    QAction *_pVerticalFlipAction;
    QAction *_pHorizontalFlipAction;
    QAction *_pClockwiseRotateAction;
    QAction *_pCounterClockwiseRotateAction;
    QAction *_pFitAction;
    QAction *_pZoomInAction;
    QAction *_pZoomOutAction;

    QAction *_pAboutAction;
    QAction *_pQtAbout;
};

#endif // MAINWINDOW_H
