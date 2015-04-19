#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T13:37:59
#
#-------------------------------------------------

QT       += core gui widgets network opengl websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = visin #mainApplication
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
    robot/rosclient.cpp \
    welcome.cpp \
    grid/display.cpp \
    grid/item_details.cpp \
    grid/items_details.cpp \
    grid/qrightclickpushbutton.cpp \
    global_objects.cpp \
    robot/crop.cpp \
    ResultsGrid/display2.cpp \
    ResultsGrid/selectgrid.cpp


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
    robot/rosclient.h \
    welcome.h \
    grid/display.h \
    grid/item_details.h \
    grid/items_details.h \
    grid/qrightclickpushbutton.h \
    global_objects.h \
    robot/crop.h \
    ResultsGrid/display2.h \
    ResultsGrid/selectgrid.h


FORMS    += bow.ui \
    inventory.ui \
    mainwindow.ui \
    qimagegrabber/dialogqimagegrabbersettings.ui \
    imagecapture.ui \
    welcome.ui \
    grid/display.ui \
    grid/item_details.ui \
    grid/items_details.ui \
    ResultsGrid/display2.ui \
    ResultsGrid/selectgrid.ui


CONFIG  += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

unix|win32: LIBS += -lboost_filesystem -lboost_system -fopenmp

OTHER_FILES += \
    README.md

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS += -Wno-unused-parameter -fopenmp -Wno-sign-compare
