#ifndef IAPPLICATION_H
#define IAPPLICATION_H

namespace mv {

class IApplication {

public slots:

	virtual QString mediaSource() const = 0;
	virtual void setMediaSource(const QString& source) = 0;
	virtual QStringList supportedFileExtensions() const = 0;
	virtual int sourceIndex() const = 0;
	virtual QStringList sources() const = 0;
	virtual void refreshSources() = 0;
	virtual void setSourceIndex(int index) = 0;

};

}

#endif // IAPPLICATION_H
