#ifndef MV_ACTION_H
#define MV_ACTION_H

#include <QAction>
#include <QJsonArray>
#include <QJsonObject>

#include "pluginevents.h"

namespace mv {

class Action: public QAction {

public:

	Action();
	Action(const QString& name, const QJsonObject& jsonObject);
	bool supports(const QKeySequence &keySequence) const;
	void setName(const QString& v);
	QString name() const;
	QString menu() const;
	QString description() const;
	QStringList command() const;
	void setMenu(const QString& menu);
	void setDefaultShortcuts(const QList<QKeySequence>& v);
	QList<QKeySequence> defaultShortcuts() const;
	QKeySequence defaultShortcut() const;
	void restoreDefaultShortcut();

private:

	QJsonObject jsonObject_;
	QString name_;
	QString menu_;
	QString description_;
	QStringList command_;
	QList<QKeySequence> defaultShortcuts_;

};

typedef std::vector<Action*> ActionVector;

}

#endif