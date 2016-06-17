#-------------------------------------------------
#
# Project created by QtCreator 2015-12-13T17:10:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

#INCLUDEPATH += /usr/local/include/opencv-3.1.0/include/
#LIBS += -L/usr/local/include/opencv-3.1.0/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/screenimage.cpp \
    src/tabcontroller.cpp \
    src/saveconfirmation.cpp \
    src/filesystem.cpp \
    src/about_app.cpp \
    src/entry.cpp \
    src/app_properties.cpp

HEADERS  += src/mainwindow.h \
    src/screenimage.h \
    src/tabcontroller.h \
    src/saveconfirmation.h \
    src/filesystem.h \
    src/about_app.h \
    src/entry.h \
    src/basic_types.h \
    src/app_properties.h

FORMS    +=

RESOURCES += \
    res/res.qrc
