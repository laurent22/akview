#include "jheadhandler.h"

#include <QDebug>
#include <QFile>

JheadHandler::JheadHandler(const QString& filePath) {
	isValid_ = false;
	filePath_ = filePath;
	wasChanged_ = false;

	jhead::ResetJpgfile();
	int result = jhead::ReadJpegFile(filePath_.toStdString().c_str(), jhead::READ_ALL);
	isValid_ = result ? true : false;

	if (!isValid()) qDebug() << "Jpeg file" << filePath << "could not be read.";
}

JheadHandler::~JheadHandler() {
	save();
	jhead::DiscardData();
}

bool JheadHandler::isValid() const {
	return isValid_;
}

bool JheadHandler::clearOrientation() {
	if (!isValid()) return false;
	const char* arg = jhead::ClearOrientation();
	if (!arg) return false;
	wasChanged_ = true;
	return true;
}

bool JheadHandler::save() {
	if (!isValid() || !wasChanged_) return false;

	QString baseBackupPath = filePath_ + ".bak";
	QString backupPath = baseBackupPath;
	int inc = 1;
	while (QFile::exists(backupPath)) {
		backupPath = baseBackupPath + "-" + QString::number(inc);
		inc++;
	}
	QFile::rename(filePath_, backupPath);
	int result = jhead::WriteJpegFile(filePath_.toStdString().c_str());
	if (!result) {
		// Restore backup
		QFile::remove(filePath_);
		QFile::rename(backupPath, filePath_);
	} else {
		QFile::remove(backupPath);
	}

	return result ? true : false;
}
