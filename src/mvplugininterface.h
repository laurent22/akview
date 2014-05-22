#ifndef MVPLUGININTERFACE_H
#define MVPLUGININTERFACE_H

#include "iapplication.h"
#include "pluginevents.h"
#include <QString>

class MvPluginInterface {

public:

	virtual ~MvPluginInterface() {}
	virtual void onInitialize(IApplication* application) { Q_UNUSED(application); }
	virtual void onKeypressed(const KeypressedEvent& event) { Q_UNUSED(event); }

};

QT_BEGIN_NAMESPACE
#define MvPluginInterface_iid "org.mv-project.MvPluginInterface"
Q_DECLARE_INTERFACE(MvPluginInterface, MvPluginInterface_iid)
QT_END_NAMESPACE

#endif
