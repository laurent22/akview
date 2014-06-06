#include "action.h"

namespace mv {

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
	return supports(keySequence);
}

bool Action::supports(const QKeySequence &keySequence) const {
	for (int i = 0; i < shortcuts().size(); i++) {
		const QKeySequence& ks = shortcuts()[i];
		if (ks.matches(keySequence)) return true;
	}
	return false;
}

QString Action::name() const {
	return name_;
}

}