#include "jsapi_application.h"
#include "../application.h"

namespace jsapi {

Application::Application(QScriptEngine* engine) {
	engine_ = engine;
}

void Application::pushUndoState() {
	mv::Application::instance()->pushUndoState();
}

}