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

// void Application::form(const QScriptValue& form) {
// 	FormElements formElements;
// 	QVariantList elements = form.toVariant().toList();
// 	for (int i = 0; i < elements.size(); i++) {
// 		QVariantMap e = elements[i].toMap();
// 		FormElement* formElement = new FormElement(e);
// 		formElements.push_back(formElement);
// 		// qDebug() << formElement->name() << formElement->type() << formElement->title();
// 	}

// 	FormDialog* dialog = new FormDialog(formElements);
// 	dialog->setModal(true);
// 	dialog->exec();
// }

}