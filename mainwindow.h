#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QAction;
class QTabWidget;
class ScreenImage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();
    void createMenu();
    void createConnectToSlots();
    void setRecentFile(const QString &filename);
    void updateListRecentFiles();
private slots:
    void newTab();
    void aboutApp();
protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
    // QTabWidget *_pTabWidget;
    ScreenImage *_pScreenImage;
   // ScreenImage* createScreenImage();


    QStringList recentFile;
    enum{maxRecentFile = 5};

    QMenu *pFileMenu;
    QMenu *pEditMenu;
    QMenu *pHelpMenu;
    QAction *_pNewTabAction;
    QAction *pOpenAction;
    QAction *pSaveAction;
    QAction *_pCloseImageAction;
    QAction *pRecentAction[maxRecentFile];
    QAction *pSeparatorAction;
    QAction *_pExitAction;

    QAction *pHorizontalFlipAction;
    QAction *_pClockwiseRotateAction;
    QAction *_pCounterClockwiseRotateAction;
    QAction *pFitAction;
    QAction *pNormalSizeAction;
    QAction *pZoomIn;
    QAction *pZoomOut;

    QAction *pAboutAction;
    QAction *pQtAbout;
};

#endif // MAINWINDOW_H
