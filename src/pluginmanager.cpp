#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QPluginLoader>

#include "pluginmanager.h"
#include "stringutil.h"
#include "version.h"

PluginManager::PluginManager(IApplication *application) {
	application_ = application;
}

bool PluginManager::loadPlugin(const QString& filePath) {
	QPluginLoader pluginLoader(filePath);

	QObject *plugin = pluginLoader.instance();
	if (!plugin) return false;

	QJsonObject metadata = pluginLoader.metaData().value("MetaData").toObject();
	qDebug() << "Loading plugin" << metadata.value("description").toString() << metadata.value("version").toString() << "from" << filePath;
	QString minVersion = metadata.value("compatibility_min_version").toString();
	QString maxVersion = metadata.value("compatibility_max_version").toString();
	QString engineVersion = version::number();

	if (minVersion == "") {
		qWarning() << "No compatibility_min_version metadata specified";
		return false;
	}

	if (maxVersion == "") {
		maxVersion = engineVersion;
	}

	if (stringutil::compareVersions(engineVersion, minVersion) < 0 || stringutil::compareVersions(engineVersion, maxVersion) > 0) {
		qWarning() << "Plugin is not compatible with version" << engineVersion;
		return false;
	}

	MvPluginInterface* mvPlugin;
	mvPlugin = qobject_cast<MvPluginInterface *>(plugin);
	mvPlugin->onInitialize(application_);

	Plugin* info = new Plugin();
	info->description = metadata.value("description").toString();
	info->version = metadata.value("version").toString();
	info->interface = mvPlugin;

	plugins_.push_back(info);
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
			qDebug() << "Successfully loaded plugin" << file.absoluteFilePath();
		}
	}
}

void PluginManager::onKeypressed(const KeypressedEvent &event) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		KeypressedEvent e = event;
		Plugin* plugin = plugins_[i];
		plugin->interface->onKeypressed(e);
		if (e.accepted) {
			qDebug() << "Keypress event has been accepted by plugin" << plugin->description;
			return;
		}
	}
}
