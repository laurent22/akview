TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    application.cpp \
    pluginmanager.cpp \
    settings.cpp \
    paths.cpp \
    stringutil.cpp \
    version.cpp

RESOURCES += qml.qrc

INCLUDEPATH += "/usr/local/Cellar/freeimage/3.15.4/include"
LIBS += /usr/local/Cellar/freeimage/3.15.4/lib/libfreeimage.dylib

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

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
    simpletypes.h
