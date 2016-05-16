#include "app_properties.h"
#include <QString>

const QString AppProrepties::_appVersion = "0.7";
const QStringList AppProrepties::_supportedFormats = QStringList() << "*.jpg" << "*.bmp"
                                                               << "*.png" << "*jpeg"
                                                               << "*.gif";
const QString AppProrepties::_name = "Image viewer";

//AppProrepties::AppProrepties() : _appVersion("0.7"),
//    _name("Image viewer"),
//    _supportedSaveFormats("*.jpg" << "*.bmp"
//                                  << "*.png" << "*jpeg"
//                                  << "*.gif")
//{}
