#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QUrl>
#include "pluginmanager.h"

class Application : public QApplication, IApplication {
	Q_OBJECT

public:

	explicit Application(int &argc, char **argv, int applicationFlags = ApplicationFlags);
	static Application* instance();
	void initialize();

protected:

	void onImageSourceChange();
	QObject* qmlRootObject() const;
	QObject* qmlImage() const;
	QObject* qmlApplicationWindow() const;

private:

	QUrl imageSource_;
	QQmlApplicationEngine* engine_;
	PluginManager* pluginManager_;

public slots:

	void mainWindow_keypressed(int key, const QString& text, int modifiers);
	void mainWindow_sourceSelected(QString source);
	QUrl imageSource() const;
	void setImageSource(const QUrl& source);
	void setImageSource(const QString& source);
	QStringList supportedFileExtensions();

};

#endif // APPLICATION_H
