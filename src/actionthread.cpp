#include "actionthread.h"

namespace mv {

ActionThread::ActionThread(QScriptEngine* engine, const QString& scriptContent, const QString& scriptPath) {
	engine_ = engine;
	scriptContent_ = scriptContent;
	scriptPath_ = scriptPath;
}

void ActionThread::run() {
	qDebug() << "Running" << scriptPath_;
	engine_->evaluate(scriptContent_, scriptPath_);
	engine_->collectGarbage();
}

void ActionThread::quit() {
	engine_->abortEvaluation();
	QThread::quit();
}

}