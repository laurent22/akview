#include "plugin.h"
#include "stringutil.h"
#include "version.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QPluginLoader>

namespace mv {

Plugin::Plugin(const QString& pluginFolderPath) {
	pluginFolderPath_ = pluginFolderPath;
}

bool Plugin::load() {
	bool ok = loadManifest();
	if (!ok) return false;
	return loadActions();
}

bool Plugin::loadManifest() {
	QString filePath = pluginFolderPath_ + "/manifest.json";

	errorMessage_ = "";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		errorMessage_ = QString("cannot open manifest file at \"%1\"").arg(filePath);
		return false;
	}

	QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
	if (doc.isNull()) {
		errorMessage_ = QString("JSON document at \"%1\" is not valid").arg(filePath);
		return false;
	}

	manifest_ = doc.object();

	QString engineVersion = version::number();
	QString minVersion = minEngineVersion();

	if (minVersion != "") {
		if (stringutil::compareVersions(engineVersion, minVersion) < 0) {
			errorMessage_ = QString("plugin is not compatible with version %1").arg(engineVersion);
			return false;
		}
	}

	return true;
}

bool Plugin::loadActions() {
	QDir dir(pluginFolderPath_ + "/actions");
	if (!dir.exists()) {
		errorMessage_ = "plugin does not have an 'actions' folder";
		return false;
	}

	QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo fileInfo = files[i];
		if (fileInfo.suffix().toLower() != "json") continue; 

		QFile file(fileInfo.absoluteFilePath());

		if (!file.open(QIODevice::ReadOnly)) {
			errorMessage_ = QString("cannot open action file at \"%1\"").arg(fileInfo.absoluteFilePath());
			continue;
		}

		QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
		if (doc.isNull()) {
			errorMessage_ = QString("JSON document at \"%1\" is not valid").arg(fileInfo.absoluteFilePath());
			return false;
		}

		Action* action = new Action(fileInfo.baseName(), doc.object());
		actions_.push_back(action);
	}

	return true;
}

QString Plugin::errorMessage() const {
	return errorMessage_;
}

QString Plugin::description() const {
	return manifest_.value("description").toString();
}

QString Plugin::version() const {
	return manifest_.value("version").toString();
}

QString Plugin::minEngineVersion() const {
	return manifest_.value("min_engine_version").toString();
}

ActionVector Plugin::actions() const {
	return actions_;
}

Action* Plugin::findAction(const QString& name) const {
	for (unsigned int i = 0; i < actions_.size(); i++) {
		Action* a = actions_[i];
		if (a->name() == name) return a;
	}
	return NULL;
}

}