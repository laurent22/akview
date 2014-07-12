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

QObject* Imaging::newImage(const QString& path) {
	JsImage* output = new JsImage(engine_);
	createdImages_.push_back(output);
	bool ok = output->load(path);
	if (!ok) qWarning() << qPrintable(QString("Could not load image: \"%1\"").arg(path));
	return (QObject*)output;
}

void Imaging::freeMemory() {
	for (unsigned int i = 0; i < createdImages_.size(); i++) {
		JsImage* image = createdImages_[i];
		delete image;
	}
	createdImages_.clear();
}

}