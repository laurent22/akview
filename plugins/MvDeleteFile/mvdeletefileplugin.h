#ifndef MVDELETEFILEPLUGIN_H
#define MVDELETEFILEPLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"
#include "../../src/iapplication.h"

class MvDeleteFilePlugin : public QObject, mv::MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvPluginInterface" FILE "mvdeletefileplugin.json")
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
