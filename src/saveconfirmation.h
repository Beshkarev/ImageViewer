#ifndef SAVECONFIRMATION
#define SAVECONFIRMATION

#include <QMap>
#include <QDialog>

class QString;
class QImage;
class QListWidget;
class QListWidgetItem;

class SaveConfirmation : public QDialog
{
    Q_OBJECT

public:
    SaveConfirmation(QWidget *pWdg = 0);
    static void addImage(const QString &name,
                         const QImage &image);
    bool isEmpty();
private slots:
    void saveImages();
private:
    void createItem(const QString &name,
                 const QImage &image);
    static QMap<QString, QImage> images;

    QListWidget *_pListWidget;
};

#endif
