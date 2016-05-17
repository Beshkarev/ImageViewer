#ifndef SAVECONFIRMATION
#define SAVECONFIRMATION

#include <QHash>
#include <QDialog>
#include <memory>
#include <QListWidget>
#include <QDir>

class QString;
class QImage;
class QListWidget;

class SaveConfirmation : public QDialog
{
    Q_OBJECT

    typedef QString originalLocationWithName;
    typedef QString tempLocationWithName;
    typedef QHash<originalLocationWithName,
                  tempLocationWithName> changedImages;

public:
    SaveConfirmation(QWidget *pWdg = 0);
    ~SaveConfirmation();
    static void addImage(const QString &imageName,
                         const QImage &image);
    bool isEmpty();
    static bool imageWasChanged(const QString &name);
    static QImage getChagedImage(const QString &name);
    static void deleteImage(const QString &name);
private slots:
    void clearDiskSpace();
    void saveImages();
private:
    void createItem(const QString &name,
                 const QImage &image);
//    static QHash<QString, QImage> images;

    static changedImages images;

    QDir dir;
    std::unique_ptr<QListWidget> _pListWidget;
};

#endif
