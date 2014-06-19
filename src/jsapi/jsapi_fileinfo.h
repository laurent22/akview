#ifndef MV_JSAPI_FILEINFO_H
#define MV_JSAPI_FILEINFO_H

#include <QObject>

namespace jsapi {

class FileInfo : public QObject {

	Q_OBJECT

public:

	FileInfo();

public slots:

	QString suffix(const QString& path) const;
	QString baseName(const QString& path) const;
	QString fileName(const QString& path) const;

};

}

#endif