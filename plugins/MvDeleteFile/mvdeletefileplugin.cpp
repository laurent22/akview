#include <QDebug>

#include "mvdeletefileplugin.h"
#include "qxtrash.h"

void MvDeleteFilePlugin::onKeypressed(mv::KeypressedEvent& event) {
	if (event.keyCode == Qt::Key_Delete || (event.keyCode == Qt::Key_Backspace && event.modifiers == Qt::ControlModifier)) {
		event.accepted = true;

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

		return;
	}
}
