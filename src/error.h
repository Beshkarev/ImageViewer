#ifndef ERROR_H
#define ERROR_H

#include <QObject>

class QString;

namespace Error
{
    void showError (const QString &textOfError,
                    const QString &title = QObject::tr("Opps"));
}

#endif // ERROR_H
