TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvjpegtoolsplugin.h \
	../../src/mvplugininterface.h \
	../../src/iapplication.h \
    dialog.h
	
SOURCES         = \
	mvjpegtoolsplugin.cpp \
    dialog.cpp
	
TARGET          = $$qtLibraryTarget(mvjpegtoolsplugin)

INCLUDEPATH += "/usr/local/Cellar/freeimage/3.15.4/include"
LIBS += /usr/local/Cellar/freeimage/3.15.4/lib/libfreeimage.dylib

OTHER_FILES += \
	mvjpegtoolsplugin.json

FORMS += \
    dialog.ui
