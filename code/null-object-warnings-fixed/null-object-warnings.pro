
TEMPLATE = app
TARGET = null-object-warnings

DEFINES += "SRC_DIR=\\\"$$PWD\\\""

QT += qml

CONFIG += c++11

# Input
HEADERS += QtMeterList.h NonQtMeter.h NonQtEntryPoint.h QtEntryPoint.h QtMeter.h
SOURCES += QtMeterList.cpp NonQtMeter.cpp NonQtEntryPoint.cpp QtEntryPoint.cpp main.cpp QtMeter.cpp

OTHER_FILES += main.qml ValueDisplayMeter.qml ValueDisplayText.qml README.md
