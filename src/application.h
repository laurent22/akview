#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include "iapplication.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mvplugininterface.h"

class Application : public QGuiApplication, IApplication {
	Q_OBJECT

public:

	explicit Application(int &argc, char **argv, int applicationFlags = ApplicationFlags);
	static Application* instance();
	void initialize();
	void testing();
	QString imageSource() const;
	void setImageSource(const QString& source);

protected:

	void onImageSourceChange();
	QObject* qmlRootObject() const;
	QObject* qmlImage() const;

private:

	QString imageSource_;
	QQmlApplicationEngine* engine_;

};

#endif // APPLICATION_H
