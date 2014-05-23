TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvdeletefileplugin.h \
	../../src/mvplugininterface.h \
	../../src/iapplication.h
SOURCES         = \
	mvdeletefileplugin.cpp

TARGET          = $$qtLibraryTarget(mvbrowserplugin)

OTHER_FILES += \
	mvdeletefileplugin.json
