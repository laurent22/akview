TEMPLATE = app

QT += widgets script

CONFIG += precompile_header

PRECOMPILED_HEADER = stable.h

CONFIG(debug, debug|release):TARGET = MultiViewer-debug
CONFIG(release, debug|release):TARGET = MultiViewer

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
	scriptutil.h \
	settings.h \
	simplefunctions.h \
	simpletypes.h \
	stringutil.h \
	version.h \
    mainwindow.h \
    progressbardialog.h \
    jsapi/jsapi_application.h \
    jsapi/jsapi_console.h \
    jsapi/jsapi_fileinfo.h \
    jsapi/jsapi_input.h \
    jsapi/jsapi_plugin.h \
    jsapi/jsapi_ui.h \
    jsapi/jsapi_system.h
	
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
	scriptutil.cpp \
	stringutil.cpp \
	version.cpp \
    mainwindow.cpp \
    progressbardialog.cpp \
    jsapi/jsapi_application.cpp \
    jsapi/jsapi_console.cpp \
    jsapi/jsapi_fileinfo.cpp \
    jsapi/jsapi_input.cpp \
    jsapi/jsapi_plugin.cpp \
    jsapi/jsapi_ui.cpp \
    jsapi/jsapi_system.cpp

RESOURCES += resources.qrc

INCLUDEPATH += "/usr/local/Cellar/freeimage/3.15.4/include"
LIBS += /usr/local/Cellar/freeimage/3.15.4/lib/libfreeimage.dylib

FORMS += \
	preferencesdialog.ui \
    mainwindow.ui \
    progressbardialog.ui
