#include "plugin.h"
#include "stringutil.h"
#include "version.h"

#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QPluginLoader>

namespace mv {

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
		Action* action = new Action(o);
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

ActionVector Plugin::actions() const {
	return actions_;
}

bool Plugin::supports(const KeypressedEvent &event) const {
	Action* a = findAction(event);
	return a ? true : false;
}

Action* Plugin::findAction(const KeypressedEvent &event) const {
	for (unsigned int i = 0; i < actions_.size(); i++) {
		Action* a = actions_[i];
		if (a->supports(event)) return a;
	}
	return NULL;
}

Action* Plugin::findAction(const QString& name) const {
	for (unsigned int i = 0; i < actions_.size(); i++) {
		Action* a = actions_[i];
		if (a->name() == name) return a;
	}
	return NULL;
}

bool Plugin::interfaceLoaded() const {
	return interface_ ? true : false;
}

MvPluginInterface* Plugin::interface() const {
	return interface_;
}

}