#ifndef MVBROWSERPLUGIN_H
#define MVBROWSERPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QtPlugin>
#include "../../src/mvplugininterface.h"
#include "../../src/iapplication.h"

class MvBrowserPlugin : public QObject, MvPluginInterface {

	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.mv-project.MvPluginInterface" FILE "mvbrowserplugin.json")
	Q_INTERFACES(MvPluginInterface)

public:

	QString description();
	void onInitialize(IApplication* application) {
		this->application = application;
	}

	void onKeypressed(const KeypressedEvent& event);
	QStringList sourceList() const;
	int sourceListIndex() const;
	void setSourceListIndex(int index);
	void nextSource();
	void previousSource();

private:

	IApplication* application;
	mutable QStringList sourceList_;
	mutable int sourceListIndex_;

};

#endif
