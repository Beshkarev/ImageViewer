#include "app_properties.h"

#include <QString>

AppProrepties::AppProrepties() : _appVersion("0.7")
{}

QString AppProrepties::version()
{
    return _appVersion;
}
