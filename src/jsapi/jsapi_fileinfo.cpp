#include "jsapi_fileinfo.h"

#include <QFileInfo>

namespace jsapi {

FileInfo::FileInfo() {}

QString FileInfo::suffix(const QString& path) const {
	return QFileInfo(path).suffix();
}

QString FileInfo::baseName(const QString& path) const {
	return QFileInfo(path).baseName();
}

QString FileInfo::fileName(const QString& path) const {
	return QFileInfo(path).fileName();
}

}