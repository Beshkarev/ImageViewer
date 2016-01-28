#ifndef CHANGEDIMAGES
#define CHANGEDIMAGES

#include <QMap>
#include <QDialog>

class QString;
class QImage;
class QTableWidget;

class ChangedImages : public QDialog
{
public:
    ChangedImages(QWidget *pWdg = 0);
    static void addImage(const QString &name,
                         const QImage &image);
    bool isEmpty();
    //void showChangedImages();
private:
    QTableWidget *_pTableWidget;
    static QMap<QString, QImage> images;
};

class QCheckBox;
class QLabel;

class TableWidgetItem : public QWidget
{
public:
    TableWidgetItem(const QString &name,
                    const QImage &img,
                    QWidget *pWdg = 0);
    bool isChecked();
private:
    QCheckBox *_pCheckBox;
};

#endif // CHANGEDIMAGES

