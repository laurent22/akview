#ifndef JSAPI_APPLICATION_H
#define JSAPI_APPLICATION_H

#include <QScriptEngine>

namespace jsapi {

class Application : public QObject {

	Q_OBJECT

public:

	Application(QScriptEngine* engine);

public slots:

	void pushUndoState();
	void popUndoState();

private:

	QScriptEngine* engine_;

};

}

#endif