#include "jsapi_system.h"

#include <QDebug>
#include <QProcess>

namespace jsapi {

System::System(QScriptEngine* engine) {
	engine_ = engine;
}

QScriptValue System::exec(const QString& cmd) {
	qDebug() << qPrintable("$ " + cmd);

	QProcess p;
	p.start(cmd);
	p.waitForFinished(-1);
	QString output = QString(p.readAllStandardError());
	if (output.trimmed() != "") output += "\n";
	output += QString(p.readAllStandardOutput());
	if (output.trimmed() == "") output = "";

	QScriptValue v = engine_->newObject();
	v.setProperty("exitCode", p.exitCode());
	v.setProperty("exitStatus", p.exitStatus());
	v.setProperty("output", output);
	return v;
}

}