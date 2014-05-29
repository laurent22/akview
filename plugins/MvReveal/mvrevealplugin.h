#ifndef MVREVEALPLUGIN_H
#define MVREVEALPLUGIN_H

#include <QObject>
#include <QString>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"

class MvRevealPlugin : public QObject, mv::MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvRevealPlugin" FILE "mvrevealplugin.json")
	Q_INTERFACES(mv::MvPluginInterface)

public:

	void onInitialize(mv::IApplication* application) {
		this->application = application;
	}

	void onAction(const QString& action);

private:

	mv::IApplication* application;
	bool revealFile(const QString& pathIn);

};

#endif
