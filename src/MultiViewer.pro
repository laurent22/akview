TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    application.cpp \
    pluginmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mvplugininterface.h \
    application.h \
    iapplication.h \
    pluginmanager.h \
    pluginevents.h
