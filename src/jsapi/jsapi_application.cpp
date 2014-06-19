#include "jsapi_application.h"
#include "../application.h"

namespace jsapi {

Application::Application(QScriptEngine* engine) {
	engine_ = engine;
}

void Application::pushUndoState() {
	mv::Application::instance()->pushUndoState();
}

void Application::popUndoState() {
	mv::Application::instance()->popUndoState();
}

void Application::showConsole(bool doShow) {
	mv::Application::instance()->mainWindow()->showConsole(doShow);
}

}