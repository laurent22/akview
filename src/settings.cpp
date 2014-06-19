#include "settings.h"
#include "version.h"

namespace mv {

#ifdef QT_NO_DEBUG
Settings::Settings() : QSettings(VER_COMPANYNAME_STR, VER_PRODUCTNAME_STR) {
#elif defined(QT_DEBUG)
// Make sure debug version don't use or break settings of release version
Settings::Settings() : QSettings(VER_COMPANYNAME_STR, QString("%1_DEBUG").arg(VER_PRODUCTNAME_STR)) {
#endif

}

QVariant Settings::value(const QString & key, const QVariant & defaultValue) const {
	QVariant v = QSettings::value(key, defaultValue);
	if (key == "undoSize" && v.isNull()) return QVariant(10);
	return v;
}

}
