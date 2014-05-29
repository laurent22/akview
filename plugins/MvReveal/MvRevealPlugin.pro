TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvrevealplugin.h \
	../../src/mvplugininterface.h
	
SOURCES         = \
	mvrevealplugin.cpp
		
TARGET = $$qtLibraryTarget(mvrevealplugin)

OTHER_FILES += \
	mvrevealplugin.json
