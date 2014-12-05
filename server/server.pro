TEMPLATE = app
QT += core network
CONFIG += console c++11

SOURCES += main.cpp \
    server.cpp \
    clientinfo.cpp \
    connection.cpp

HEADERS += \
    server.h \
    clientinfo.h \
    connection.h


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

unix|win32: LIBS += -lgmp
