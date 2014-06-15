TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvmetadataplugin.h \
	../../src/mvplugininterface.h
	
SOURCES         = \
	mvmetadataplugin.cpp
		
TARGET = $$qtLibraryTarget(mvmetadataplugin)

OTHER_FILES += \
	mvmetadataplugin.json
