#include <QDebug>

#include "mvdeletefileplugin.h"
#include "qxtrash.h"

void MvDeleteFilePlugin::onAction(const QString &action) {
	if (action != "delete") return;

	QxTrash trash;
	int sourceIndex = application->sourceIndex();
	QString source = application->source();
	if (source == "") return;

	bool ok = trash.moveToTrash(source);
	if (ok) {
		application->refreshSources();
		QStringList sources = application->sources();
		if (sources.size() <= 0) {
			application->setSource("");
		} else {
			if (sourceIndex >= sources.size()) sourceIndex = 0;
			application->setSourceIndex(sourceIndex);
		}
	}
}
