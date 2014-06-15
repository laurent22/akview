#ifndef MVINFORMATIONPLUGIN_H
#define MVINFORMATIONPLUGIN_H

#include <QObject>
#include <QString>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"

class MvMetadataPlugin : public QObject, mv::MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvMetadataPlugin" FILE "mvmetadataplugin.json")
	Q_INTERFACES(mv::MvPluginInterface)

public:

	void onInitialize(mv::IApplication* application) {
		this->application = application;
	}

	void onAction(const QString& action);

private:

	mv::IApplication* application;

};

#endif
