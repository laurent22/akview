#ifndef MV_JSAPI_SYSTEM_H
#define MV_JSAPI_SYSTEM_H

namespace jsapi {

class System : public QObject {

	Q_OBJECT
	Q_PROPERTY(QString os READ os)

public:

	System(QScriptEngine* engine);
	void resetState();

public slots:

	QScriptValue exec(const QString& cmd);
	QScriptValue exec(const QString& program, const QStringList& cmd);
	QString os() const;
	void onScriptAbort();
	void execProcess_finished(int, QProcess::ExitStatus);

private:

	QScriptEngine* engine_;
	QScriptValue handleExecProcess();
	QScriptValue buildExecResponse(QProcess* p);
	QScriptValue buildExecResponse(int exitCode, int exitStatus, QString stdout, QString stderr) const;
	QProcess* execProcess_;
	bool scriptAborting_;

};

}

#endif