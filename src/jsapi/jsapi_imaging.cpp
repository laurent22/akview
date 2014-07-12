#include "jsapi_imaging.h"

namespace jsapi {

JsImage::JsImage(QScriptEngine* engine) : QImage() {
	engine_ = engine;
}

QScriptValue JsImage::size_() const {
	QSize s = size();
	QScriptValue v = engine_->newObject();
	v.setProperty("width", s.width());
	v.setProperty("height", s.height());
	return v;
}


Imaging::Imaging(QScriptEngine* engine) {
	engine_ = engine;
}

QScriptValue Imaging::newImage(const QString& path) {
	JsImage* output = new JsImage(engine_);
	bool ok = output->load(path);
	if (!ok) qWarning() << qPrintable(QString("Could not load image: \"%1\"").arg(path));
	return engine_->newQObject(output, QScriptEngine::ScriptOwnership);
}

}