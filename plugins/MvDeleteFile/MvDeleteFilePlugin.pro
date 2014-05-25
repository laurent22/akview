TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
HEADERS         = \
	mvdeletefileplugin.h \
	../../src/mvplugininterface.h \
	../../src/iapplication.h \
    qxtrash.h
SOURCES         = \
	mvdeletefileplugin.cpp

win32 {
	SOURCES += qxtrash_win.cpp
}

mac {
	OBJECTIVE_SOURCES += qxtrash_osx.mm
	LIBS += -framework AppKit -framework Foundation
}

TARGET = $$qtLibraryTarget(mvdeletefileplugin)

OTHER_FILES += \
	mvdeletefileplugin.json
