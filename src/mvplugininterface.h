#ifndef MVPLUGININTERFACE_H
#define MVPLUGININTERFACE_H

#include "iapplication.h"
#include "pluginevents.h"
#include <QString>

namespace mv {

class MvPluginInterface {

public:

	virtual ~MvPluginInterface() {}
	virtual void onInitialize(IApplication* application) { Q_UNUSED(application); }
	virtual void onAction(const QString& action) { Q_UNUSED(action); }

};

}

QT_BEGIN_NAMESPACE
#define MvPluginInterface_iid "org.mv-project.MvPluginInterface"
Q_DECLARE_INTERFACE(mv::MvPluginInterface, MvPluginInterface_iid)
QT_END_NAMESPACE

#endif
