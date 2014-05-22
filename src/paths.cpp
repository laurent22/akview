#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

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
	return dir.absolutePath();
#else
	return "";
#endif
}
