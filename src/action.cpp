#include "action.h"

namespace mv {

Action::Action(): QAction(NULL) {}

Action::Action(const QJsonObject &jsonObject): QAction(NULL) {
	jsonObject_ = jsonObject;

	id_ = jsonObject_.value("id").toString();
	setText(jsonObject_.value("title").toString());
	menu_ = jsonObject_.value("menu").toString();
	description_ = jsonObject_.value("description").toString();

	QJsonValue v = jsonObject_.value("show_console");
	showConsole_ = v.isUndefined() ? false : v.toBool();

	QJsonArray dependenciesArray = perOsValue(jsonObject, "dependencies").toArray();
	for (int i = 0; i < dependenciesArray.size(); i++) {
		QJsonObject o = dependenciesArray[i].toObject();
		Dependency* d = new Dependency();
		d->command = o.value("command").toString();
		d->package = o.value("package").toString();
		if (d->package == "") d->package = d->command;
		dependencies_.push_back(d);
	}

	QJsonArray commandArray = jsonObject.value("command").toArray();
	for (int i = 0; i < commandArray.size(); i++) {
		QString s = commandArray[i].toString();
		command_ << s;
	}

	QJsonArray array = perOsValue(jsonObject, "shortcuts").toArray();
	QList<QKeySequence> shortcuts;
	for (int i = 0; i < array.size(); i++) {
		QString s = array[i].toString();
		s = s.replace(" ", "");
		s = s.trimmed();
		QKeySequence ks(s);
		shortcuts.push_back(ks);
		defaultShortcuts_.push_back(ks);
	}

	setShortcuts(shortcuts);
}

QJsonValue Action::perOsValue(const QJsonObject& jsonObject, const QString& name) const {
	QJsonValue output;
#ifdef Q_OS_MAC
	QString key = "osx_" + name;
#elif defined(Q_OS_WIN)
	QString key = "win_" + name;
#elif defined(Q_OS_LINUX)
	QString key = "linux_" + name;
#else
#error Unsupported OS
#endif
	if (jsonObject.contains(key)) return jsonObject.value(key);
	return jsonObject.value(name);
}

bool Action::showConsole() const {
	return showConsole_;
}

bool Action::supports(const QKeySequence &keySequence) const {
	for (int i = 0; i < shortcuts().size(); i++) {
		const QKeySequence& ks = shortcuts()[i];
		if (ks.matches(keySequence) == QKeySequence::ExactMatch) return true;
	}
	return false;
}

void Action::setDefaultShortcuts(const QList<QKeySequence>& v) {
	defaultShortcuts_ = v;
}

void Action::restoreDefaultShortcut() {
	setShortcuts(defaultShortcuts());
}

void Action::setId(const QString& v) {
	id_ = v;
}

QString Action::id() const {
	return id_;
}

QString Action::menu() const {
	return menu_;
}

QString Action::description() const {
	return description_;
}

QStringList Action::command() const {
	return command_;
}

DependencyVector Action::dependencies() const {
	return dependencies_;
}

void Action::setMenu(const QString& menu) {
	menu_ = menu;
}

QList<QKeySequence> Action::defaultShortcuts() const {
	return defaultShortcuts_;
}

QKeySequence Action::defaultShortcut() const {
	if (defaultShortcuts_.size() <= 0) return QKeySequence();
	return defaultShortcuts_[0];
}

}
