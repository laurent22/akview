#include <QtWidgets>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "mvbrowserplugin.h"

void MvBrowserPlugin::onKeypressed(const KeypressedEvent& event) {
	if (event.keyCode == Qt::Key_Left) {
		previousSource();
	}

	if (event.keyCode == Qt::Key_Right) {
		nextSource();
	}
}

void MvBrowserPlugin::setSourceListIndex(int index) {
	QStringList sources = sourceList();
	if (!sources.size()) return;

	QString sourceString = sources[index];
	sourceListIndex_ = index;

	this->application->setImageSource(sourceString);
}

void MvBrowserPlugin::nextSource() {
	int index = sourceListIndex();
	QStringList sources = sourceList();
	index++;
	if (index >= sources.size()) index = 0;
	setSourceListIndex(index);
}

void MvBrowserPlugin::previousSource() {
	int index = sourceListIndex();
	QStringList sources = sourceList();
	index--;
	if (index < 0) index = sources.size() - 1;
	setSourceListIndex(index);
}

int MvBrowserPlugin::sourceListIndex() const {
	QUrl sourceUrl = this->application->imageSource();

	if (QFileInfo(sourceUrl.toLocalFile()).dir().absolutePath() != sourceListDir_) {
		// Current dir has changed - reload source list
		sourceListIndex_ = -1;
		sourceList_.clear();
	}

	QStringList sources = sourceList();
	if (!sources.size()) return -1;

	QString source = QFileInfo(sourceUrl.toLocalFile()).fileName();

	// Check if the index we have is correct
	if (sourceListIndex_ >= 0 && sourceListIndex_ < sources.size() && sources[sourceListIndex_] == source) return sourceListIndex_;

	// If it's not correct, try to get it from the current source and source list
	sourceListIndex_ = -1;

	for (int i = 0; i < sources.size(); i++) {
		if (QFileInfo(sources[i]).fileName() == source) {
			sourceListIndex_ = i;
			break;
		}
	}
	return sourceListIndex_;
}

QStringList MvBrowserPlugin::sourceList() const {
	if (sourceList_.length()) return sourceList_;

	QStringList supportedFileExtensions = application->supportedFileExtensions();
	sourceListIndex_ = -1;
	QUrl source = application->imageSource();
	if (source.isLocalFile()) {
		QDir dir = QFileInfo(source.toLocalFile()).dir();
		sourceListDir_ = dir.absolutePath();
		QFileInfoList files = dir.entryInfoList(QDir::Files, QDir::LocaleAware);
		for (int i = 0; i < files.size(); i++) {
			if (!supportedFileExtensions.contains(files[i].suffix().toLower())) continue;
			sourceList_.append(files[i].absoluteFilePath());
		}
	}

	return sourceList_;
}
