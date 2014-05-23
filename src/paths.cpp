#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "constants.h"
#include "paths.h"

Paths::Paths() {

}

QString Paths::applicationFolder() const {
	return qApp->applicationDirPath();
}

QString Paths::pluginFolder() const {
	QString appFilePath = qApp->applicationFilePath();
	QDir dir = QFileInfo(appFilePath).dir();

#ifdef QT_DEBUG
	if (appFilePath.indexOf(".app/Contents/MacOS/") >= 0) {
		dir.cdUp();
		dir.cdUp();
		dir.cdUp();
	}

	dir.cdUp();
	dir.cd("plugins/debug");
#else // QT_DEBUG

#ifdef MV_DEBUG
	if (appFilePath.indexOf(".app/Contents/MacOS/") >= 0) {
		dir.cdUp();
		dir.cdUp();
		dir.cdUp();
	}

	dir.cdUp();
	dir.cd("plugins/release");
#else
	if (appFilePath.indexOf(".app/Contents/MacOS/") >= 0) {
		dir.cdUp();
		dir.cd("PlugIns/multiviewer");
	}
#endif // MV_DEBUG

#endif // QT_DEBUG

	return dir.absolutePath();
}