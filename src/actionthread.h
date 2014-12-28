#ifndef MV_ACTIONTHREAD_H
#define MV_ACTIONTHREAD_H

namespace mv {

class ActionThread : public QThread {

	Q_OBJECT

public:

	ActionThread(QScriptEngine* engine, const QString& scriptContent, const QString& scriptPath);
	void run();
	void quit();

private:

	QScriptEngine* engine_;
	QString scriptContent_;
	QString scriptPath_;

};

}

#endif