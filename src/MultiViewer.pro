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
	jheadhandler.h \
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
	third_party/jhead/jhead.h \
	version.h \
    mainwindow.h \
    progressbardialog.h
	
SOURCES += main.cpp \
	action.cpp \
	actionlistitemwidget.cpp \
	application.cpp \
	consolewidget.cpp \
	exif.cpp \
	jheadhandler.cpp \
	messageboxes.cpp \
	packagemanager.cpp \
	paths.cpp \
	plugin.cpp \
	pluginmanager.cpp \
	preferencesdialog.cpp \
	processutil.cpp \
	settings.cpp \
	stringutil.cpp \
	third_party/jhead/jhead_exif.cpp \
	third_party/jhead/jhead_gpsinfo.cpp \
	third_party/jhead/jhead_iptc.cpp \
	third_party/jhead/jhead_jhead.cpp \
	third_party/jhead/jhead_jpgfile.cpp \
	third_party/jhead/jhead_jpgqguess.cpp \
	third_party/jhead/jhead_makernote.cpp \
	third_party/jhead/jhead_paths.cpp \
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
