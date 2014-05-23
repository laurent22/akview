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
	~Application();
	static Application* instance();
	void initialize();
	void setWindowTitle(const QString& title);

protected:

	bool event(QEvent* event);

	void onMediaSourceChange();
	void onExit();

	QObject* qmlRootObject() const;
	QObject* qmlImage() const;
	QObject* qmlApplicationWindow() const;

private:

	QString mediaSource_;
	QQmlApplicationEngine* engine_;
	PluginManager* pluginManager_;

public slots:

	void mainWindow_keypressed(int key, const QString& text, int modifiers);

	QString mediaSource() const;
	void setMediaSource(const QString& source);
	QStringList supportedFileExtensions();

};

#endif // APPLICATION_H
