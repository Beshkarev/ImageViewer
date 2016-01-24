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

INCLUDEPATH += /usr/local/include/opencv-3.1.0/include/
LIBS += -L/usr/local/include/opencv-3.1.0/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/screenimage.cpp \
    src/tabcontroller.cpp

HEADERS  += src/mainwindow.h \
    src/screenimage.h \
    src/tabcontroller.h

FORMS    +=
