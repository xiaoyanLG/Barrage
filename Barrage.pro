#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T16:22:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Barrage
TEMPLATE = app

INCLUDEPATH += qtsingle/ DouYuDanMu/

SOURCES += main.cpp\
        mainwindow.cpp \
    qtsingle/qtlocalpeer.cpp \
    qtsingle/qtlockedfile.cpp \
    qtsingle/qtlockedfile_win.cpp \
    qtsingle/qtsingleapplication.cpp \
    qtsingle/qtsinglecoreapplication.cpp \
    qtsingle/qtlockedfile_unix.cpp \
    xybordershadowwidget.cpp \
    xymenu.cpp \
    xymenustyle.cpp \
    xytooltips.cpp \
    xymousemonitor.cpp \
    xymovepath.cpp \
    xysignalbarragescreen.cpp \
    xybarrageitem.cpp \
    xybarragescreen.cpp \
    xydrawwidget.cpp \
    xyemoticonwidget.cpp \
    xyexpressionlabel.cpp \
    xysystemtray.cpp \
    xyfullscreenshots.cpp \
    xytextboard.cpp \
    xymovablewidget.cpp \
    DouYuDanMu/douyutcpsocket.cpp \
    DouYuDanMu/jsonparse.cpp \
    DouYuDanMu/networkaccess.cpp \
    DouYuDanMu/stringgenerator.cpp

HEADERS  += mainwindow.h \
    qtsingle/qtlocalpeer.h \
    qtsingle/qtlockedfile.h \
    qtsingle/qtsingleapplication.h \
    qtsingle/qtsinglecoreapplication.h \
    xybordershadowwidget.h \
    xymenu.h \
    xymenustyle.h \
    xytooltips.h \
    xymousemonitor.h \
    xymovepath.h \
    xysignalbarragescreen.h \
    xybarrageitem.h \
    xybarragescreen.h \
    xydrawwidget.h \
    xyemoticonwidget.h \
    xyexpressionlabel.h \
    xysystemtray.h \
    xyfullscreenshots.h \
    xytextboard.h \
    xymovablewidget.h \
    DouYuDanMu/danmuconfig.h \
    DouYuDanMu/douyutcpsocket.h \
    DouYuDanMu/jsonparse.h \
    DouYuDanMu/networkaccess.h \
    DouYuDanMu/stringgenerator.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    images/images.qrc

RC_FILE = ico.rc

LIBS += -lws2_32 -ladvapi32 -lkernel32 -luser32
