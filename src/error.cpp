#include "error.h"

#include <QString>
#include <QMessageBox>


void Error::showError(const QString &textOfError,
                      const QString &title)
{
    QMessageBox::warning(nullptr, title, textOfError,
                         QMessageBox::Ok);
}
