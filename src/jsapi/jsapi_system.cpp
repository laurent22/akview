#include "jsapi_system.h"

namespace jsapi {

System::System(QScriptEngine* engine) {
	engine_ = engine;
	execProcess_ = NULL;
	resetState();
}

void System::resetState() {
	if (execProcess_) {
		delete execProcess_;
		execProcess_ = NULL;
	}
	scriptAborting_ = false;
}

QScriptValue System::handleExecProcess() {
	execProcess_->waitForFinished(-1);
	QScriptValue output;
	if (!scriptAborting_ && execProcess_) {
		output = buildExecResponse(execProcess_);
	} else {
		output = buildExecResponse(0, QProcess::CrashExit, "", "");
	}
	return output;
}

QScriptValue System::exec(const QString& cmd) {
	qDebug() << qPrintable("$ " + cmd);
	execProcess_ = new QProcess();
	connect(execProcess_, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(execProcess_finished(int, QProcess::ExitStatus)), Qt::DirectConnection);
	execProcess_->start(cmd);
	return handleExecProcess();
}

QScriptValue System::exec(const QString& program, const QStringList& args) {
	qDebug() << qPrintable("$ " + program) << args;
	execProcess_ = new QProcess();
	connect(execProcess_, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(execProcess_finished(int, QProcess::ExitStatus)), Qt::DirectConnection);
	execProcess_->start(program, args);
	return handleExecProcess();
}

void System::execProcess_finished(int, QProcess::ExitStatus) {
	if (execProcess_) {
		delete execProcess_;
		execProcess_ = NULL;
	}
}

QScriptValue System::buildExecResponse(QProcess* p) {
	QString stderr = QString(p->readAllStandardError());
	if (stderr.trimmed() == "") stderr = "";

	QString stdout = QString(p->readAllStandardOutput());
	if (stdout.trimmed() == "") stdout = "";

	QString output = stderr;
	if (output != "") output += "\n";
	output += stdout;

	if (output != "") qDebug() << qPrintable(output.trimmed());

	return buildExecResponse(p->exitCode(), p->exitStatus(), stdout, stderr);
}

QScriptValue System::buildExecResponse(int exitCode, int exitStatus, QString stdout, QString stderr) const {
	QScriptValue v = engine_->newObject();
	v.setProperty("exitCode", exitCode);
	v.setProperty("exitStatus", exitStatus);
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

void System::onScriptAbort() {
	if (execProcess_) {
		scriptAborting_ = true;
		QString s = QString("Terminating: %1").arg(execProcess_->program());
		if (execProcess_->arguments().size()) s += QString(" %1").arg(execProcess_->arguments().join(" "));
		qDebug() << qPrintable(s);
		execProcess_->terminate();
		execProcess_->waitForFinished(300);
		execProcess_->kill();
		execProcess_->waitForFinished(1000);
		scriptAborting_ = false;
	}
}

}