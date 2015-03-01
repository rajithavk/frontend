#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:37:59
#
#-------------------------------------------------

QT       += core gui widgets network opengl websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = subdirs #mainApplication
TEMPLATE = app


SOURCES += main.cpp\
        bow.cpp \
    functions.cpp \
    imagelistmodel.cpp \
    inventory.cpp \
    imageitem.cpp \
    imageview.cpp \
    mainwindow.cpp \
    qimagegrabber/dialogqimagegrabbersettings.cpp \
    qimagegrabber/qimagegrabber.cpp \
    qimagegrabber/qimagegrabberfile.cpp \
    qimagegrabber/qimagegrabberhttp.cpp \
    qimagegrabber/qimagegrabbericamview.cpp \
    qimagegrabber/qimagegrabbermjpeg.cpp \
    qimagegrabber/qimagegrabberv4l2.cpp \
    imagecapture.cpp \
    robot/json.cpp \
    robot/rosclient.cpp

HEADERS  += bow.h \
    functions.hpp \
    imagelistmodel.h \
    CONV.h \
    inventory.h \
    imageitem.h \
    imageview.h \
    mainwindow.h \
    qimagegrabber/dialogqimagegrabbersettings.h \
    qimagegrabber/qimagegrabber.h \
    qimagegrabber/qimagegrabberfile.h \
    qimagegrabber/qimagegrabber_global.h \
    qimagegrabber/qimagegrabberhttp.h \
    qimagegrabber/qimagegrabbericamview.h \
    qimagegrabber/qimagegrabbermjpeg.h \
    qimagegrabber/qimagegrabberv4l2.h \
    imagecapture.h \
    robot/json.h \
    robot/rosclient.h

FORMS    += bow.ui \
    inventory.ui \
    mainwindow.ui \
    qimagegrabber/dialogqimagegrabbersettings.ui \
    imagecapture.ui

CONFIG  += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

unix|win32: LIBS += -lboost_filesystem -lboost_system

OTHER_FILES += \
    README.md

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS += -Wno-unused-parameter
