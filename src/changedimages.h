#ifndef CHANGEDIMAGES
#define CHANGEDIMAGES

#include <QMap>
#include <QDialog>

class QString;
class QImage;
class QTreeWidgetItem;

class ChangedImages : public QDialog
{
public:
    ChangedImages(QWidget *pWdg = 0);
    static void addImage(const QString &name,
                         const QImage &image);
    bool isEmpty();
private:
    void addTreeItem(QTreeWidgetItem *parent,
                     const QString &name,
                     const QImage &image);
    static QMap<QString, QImage> images;
};

#endif // CHANGEDIMAGES

