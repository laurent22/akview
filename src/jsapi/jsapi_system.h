#ifndef MV_JSAPI_SYSTEM_H
#define MV_JSAPI_SYSTEM_H

#include <QProcess>
#include <QScriptEngine>
#include <QScriptValue>

namespace jsapi {

class System : public QObject {

	Q_OBJECT
	Q_PROPERTY(QString os READ os)

public:

	System(QScriptEngine* engine);

public slots:

	QScriptValue exec(const QString& cmd);
	QScriptValue exec(const QString& program, const QStringList& cmd);
	QString os() const;

private:

	QScriptEngine* engine_;
	QScriptValue buildExecResponse(QProcess& p);

};

}

#endif