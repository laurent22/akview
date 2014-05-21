#include <QtWidgets>
#include <QDebug>

#include "mvbrowserplugin.h"

QString MvBrowserPlugin::description() {
	application->testing();
	return "Browser Plugin";
}
