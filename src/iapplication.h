#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include "settings.h"

namespace mv {

class IApplication {

public slots:

	virtual QString source() const = 0;
	virtual void setSource(const QString& source) = 0;
	virtual QStringList supportedFileExtensions() const = 0;
	virtual int sourceIndex() const = 0;
	virtual QStringList sources() const = 0;
	virtual void refreshSources() = 0;
	virtual void setSourceIndex(int index) = 0;
	virtual Settings* settings() const = 0;
	virtual void reloadSource() = 0;
	virtual bool runAppleScript(const QString& script) = 0;

};

}

#endif // IAPPLICATION_H
