#include <version.h>

#include <QString>
#include <QStringList>

namespace version {

int majorNumber() {
	QStringList s = QString("%1").arg(VER_PRODUCTVERSION_STR).split(".");
	return s[0].toInt();
}

QString number() {
    return QString("%1").arg(VER_PRODUCTVERSION_STR);
}

}
