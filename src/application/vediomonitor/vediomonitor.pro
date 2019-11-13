######################################################################
# Automatically generated by qmake (3.0) ?? 4? 23 10:30:27 2019
######################################################################

TEMPLATE = lib
TARGET = vediomonitor
DESTDIR = $$(APPLIBRARY)

unix:MOC_DIR=.tmp
unix:OBJECTS_DIR=.tmp
unix:UI_DIR=.tmp

QT += core gui widgets

INCLUDEPATH += $(FFMPEGINCLUDE) $(SDLINCLUDE)

LIBS += -L$(FFMPEGLIB) -lavcodec -lavdevice -lavfilter -lavformat
LIBS += -L$(SDLLIB) -lSDL2 -lSDL2main -lSDL2_test

INCLUDEPATH += $$(SRCLIB)/common/  $$(SRCLIB)/filemanager/
INCLUDEPATH += $$(SRCLIB)/widget/  ../../vediorecorde
LIBS += -L$(LIB) -lcommon -lfilemanager -lwidget -lvediorecorde


# Input
HEADERS +=  vediomonitorplaywidget.h \
            vediomonitorwidget.h \
            vediomonitorthread.h \


SOURCES +=  vediomonitorplaywidget.cpp \
            vediomonitorwidget.cpp \
            vediomonitorthread.cpp \