#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings  {

	Q_OBJECT

public:

	explicit Settings();

};

#endif // SETTINGS_H
