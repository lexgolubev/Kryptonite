######################################################################
# Automatically generated by qmake (3.0) Sun Nov 16 10:20:57 2014
######################################################################

QT += gui core testlib widgets

CONFIG += c++11
TEMPLATE = app
TARGET = Kryptonite
INCLUDEPATH += .

# Input
HEADERS += algo/rsa/rsa.h \
           algo/rsa/rsakey.h \
           algo/rsa/rsakeygenerator.h \
           algo/twofish/twofish.h \
           main/mainwindow.h

FORMS += main/mainwindow.ui

SOURCES += algo/rsa/rsa.cpp \
           algo/rsa/rsakey.cpp \
           algo/rsa/rsakeygenerator.cpp \
           algo/twofish/twofish.cpp \
           main/main.cpp \
           main/mainwindow.cpp \
           test/testtwofish.cpp

LIBS += -lgmp -lboost_random
