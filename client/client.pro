#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T23:25:49
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h

FORMS    += mainwindow.ui

LIBS += -lgmp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../algo/release/ -lalgo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../algo/debug/ -lalgo
else:unix: LIBS += -L$$OUT_PWD/../algo/ -lalgo

INCLUDEPATH += $$PWD/../algo
DEPENDPATH += $$PWD/../algo

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../algo/release/libalgo.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../algo/debug/libalgo.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../algo/release/algo.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../algo/debug/algo.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../algo/libalgo.a
