#ifndef MV_JSAPI_CONSOLE_H
#define MV_JSAPI_CONSOLE_H

#include <QDebug>

namespace jsapi {

class Console : public QObject {

	Q_OBJECT

public:

	Console() {};

public slots:

	inline void info(const QString& s) { qDebug() << qPrintable(s); }
	inline void warn(const QString& s) { qWarning() << qPrintable(s); }
	inline void error(const QString& s) { qCritical() << qPrintable(s); }

};

}

#endif