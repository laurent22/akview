#ifndef JHEADHANDLER_H
#define JHEADHANDLER_H

#include <QObject>
#include "third_party/jhead/jhead.h"

class JheadHandler : public QObject {

	Q_OBJECT

public:

	JheadHandler(const QString& filePath);
	~JheadHandler();
	bool isValid() const;
	bool clearOrientation();
	bool save();

private:

	bool isValid_;
	QString filePath_;
	bool wasChanged_;

};

#endif // JHEADHANDLER_H
