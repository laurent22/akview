#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

namespace mv {

class Settings : public QSettings  {

	Q_OBJECT

public:

	explicit Settings();
	QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;

};

}

#endif // SETTINGS_H
