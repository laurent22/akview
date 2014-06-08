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
	Action(const QJsonObject& jsonObject);
	bool supports(const QKeySequence &keySequence) const;
	void setName(const QString& v);
	QString name() const;
	QString menu() const;
	void setMenu(const QString& menu);

private:

	QJsonObject jsonObject_;
	QString name_;
	QString menu_;

};

typedef std::vector<Action*> ActionVector;

}

#endif