#ifndef SCRIPTUTIL_H
#define SCRIPTUTIL_H

#include <QScriptValue>
#include <QVariant>

namespace mv {

namespace scriptutil {

	QScriptValue variantToScriptValue(const QVariant& v);

}

}

#endif
