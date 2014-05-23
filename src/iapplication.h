#ifndef IAPPLICATION_H
#define IAPPLICATION_H

class IApplication {

public slots:

	virtual QString mediaSource() const = 0;
	virtual void setMediaSource(const QString& source) = 0;
	virtual QStringList supportedFileExtensions() = 0;

};

#endif // IAPPLICATION_H
