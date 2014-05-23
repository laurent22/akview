#include <QDebug>

#include "mvdeletefileplugin.h"
#include "qxtrash.h"

void MvDeleteFilePlugin::onKeypressed(mv::KeypressedEvent& event) {
	if (event.keyCode == Qt::Key_Delete || (event.keyCode == Qt::Key_Backspace && event.modifiers == Qt::ControlModifier)) {
		event.accepted = true;

		QxTrash trash;
		int sourceIndex = application->sourceIndex();
		QString source = application->mediaSource();
		if (source == "") return;

		bool ok = trash.moveToTrash(source);
		if (ok) {
			application->refreshSources();
			sourceIndex--;
			if (sourceIndex < 0) {
				QStringList sources = application->sources();
				if (sources.size() > 0) {
					sourceIndex = sources.size() - 1;
				}
			}

			if (sourceIndex < 0) {
				application->setMediaSource("");
			} else {
				application->setSourceIndex(sourceIndex);
			}
		}

		return;
	}
}
