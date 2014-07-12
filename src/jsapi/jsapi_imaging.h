#ifndef MV_JSAPI_IMAGING_H
#define MV_JSAPI_IMAGING_H

namespace jsapi {

class JsImage: public QObject, public QImage {

	Q_OBJECT
	Q_PROPERTY(QScriptValue size READ size_)

public:

	JsImage(QScriptEngine* engine);

public slots:

	QScriptValue size_() const;

private:

	QScriptEngine* engine_;

};

class Imaging : public QObject {

	Q_OBJECT

public:

	Imaging(QScriptEngine* engine);

public slots:

	QScriptValue newImage(const QString& path);

private:

	QScriptEngine* engine_;

};

}

#endif