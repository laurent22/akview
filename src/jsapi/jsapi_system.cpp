#include "jsapi_system.h"

#include <QDebug>

namespace jsapi {

System::System(QScriptEngine* engine) {
	engine_ = engine;
}

QScriptValue System::exec(const QString& cmd) {
	qDebug() << qPrintable("$ " + cmd);

	QProcess p;
	p.start(cmd);
	p.waitForFinished(-1);
	
	return buildExecResponse(p);
}

QScriptValue System::exec(const QString& program, const QStringList& args) {
	qDebug() << qPrintable("$ " + program) << args;

	QProcess p;
	p.start(program, args);
	p.waitForFinished(-1);
	
	return buildExecResponse(p);
}

QScriptValue System::buildExecResponse(QProcess& p) {
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

QString System::os() const {
#ifdef Q_OS_OSX
	return "osx";
#elif defined(Q_OS_WIN)
	return "windows";
#elif defined(Q_OS_LINUX)
	return "linux";
#else
	#error Unknown operating system
#endif
}

}