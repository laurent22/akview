#ifndef MVPLUGININTERFACE_H
#define MVPLUGININTERFACE_H

#include "iapplication.h"
#include <QString>

class MvPluginInterface {

public:

	virtual ~MvPluginInterface() {}
	virtual QString description() { return ""; }
	virtual void onInitialize(IApplication* application) {}
	virtual void onKeypressed(const KeypressedEvent& event) {}

};

QT_BEGIN_NAMESPACE
#define MvPluginInterface_iid "org.mv-project.MvPluginInterface"
Q_DECLARE_INTERFACE(MvPluginInterface, MvPluginInterface_iid)
QT_END_NAMESPACE

#endif
