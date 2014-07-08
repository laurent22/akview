#ifndef JSAPI_PLUGIN_H
#define JSAPI_PLUGIN_H

#include <QScriptEngine>
#include <QScriptValue>

#include "../action.h"
#include "../plugin.h"

// namespace mv {
// 	class Plugin;
// 	class Action;
// }

namespace jsapi {

class Plugin : public QObject {

	Q_OBJECT

public:

	Plugin(QScriptEngine* engine, mv::Plugin* plugin, mv::Action* action);

public slots:

	QScriptValue setting(const QString& name, const QScriptValue& defaultValue = QScriptValue());
	void setSetting(const QString& name, const QScriptValue& value);

private:

	QScriptEngine* engine_;
	mv::Plugin* plugin_;
	mv::Action* action_;

};

}

#endif