TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvdeletefileplugin.h \
	../../src/mvplugininterface.h \
	../../src/iapplication.h \
    qxtrash.h
SOURCES         = \
	mvdeletefileplugin.cpp \
    qxtrash.cpp

TARGET          = $$qtLibraryTarget(mvdeletefileplugin)

OTHER_FILES += \
	mvdeletefileplugin.json
