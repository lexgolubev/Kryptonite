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
    twofish/twofish.h

LIBS += -lgmp -lboost_random

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/release/ -lboost_random
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/debug/ -lboost_random
else:unix: LIBS += -L$$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/ -lboost_random

INCLUDEPATH += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi
DEPENDPATH += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/release/libboost_random.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/debug/libboost_random.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/release/boost_random.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/debug/boost_random.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../Downloads/boost_1_55_0/bin.v2/libs/random/build/gcc-4.8/release/link-static/threading-multi/libboost_random.a
