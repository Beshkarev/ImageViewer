#include "app_properties.h"

AppProrepties::AppProrepties() : appVersion("0.6")
{}

QString AppProrepties::version()
{
    return appVersion;
}
