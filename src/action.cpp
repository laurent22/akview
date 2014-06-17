#include "action.h"

#include <QDebug>

namespace mv {

Action::Action(): QAction(NULL) {}

Action::Action(const QString& name, const QJsonObject &jsonObject): QAction(NULL) {
	jsonObject_ = jsonObject;

	name_ = name;
	setText(jsonObject_.value("title").toString());
	menu_ = jsonObject_.value("menu").toString();
	description_ = jsonObject_.value("description").toString();

	QJsonArray dependenciesArray;

#ifdef Q_OS_MAC
	dependenciesArray = jsonObject.value("osx_dependencies").toArray();
#elif defined(Q_OS_WIN)
	dependenciesArray = jsonObject.value("win_dependencies").toArray();
#elif defined(Q_OS_LINUX)
	dependenciesArray = jsonObject.value("linux_dependencies").toArray();
#endif

	if (dependenciesArray.size() == 0) dependenciesArray = jsonObject.value("dependencies").toArray();
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

	QJsonArray array = jsonObject.value("shortcuts").toArray();
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

void Action::setName(const QString& v) {
	name_ = v;
}

QString Action::name() const {
	return name_;
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