#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "mvplugininterface.h"
#include "pluginevents.h"

typedef std::vector<MvPluginInterface*> PluginVector;

class PluginManager {

public:

	PluginManager(IApplication* application);
	bool loadPlugin(const QString& filePath);
	void loadPlugins(const QString& folderPath);

	void onKeypressed(const KeypressedEvent& event);

private:

	PluginVector plugins_;
	IApplication* application_;

};

#endif // PLUGINMANAGER_H
