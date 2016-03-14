#ifndef SAVECONFIRMATION
#define SAVECONFIRMATION

#include <QHash>
#include <QDialog>

class QString;
class QImage;
class QListWidget;

class SaveConfirmation : public QDialog
{
    Q_OBJECT

public:
    SaveConfirmation(QWidget *pWdg = 0);
    static void addImage(const QString name,
                         const QImage image);
    bool isEmpty();
    static bool imageWasChanged(const QString &name);
    static QImage getChagedImage(const QString &name);
    static void deleteImage(const QString &name);
private slots:
    void saveImages();
private:
    void createItem(const QString &name,
                 const QImage &image);
    static QHash<QString, QImage> images;

    QListWidget *_pListWidget;
};

#endif
