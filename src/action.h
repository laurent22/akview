#ifndef MV_ACTION_H
#define MV_ACTION_H

#include "pluginevents.h"

namespace mv {

struct Dependency {
	QString command;
	QString package;
};

typedef std::vector<Dependency*> DependencyVector;

class Action: public QAction {

public:

	Action();
	Action(const QJsonObject& jsonObject);
	bool supports(const QKeySequence &keySequence) const;
	void setId(const QString& v);
	QString id() const;
	QString menu() const;
	bool showConsole() const;
	QString description() const;
	QStringList command() const;
	bool batchModeSupported() const;
	void setMenu(const QString& menu);
	void setDefaultShortcuts(const QList<QKeySequence>& v);
	QList<QKeySequence> defaultShortcuts() const;
	QKeySequence defaultShortcut() const;
	void restoreDefaultShortcut();
	DependencyVector dependencies() const;

private:
	
	QJsonValue perOsValue(const QJsonObject& jsonObject, const QString& name) const;

	QJsonObject jsonObject_;
	QString id_;
	QString menu_;
	QString description_;
	QStringList command_;
	QList<QKeySequence> defaultShortcuts_;
	DependencyVector dependencies_;
	bool showConsole_;
	bool batchModeSupported_;

};

typedef std::vector<Action*> ActionVector;

}

#endif
