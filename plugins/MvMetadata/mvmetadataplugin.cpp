#include <QDebug>

#include "mvmetadataplugin.h"

void MvMetadataPlugin::onAction(const QString &action) {
	if (action != "information") return;

	qDebug() << "pluginaaaaaaaaaaaaa";
}
