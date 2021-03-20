include(../../qtproject.pri)
#include(../../src/libs/)

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stockdemo
TEMPLATE = app
DESTDIR = $$IDE_APP_PATH

LIBS *= -l$$qtLibraryNameVersion(network, 1)

INCLUDEPATH += ..

CONFIG += c++11

SOURCES += \
    main.cpp
