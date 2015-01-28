#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:37:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mainApplication
TEMPLATE = app


SOURCES += main.cpp\
        bow.cpp \
    functions.cpp \
    imagelistmodel.cpp

HEADERS  += bow.h \
    functions.hpp \
    imagelistmodel.h \
    CONV.h

FORMS    += bow.ui

CONFIG  += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

unix|win32: LIBS += -lboost_filesystem -lboost_system

OTHER_FILES +=

RESOURCES += \
    res.qrc

