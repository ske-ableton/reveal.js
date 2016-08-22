
TEMPLATE = app
TARGET = selection_info

QT += quickwidgets

CONFIG += c++11

DEFINES += "SRC_DIR=\\\"$$PWD\\\"" QT_NO_SIGNALS_SLOTS_KEYWORDS

INCLUDEPATH += lib

# Input
SOURCES += main.cpp \
           lib/selectioninfo.cpp \
           mymodel.cpp \

HEADERS += lib/selectioninfo.h \
           mymodel.h \
