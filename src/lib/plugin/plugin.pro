######################################################################
# Automatically generated by qmake (3.0) ?? 4? 22 19:37:53 2019
######################################################################

TEMPLATE = lib
TARGET = signalplugin
DESTDIR = $$(PLUGINS)

#QT += gui
QT += core gui widgets
CONFIG += plugin

unix:MOC_DIR=.tmp
unix:OBJECTS_DIR=.tmp
unix:UI_DIR=.tmp

EXAMPLE_FILES = SignalPlugin.json

#INCLUDEPATH += $(FFMPEGINCLUDE) $(SDLINCLUDE)

#LIBS += -L$(FFMPEGLIB)
#LIBS += -L$(SDLLIB)

# Input
HEADERS += plugininterface.h \
            signalplugin.h \


SOURCES += signalplugin.cpp \
