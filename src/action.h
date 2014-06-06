#ifndef MV_ACTION_H
#define MV_ACTION_H

#include <QAction>
#include <QJsonArray>
#include <QJsonObject>

#include "pluginevents.h"

namespace mv {

class Action: public QAction {

public:

	Action(const QJsonObject& jsonObject);
	bool supports(const QKeySequence &keySequence) const;
	QString name() const;

private:

	QJsonObject jsonObject_;
	QString name_;

};

typedef std::vector<Action*> ActionVector;

}

#endif