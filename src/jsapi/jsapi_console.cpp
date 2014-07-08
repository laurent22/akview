#include "jsapi_console.h"
#include "../application.h"

#include <QScriptValueIterator>

namespace jsapi {

Console::Console() {}

void Console::saveVScrollValue(int v) {
	savedScrollValue_ = v;
}

void Console::info(const QString& s) { qDebug() << qPrintable(s); }
void Console::warn(const QString& s) { qWarning() << qPrintable(s); }
void Console::error(const QString& s) { qCritical() << qPrintable(s); }

QString Console::dir_(const QScriptValue& v, int indent) {
	QString output;

	if (v.isString()) {
		output = "\"" + v.toString() + "\"";
	} else if (v.isBool()) {
		output = v.toBool() ? "true" : "false";
	} else if (v.isNumber()) {
		output = v.toString();
	} else if (v.isObject()) {
		QScriptValueIterator it(v);
		while (it.hasNext()) {
			it.next();
			if (output != "") output += "\n";
			indent = !indent ? 1 : indent;
			for (int i = 0; i < indent; i++) output += "    ";
			output += it.name() + " : " + dir_(it.value(), indent + 1);
		}
		output = output != "" ? "Object\n" + output : "Object";
	}

	return output;
}

void Console::dir(const QScriptValue& v) {
	QString s = dir_(v, 0);
	qDebug() << qPrintable(s);
}

void Console::restoreVScrollValue() {
	mv::Application::instance()->mainWindow()->console()->setVScrollValue(savedScrollValue_);
}

void Console::show() {
	mv::Application::instance()->mainWindow()->showConsole();
}

void Console::hide() {
	mv::Application::instance()->mainWindow()->showConsole(false);
}

void Console::showLastOutput() {
	mv::Application::instance()->mainWindow()->showConsole();
	restoreVScrollValue();
}

}