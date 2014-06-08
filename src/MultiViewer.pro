TEMPLATE = app

QT += widgets

HEADERS += \
	action.h \
	actionlistitemwidget.h \
	application.h \
	constants.h \
	iapplication.h \
	jheadhandler.h \
	mvplugininterface.h \
	paths.h \
	plugin.h \
	pluginevents.h \
	pluginmanager.h \
	preferencesdialog.h \
	settings.h \
	simpletypes.h \
	stringutil.h \
	third_party/jhead/jhead.h \
	version.h \
    mainwindow.h
	
SOURCES += main.cpp \
	action.cpp \
	actionlistitemwidget.cpp \
	application.cpp \
	jheadhandler.cpp \
	paths.cpp \
	plugin.cpp \
	pluginmanager.cpp \
	preferencesdialog.cpp \
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
    mainwindow.cpp

RESOURCES += qml.qrc

FORMS += \
	preferencesdialog.ui \
    mainwindow.ui
