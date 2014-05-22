TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvbrowserplugin.h \
	../../src/mvplugininterface.h \
	../../src/iapplication.h
SOURCES         = \
    mvbrowserplugin.cpp

TARGET          = $$qtLibraryTarget(mvbrowserplugin)

OTHER_FILES += \
	mvbrowserplugin.json
