#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T23:17:45
#
#-------------------------------------------------

QT       -= gui

TARGET = algo
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

SOURCES += \
    rsa/rsa.cpp \
    rsa/rsakey.cpp \
    rsa/rsakeygenerator.cpp \
    twofish/twofish.cpp

HEADERS += \
    rsa/rsa.h \
    rsa/rsakey.h \
    rsa/rsakeygenerator.h \
    twofish/twofish.h \
    bytearray.h

LIBS += -lgmp -lboost_random

unix {
    target.path = /usr/lib
    INSTALLS += target
}
