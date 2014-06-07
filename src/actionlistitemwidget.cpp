#include "actionlistitemwidget.h"

namespace mv {

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
	if (shortcutIsOverridden()) return shortcut_;
	return action()->shortcut();
}

void ActionListWidgetItem::setShortcutIsOverridden(bool v) {
	shortcutIsOverridden_ = v;
}

bool ActionListWidgetItem::shortcutIsOverridden() const {
	return shortcutIsOverridden_;
}

void ActionListWidgetItem::updateDisplay() {
	QFont font(this->font());
	font.setBold(shortcutIsOverridden());
	setFont(font);
	QString shortcutString = shortcut().toString();
	if (shortcutString == "") shortcutString = QObject::tr("(None)");
	setText(action()->text() + " - " + shortcutString);
}

}