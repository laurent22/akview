#include <QDebug>
#include <QDir>
#include <QPluginLoader>

#include "action.h"
#include "pluginmanager.h"

namespace mv {

PluginManager::PluginManager(IApplication *application) {
	application_ = application;
}

bool PluginManager::loadPlugin(const QString& filePath) {
	Plugin* plugin = new Plugin(application_, filePath);
	bool ok = plugin->loadMetadata();
	if (!ok) {
		qWarning() << "could not load plugin" << filePath << ":" << plugin->errorMessage();
		return false;
	}

	plugins_.push_back(plugin);
	return true;
}

void PluginManager::loadPlugins(const QString &folderPath) {
	qDebug() << "Loading plugins from" << folderPath;

	QDir dir(folderPath);
	QFileInfoList files = dir.entryInfoList(QDir::Files);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files[i];
		if (file.suffix() != "dylib") continue;
		bool loaded = loadPlugin(file.absoluteFilePath());
		if (!loaded) {
			qWarning() << "Could not load plugin" << file.absoluteFilePath();
		} else {
			qDebug() << "Successfully preloaded plugin" << file.absoluteFilePath();
		}
	}
}

PluginVector PluginManager::plugins() const {
	return plugins_;
}

void PluginManager::onAction(const QString& actionName) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		Plugin* plugin = plugins_[i];
		Action* action = plugin->findAction(actionName);
		if (!action) continue;

		if (!plugin->interfaceLoaded()) {
			bool ok = plugin->loadInterface();
			if (!ok) {
				qWarning() << "could not load plugin interface of" << plugin->description() << ":" << plugin->errorMessage();
				continue;
			}
		}

		plugin->interface()->onAction(action->name());
		qDebug() << "Action" << actionName << "has been processed by plugin" << plugin->description();
		return;
	}
}

}
