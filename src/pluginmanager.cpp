#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPluginLoader>

#include "pluginmanager.h"
#include "stringutil.h"
#include "version.h"

namespace mv {

PluginManager::PluginManager(IApplication *application) {
	application_ = application;
}

bool PluginManager::loadPlugin(const QString& filePath) {
	Plugin plugin(application_, filePath);
	bool ok = plugin.loadMetadata();
	if (!ok) {
		qWarning() << "could not load plugin" << filePath << ":" << plugin.errorMessage();
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

void PluginManager::onKeypressed(const KeypressedEvent &event) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		Plugin& plugin = plugins_[i];
		if (!plugin.supports(event)) continue;

		if (!plugin.interfaceLoaded()) {
			bool ok = plugin.loadInterface();
			if (!ok) {
				qWarning() << "could not load plugin interface of" << plugin.description() << ":" << plugin.errorMessage();
				continue;
			}
		}

		KeypressedEvent e = event;
		plugin.interface()->onKeypressed(e);
		if (e.accepted) {
			qDebug() << "Keypress event has been accepted by plugin" << plugin.description();
			return;
		}
	}
}

QString Plugin::metadataFilePath(const QString &pluginFilePath) const {
	QFileInfo fileInfo = QFileInfo(pluginFilePath);
	QString basename = fileInfo.baseName();
	if (basename.indexOf("lib") == 0) {
		basename = basename.mid(3);
	}
	if (basename.right(6) == "_debug") {
		basename = basename.left(basename.length() - 6);
	}
	return fileInfo.absolutePath() + "/" + basename + ".json";
}

Plugin::Plugin(IApplication* application, const QString& pluginFilePath) {
	application_ = application;
	pluginFilePath_ = pluginFilePath;
	interface_ = NULL;
}

bool Plugin::loadMetadata() {
	QString filePath = metadataFilePath(pluginFilePath_);

	errorMessage_ = "";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		errorMessage_ = QString("cannot open metadata file at \"%1\"").arg(filePath);
		return false;
	}

	QByteArray rawData = file.readAll();
	QJsonDocument doc(QJsonDocument::fromJson(rawData));
	if (doc.isNull()) {
		errorMessage_ = QString("JSON document at \"%1\" is not valid").arg(filePath);
		return false;
	}

	metadata_ = doc.object();

	QJsonArray actionObjects = metadata_.value("actions").toArray();
	for (int i = 0; i < actionObjects.size(); i++) {
		QJsonObject o = actionObjects[i].toObject();
		PluginAction action(o);
		actions_.push_back(action);
	}

	return true;
}

bool Plugin::loadInterface() {
	QPluginLoader pluginLoader(pluginFilePath_);

	errorMessage_ = "";

	QObject *plugin = pluginLoader.instance();
	if (!plugin) {
		errorMessage_ = "cannot load plugin";
		return false;
	}

	qDebug() << "Loading plugin interface" << description() << version() << "from" << pluginFilePath_;
	QString engineVersion = version::number();
	QString minVersion = compatibilityMinVersion();
	QString maxVersion = compatibilityMaxVersion();

	if (minVersion == "") {
		errorMessage_ = "required compatibility_min_version metadata is missing";
		return false;
	}

	if (maxVersion == "") {
		maxVersion = engineVersion;
	}

	if (stringutil::compareVersions(engineVersion, minVersion) < 0 || stringutil::compareVersions(engineVersion, maxVersion) > 0) {
		errorMessage_ = QString("plugin is not compatible with version %1").arg(engineVersion);
		return false;
	}

	interface_ = qobject_cast<MvPluginInterface*>(plugin);
	interface_->onInitialize(application_);

	return true;
}

QString Plugin::errorMessage() const {
	return errorMessage_;
}

QString Plugin::description() const {
	return metadata_.value("description").toString();
}

QString Plugin::version() const {
	return metadata_.value("version").toString();
}

QString Plugin::compatibilityMinVersion() const {
	return metadata_.value("compatibility_min_version").toString();
}

QString Plugin::compatibilityMaxVersion() const {
	return metadata_.value("compatibility_max_version").toString();
}

QString Plugin::toString() const {
	QString s;
	s += "Description: " + description() + "\n";
	s += "Version: " + version() + "\n";
	s += QString("Interface: %1\n").arg(interface_ ? "loaded" : "not loaded");
	s += "Actions:\n";
	for (unsigned int i = 0; i < actions_.size(); i++) {
		s += actions_[i].toString();
	}
	s += "\n";
	return s;
}

bool Plugin::supports(const KeypressedEvent &event) const {
	for (unsigned int i = 0; i < actions_.size(); i++) {
		PluginAction a = actions_[i];
		if (a.supports(event)) return true;
	}
	return false;
}

bool Plugin::interfaceLoaded() const {
	return interface_ ? true : false;
}

MvPluginInterface* Plugin::interface() const {
	return interface_;
}

PluginAction::PluginAction(const QJsonObject &jsonObject) {
	jsonObject_ = jsonObject;
	QJsonArray array = jsonObject.value("shortcuts").toArray();
	for (int i = 0; i < array.size(); i++) {
		QString s = array[i].toString();
		s = s.replace(" ", "");
		s = s.trimmed();
		QKeySequence ks(s);
		shortcuts_.push_back(ks);
	}
}

QString PluginAction::name() const {
	return jsonObject_.value("name").toString();
}

QString PluginAction::description() const {
	return jsonObject_.value("description").toString();
}

QKeySequenceVector PluginAction::shortcuts() const {
	return shortcuts_;
}

QString PluginAction::toString() const {
	QString s;
	s += "Name: " + name() + "\n";
	s += "Description: " + description() + "\n";
	s += "Shorcuts: ";
	QString temp;
	for (unsigned int i = 0; i < shortcuts_.size(); i++) {
		if (temp != "") temp += "; ";
		temp += shortcuts_[i].toString();
	}
	s += temp + "\n";
	return s;
}

bool PluginAction::supports(const KeypressedEvent &event) const {
	QKeySequence keySequence(event.modifiers + event.keyCode);
	for (unsigned int i = 0; i < shortcuts_.size(); i++) {
		const QKeySequence& ks = shortcuts_[i];
		if (ks.matches(keySequence)) return true;
	}
	return false;
}

}
