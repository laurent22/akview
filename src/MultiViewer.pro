TEMPLATE = app

QT += widgets

HEADERS += \
	action.h \
	actionlistitemwidget.h \
	application.h \
	consolewidget.h \
	constants.h \
	exif.h \
	iapplication.h \
	messageboxes.h \
	mvplugininterface.h \
	packagemanager.h \
	paths.h \
	plugin.h \
	pluginevents.h \
	pluginmanager.h \
	preferencesdialog.h \
	processutil.h \
	settings.h \
	simplefunctions.h \
	simpletypes.h \
	stringutil.h \
	version.h \
    mainwindow.h \
    progressbardialog.h
	
SOURCES += main.cpp \
	action.cpp \
	actionlistitemwidget.cpp \
	application.cpp \
	consolewidget.cpp \
	exif.cpp \
	messageboxes.cpp \
	packagemanager.cpp \
	paths.cpp \
	plugin.cpp \
	pluginmanager.cpp \
	preferencesdialog.cpp \
	processutil.cpp \
	settings.cpp \
	stringutil.cpp \
	version.cpp \
    mainwindow.cpp \
    progressbardialog.cpp

RESOURCES += resources.qrc

INCLUDEPATH += "/usr/local/Cellar/freeimage/3.15.4/include"
LIBS += /usr/local/Cellar/freeimage/3.15.4/lib/libfreeimage.dylib

FORMS += \
	preferencesdialog.ui \
    mainwindow.ui \
    progressbardialog.ui
