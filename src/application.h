#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPluginLoader>
#include "iapplication.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QUrl>
#include "mvplugininterface.h"

typedef std::vector<MvPluginInterface*> PluginVector;

class Application : public QGuiApplication, IApplication {
	Q_OBJECT

public:

	explicit Application(int &argc, char **argv, int applicationFlags = ApplicationFlags);
	static Application* instance();
	void initialize();

protected:

	void onImageSourceChange();
	QObject* qmlRootObject() const;
	QObject* qmlImage() const;

private:

	QUrl imageSource_;
	QQmlApplicationEngine* engine_;
	PluginVector plugins_;

public slots:

	void mainWindow_keypressed(int key);
	QUrl imageSource() const;
	void setImageSource(const QUrl& source);
	void setImageSource(const QString& source);

};

#endif // APPLICATION_H
