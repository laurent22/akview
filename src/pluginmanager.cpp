#include <QDebug>
#include <QDir>

#include <QPluginLoader>
#include "pluginmanager.h"

PluginManager::PluginManager(IApplication *application) {
	application_ = application;
}

bool PluginManager::loadPlugin(const QString& filePath) {
	QPluginLoader pluginLoader(filePath);

	QObject *plugin = pluginLoader.instance();
	if (!plugin) return false;

	MvPluginInterface* mvPlugin;
	mvPlugin = qobject_cast<MvPluginInterface *>(plugin);
	mvPlugin->onInitialize(application_);
	plugins_.push_back(mvPlugin);
	return true;
}

void PluginManager::loadPlugins(const QString &folderPath) {
	QDir dir(folderPath);
	QFileInfoList files = dir.entryInfoList(QDir::Files);
	for (unsigned int i = 0; i < files.size(); i++) {
		QFileInfo file = files[i];
		if (file.suffix() != "dylib") continue;
		bool loaded = loadPlugin(file.absoluteFilePath());
		if (!loaded) {
			qWarning() << "Could not load plugin" << file.absoluteFilePath();
		} else {
			qDebug() << "Successfully loaded plugin" << file.absoluteFilePath();
		}
	}
}

void PluginManager::onKeypressed(const KeypressedEvent &event) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		MvPluginInterface* plugin = plugins_[i];
		plugin->onKeypressed(event);
	}
}
