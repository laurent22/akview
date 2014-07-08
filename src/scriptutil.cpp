#include "scriptutil.h"

#include <QDebug>

namespace mv {
namespace scriptutil {

QScriptValue variantToScriptValue(const QVariant& v) {
	if (v.type() == QVariant::String) return QScriptValue(v.toString());
	if (v.type() == QVariant::Int) return QScriptValue(v.toInt());
	if (v.type() == QVariant::Bool) return QScriptValue(v.toBool());
	if (v.type() == QVariant::Double) return QScriptValue(v.toDouble());
	qWarning() << "Unsupported type:" << v.type();
	return QScriptValue();
}

}
}