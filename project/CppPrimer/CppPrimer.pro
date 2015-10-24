TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    Chapter1/SalesItem.cpp \
    Chapter2/Test.cpp \
    Chapter7/SalesData.cpp \
    Chapter7/Screen.cpp \
    Chapter7/InClassInitVa.cpp \
    Chapter12/SourceManager.cpp \
    Chapter12/Message.cpp \
    Chapter12/Folder.cpp \
    Chapter12/StrVector.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Chapter1/SalesItem.h \
    Chapter2/Test.h \
    Chapter7/SalesData.h \
    Chapter7/Screen.h \
    Chapter7/InClassInitVa.h \
    Chapter12/SourceManager.h \
    Chapter12/Message.h \
    Chapter12/Folder.h \
    Chapter12/StrVector.h

