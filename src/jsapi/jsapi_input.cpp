#include "jsapi_input.h"

namespace jsapi {

Input::Input(QScriptEngine* engine, const QStringList& filePaths) {
	engine_ = engine;
	filePaths_ = filePaths;
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

QString Input::escapePath(const QString& path) const {
	QString output = path;
	output.replace("\"", "\\\"");
	output = "\"" + output + "\"";
	return output;
}

}