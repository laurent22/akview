TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    application.cpp \
    pluginmanager.cpp \
    settings.cpp \
    paths.cpp \
    stringutil.cpp \
	version.cpp \
	jheadhandler.cpp \
    third_party/jhead/jhead_exif.cpp \
    third_party/jhead/jhead_gpsinfo.cpp \
    third_party/jhead/jhead_iptc.cpp \
    third_party/jhead/jhead_jhead.cpp \
    third_party/jhead/jhead_jpgfile.cpp \
    third_party/jhead/jhead_jpgqguess.cpp \
    third_party/jhead/jhead_makernote.cpp \
    third_party/jhead/jhead_paths.cpp \
    preferencesdialog.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mvplugininterface.h \
    application.h \
    iapplication.h \
    pluginmanager.h \
    pluginevents.h \
    settings.h \
    constants.h \
    paths.h \
    stringutil.h \
    version.h \
	simpletypes.h \
    third_party/jhead/jhead.h \
    jheadhandler.h \
    preferencesdialog.h

FORMS += \
    preferencesdialog.ui

OTHER_FILES +=
