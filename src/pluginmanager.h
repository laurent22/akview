#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugin.h"

namespace mv {

class PluginManager {

public:

	PluginManager();
	bool loadPlugin(const QString& folderPath);
	void loadPlugins(const QString& folderPath);
	PluginVector plugins() const;
	void onAction(const QString& actionName);

private:

	PluginVector plugins_;

	QStringList replaceVariables(const QStringList& command);

};

}

#endif // PLUGINMANAGER_H
