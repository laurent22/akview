#include "jsapi_console.h"
#include "../application.h"

namespace jsapi {

Console::Console() {}

void Console::saveVScrollValue(int v) {
	savedScrollValue_ = v;
}

void Console::info(const QString& s) { qDebug() << qPrintable(s); }
void Console::warn(const QString& s) { qWarning() << qPrintable(s); }
void Console::error(const QString& s) { qCritical() << qPrintable(s); }

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