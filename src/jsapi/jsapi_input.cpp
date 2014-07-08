#include "jsapi_input.h"

namespace jsapi {

Input::Input(QScriptEngine* engine, const QStringList& filePaths, const QRect& selectionRect, const QSize& imageSize) {
	engine_ = engine;
	filePaths_ = filePaths;
	selectionRect_ = selectionRect;
	imageSize_ = imageSize;
}

QString Input::filePath() const {
	if (filePaths_.size() == 0) return "";
	return filePaths_[0];
}

QString Input::escapedFilePath() const {
	if (filePaths_.size() == 0) return "";
	return escapePath(filePaths_[0]);
}

QStringList Input::filePaths() const {
	return filePaths_;
}

QString Input::escapedFilePaths() const {
	QString output;
	for (int i = 0; i < filePaths_.size(); i++) {
		QString f = filePaths_[i];
		if (output != "") output += " ";
		output += escapePath(f);
	}
	return output;
}

QScriptValue Input::selectionRect() const {
	if (!selectionRect_.isValid()) return QScriptValue(QScriptValue::UndefinedValue);
	QScriptValue v = engine_->newObject();
	v.setProperty("x", selectionRect_.x());
	v.setProperty("y", selectionRect_.y());
	v.setProperty("width", selectionRect_.width());
	v.setProperty("height", selectionRect_.height());
	return v;
}

QString Input::escapePath(const QString& path) const {
	QString output = path;
	output.replace("\"", "\\\"");
	output = "\"" + output + "\"";
	return output;
}

QScriptValue Input::imageSize() const {
	QScriptValue v = engine_->newObject();
	v.setProperty("width", imageSize_.width());
	v.setProperty("height", imageSize_.height());
	return v;
}

}