#include "jsapi_plugin.h"
#include "../scriptutil.h"
#include "../settings.h"

namespace jsapi {

Plugin::Plugin(QScriptEngine* engine, mv::Plugin* plugin, mv::Action* action) {
	engine_ = engine;
	plugin_ = plugin;
	action_ = action;
}

QScriptValue Plugin::setting(const QString& name, const QScriptValue& defaultValue) {
	mv::Settings settings;
	settings.beginGroup("plugins/" + plugin_->id());
	QVariant v = settings.value(name);
	if (v.isNull() && !defaultValue.isNull()) {
		return defaultValue;
	}
	return mv::scriptutil::variantToScriptValue(v);
}

void Plugin::setSetting(const QString& name, const QScriptValue& value) {
	mv::Settings settings;
	settings.beginGroup("plugins/" + plugin_->id());
	settings.setValue(name, value.toVariant());
}

}