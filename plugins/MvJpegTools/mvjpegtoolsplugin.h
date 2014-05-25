#ifndef MVJPEGTOOLSPLUGIN_H
#define MVJPEGTOOLSPLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"
#include "../../src/iapplication.h"

class MvJpegToolsPlugin : public QObject, mv::MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvPluginInterface" FILE "mvjpegtoolsplugin.json")
	Q_INTERFACES(mv::MvPluginInterface)

public:

	void onInitialize(mv::IApplication* application) {
		this->application = application;
	}

	void onKeypressed(mv::KeypressedEvent &event);

private:

	mv::IApplication* application;

};

#endif
