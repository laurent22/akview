#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "mvplugininterface.h"
#include "pluginevents.h"

struct Plugin {
	MvPluginInterface* interface;
	QString description;
	QString version;
};

typedef std::vector<Plugin*> PluginVector;

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
