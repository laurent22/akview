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
	
	QString stderr = QString(p.readAllStandardError());
	if (stderr.trimmed() == "") stderr = "";

	QString stdout = QString(p.readAllStandardOutput());
	if (stdout.trimmed() == "") stdout = "";

	QString output = stderr;
	if (output != "") output += "\n";
	output += stdout;

	if (output != "") qDebug() << qPrintable(output.trimmed());

	QScriptValue v = engine_->newObject();
	v.setProperty("exitCode", p.exitCode());
	v.setProperty("exitStatus", p.exitStatus());
	v.setProperty("stdout", stdout);
	v.setProperty("stderr", stderr);
	return v;
}

}