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
class QThread;

class SaveConfirmation : public QDialog
{
    Q_OBJECT

    //typedef QString originalLocationWithName;
    using originalLocationWithName = QString;
    //typedef QString tempLocationWithName;
    using tempLocationWithName = QString;
    //typedef QHash<originalLocationWithName,
    //              tempLocationWithName> changedImages;
    using changedImages = QHash<originalLocationWithName,
                  tempLocationWithName>;

public:
    SaveConfirmation(QWidget *pWdg = 0);
    ~SaveConfirmation();
    static void addImage(const QString &imageName,
                         const QImage &image);
    static bool isEmpty();
    static bool imageWasChanged(const QString &name);
    static QImage getChagedImage(const QString &name);
    static void deleteImage(const QString &name);

private slots:
    void saveImages();

signals:
    void hide();

private:
    void createItem(const QString &name,
                 const QImage &image);
//    static QHash<QString, QImage> images;

    static changedImages images;

    QDir dir;
    std::unique_ptr<QListWidget> _pListWidget;
//    QThread *pThread;

};

#endif
