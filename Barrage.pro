#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T16:22:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Barrage
TEMPLATE = app

INCLUDEPATH += qtsingle/

SOURCES += main.cpp\
        mainwindow.cpp \
    cbarrageitem.cpp \
    cbarragescreen.cpp \
    systemtray.cpp \
    cdrawwidget.cpp \
    cemoticonwidget.cpp \
    qexpressionlabel.cpp \
    cmousemonitorwidget.cpp \
    csignalbarragescreen.cpp \
    cmovepath.cpp \
    qtsingle/qtlocalpeer.cpp \
    qtsingle/qtlockedfile.cpp \
    qtsingle/qtlockedfile_win.cpp \
    qtsingle/qtsingleapplication.cpp \
    qtsingle/qtsinglecoreapplication.cpp \
    qtsingle/qtlockedfile_unix.cpp \
    xyaction.cpp \
    xybordershadowwidget.cpp \
    xymenu.cpp \
    xymenustyle.cpp

HEADERS  += mainwindow.h \
    cbarrageitem.h \
    cbarragescreen.h \
    systemtray.h \
    cdrawwidget.h \
    cemoticonwidget.h \
    qexpressionlabel.h \
    cmousemonitorwidget.h \
    csignalbarragescreen.h \
    cmovepath.h \
    qtsingle/qtlocalpeer.h \
    qtsingle/qtlockedfile.h \
    qtsingle/qtsingleapplication.h \
    qtsingle/qtsinglecoreapplication.h \
    xyaction.h \
    xybordershadowwidget.h \
    xymenu.h \
    xymenustyle.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    images/images.qrc

RC_FILE = ico.rc

LIBS += -lws2_32 -ladvapi32 -lkernel32 -luser32
