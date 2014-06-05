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

void PluginManager::onKeypressed(const KeypressedEvent &event) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		Plugin* plugin = plugins_[i];
		Action* action = plugin->findAction(event);
		if (!action) continue;

		if (!plugin->interfaceLoaded()) {
			bool ok = plugin->loadInterface();
			if (!ok) {
				qWarning() << "could not load plugin interface of" << plugin->description() << ":" << plugin->errorMessage();
				continue;
			}
		}

		plugin->interface()->onAction(action->name());
		qDebug() << "Keypress event has been processed by plugin" << plugin->description();
		return;
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

bool Plugin::interfaceLoaded() const {
	return interface_ ? true : false;
}

MvPluginInterface* Plugin::interface() const {
	return interface_;
}

Action::Action(const QJsonObject &jsonObject): QAction(NULL) {
	jsonObject_ = jsonObject;

	name_ = jsonObject_.value("name").toString();
	setText(jsonObject_.value("description").toString());

	QJsonArray array = jsonObject.value("shortcuts").toArray();
	QList<QKeySequence> shortcuts;
	for (int i = 0; i < array.size(); i++) {
		QString s = array[i].toString();
		s = s.replace(" ", "");
		s = s.trimmed();
		QKeySequence ks(s);
		shortcuts.push_back(ks);
	}
	setShortcuts(shortcuts);
}

bool Action::supports(const KeypressedEvent &event) const {
	QKeySequence keySequence(event.modifiers + event.keyCode);
	for (unsigned int i = 0; i < shortcuts().size(); i++) {
		const QKeySequence& ks = shortcuts()[i];
		if (ks.matches(keySequence)) return true;
	}
	return false;
}

QString Action::name() const {
	return name_;
}

ActionListWidgetItem::ActionListWidgetItem(Action *action) {
	shortcutIsOverridden_ = false;
	action_ = action;
	updateDisplay();
}

Action *ActionListWidgetItem::action() const {
	return action_;
}

void ActionListWidgetItem::setShortcut(const QKeySequence& ks) {
	shortcut_ = ks;
}

QKeySequence ActionListWidgetItem::shortcut() const {
	if (shortcut_.isEmpty()) return action()->shortcut();
	return shortcut_;
}

void ActionListWidgetItem::setShortcutIsOverridden(bool v) {
	shortcutIsOverridden_ = v;
	qDebug() << "Setting to " << shortcutIsOverridden_;
}

bool ActionListWidgetItem::shortcutIsOverridden() const {
	return shortcutIsOverridden_;
}

void ActionListWidgetItem::updateDisplay() {
	QFont font(this->font());
	font.setBold(shortcutIsOverridden());
	setFont(font);
	setText(shortcut().toString() + " - " + action()->text());

	qDebug() << text() << shortcutIsOverridden();
}

}
