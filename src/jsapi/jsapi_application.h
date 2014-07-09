#ifndef JSAPI_APPLICATION_H
#define JSAPI_APPLICATION_H

namespace jsapi {

class Application : public QObject {

	Q_OBJECT

public:

	Application(QScriptEngine* engine);

public slots:

	void pushUndoState();
	void popUndoState();
	// void form(const QScriptValue& form);

private:

	QScriptEngine* engine_;

};

}

#endif