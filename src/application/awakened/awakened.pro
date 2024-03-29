######################################################################
# Automatically generated by qmake (3.0) ?? 4? 23 10:30:27 2019
######################################################################

TEMPLATE = app
TARGET = AwakenedWorld
DESTDIR = $$(BIN)

unix:MOC_DIR=.tmp
unix:OBJECTS_DIR=.tmp
unix:UI_DIR=.tmp

QT += core gui widgets

INCLUDEPATH +=  $(X264INCLUDE)
LIBS += -L$(X264LIB) -lx264
LIBS += -L$(OPENCV4LIB) -lopencv_core -lopencv_highgui

INCLUDEPATH += $$(SRCLIB)/common/  $$(SRCLIB)/filemanager/ $$(SRCLIB)/playertype
LIBS += -L$(LIB) -lcommon -lfilemanager -lplayertype

INCLUDEPATH += ../qtplayer $$(SRCLIB)/widget/ ../musicplayer ../vediomonitor ../rtmplive
INCLUDEPATH += ../desktopcapture ../pictureprocessor
LIBS += -L$(APPLIBRARY) -lqtplayer -lmusicplayer -lvediomonitor -lrtmplive
LIBS += -L$(APPLIBRARY) -ldesktopcapture -lpictureprocessor


# Input
HEADERS += mainwidget.h \
           appdisplayshelf.h \
           appwidget.h \
           apptitlewidget.h \

SOURCES += main.cpp \
           mainwidget.cpp \
           appdisplayshelf.cpp \
           appwidget.cpp \
           apptitlewidget.cpp \
