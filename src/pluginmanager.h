#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugin.h"
#include "pluginevents.h"

namespace mv {

class PluginManager {

public:

	PluginManager(IApplication* application);
	bool loadPlugin(const QString& filePath);
	void loadPlugins(const QString& folderPath);
	PluginVector plugins() const;
	void onKeypressed(const KeypressedEvent& event);
	void onAction(const QString& actionName);

private:

	PluginVector plugins_;
	IApplication* application_;

};

}

#endif // PLUGINMANAGER_H
