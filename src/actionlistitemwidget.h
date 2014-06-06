#ifndef MV_ACTIONLISTWIDGETITEM_H
#define MV_ACTIONLISTWIDGETITEM_H

#include <QKeySequence>
#include <QListWidgetItem>

#include "action.h"

namespace mv {

class ActionListWidgetItem: public QListWidgetItem {

public:

	ActionListWidgetItem(Action* action);
	void setShortcut(const QKeySequence& ks);
	void setShortcutIsOverridden(bool v);
	bool shortcutIsOverridden() const;
	QKeySequence shortcut() const;
	Action* action() const;
	void updateDisplay();

private:

	Action* action_;
	QKeySequence shortcut_;
	bool shortcutIsOverridden_;

};

}

#endif