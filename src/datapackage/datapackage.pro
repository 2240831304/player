######################################################################
# Automatically generated by qmake (3.0) ?? 4? 29 17:23:24 2019
######################################################################

TEMPLATE = lib
TARGET = datapackage
DESTDIR = $$(LIB)

QT       += core gui
unix:MOC_DIR=.tmp
unix:OBJECTS_DIR=.tmp
unix:UI_DIR=.tmp

INCLUDEPATH += $(FFMPEGINCLUDE) $(SDLINCLUDE) $$(SRCLIB)/common/
INCLUDEPATH += $$(SRCLIB)/plugin/

LIBS += -L$(ZLIB) -lz -llzma
LIBS += -L$(FFMPEGLIB) -lavcodec -lavdevice -lavfilter -lavformat
LIBS += -L$(FFMPEGLIB) -lavutil -lswresample -lswscale
LIBS += -L$(SDLLIB) -lSDL2 -lSDL2main -lSDL2_test -lSDL2_mixer
LIBS += -L$(LIB) -lcommon


# Input
HEADERS +=  mp4package.h \
            rtmpplugflow.h \


SOURCES +=  mp4package.cpp \
            rtmpplugflow.cpp \
