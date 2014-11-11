#-------------------------------------------------
#
# Project created by QtCreator 2014-11-02T10:19:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kryptonite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rsakeygenerator.cpp \
    rsakey.cpp \
    rsa.cpp \
    twofish.cpp

HEADERS  += mainwindow.h \
    rsakeygenerator.h \
    rsakey.h \
    rsa.h \
    twofish.h

FORMS    += mainwindow.ui

CONFIG += c++11

LIBS += -lgmp -lboost_random
