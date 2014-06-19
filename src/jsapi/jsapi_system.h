#ifndef MV_JSAPI_SYSTEM_H
#define MV_JSAPI_SYSTEM_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptValue>

namespace jsapi {

class System : public QObject {

	Q_OBJECT

public:

	System(QScriptEngine* engine);

public slots:

	QScriptValue exec(const QString& cmd);

private:

	QScriptEngine* engine_;

};

}

#endif