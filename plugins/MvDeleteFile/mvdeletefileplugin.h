#ifndef MVDELETEFILEPLUGIN_H
#define MVDELETEFILEPLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"
#include "../../src/iapplication.h"

class MvDeleteFilePlugin : public QObject, MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvPluginInterface" FILE "mvdeletefileplugin.json")
	Q_INTERFACES(MvPluginInterface)

public:

	void onInitialize(IApplication* application) {
		this->application = application;
	}

	void onKeypressed(KeypressedEvent &event);

private:

	IApplication* application;

};

#endif
