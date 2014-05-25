#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QUrl>

#include "pluginmanager.h"

namespace mv {

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
	QObject *qmlModelLayer() const;

private:

	QString mediaSource_;
	QQmlApplicationEngine* engine_;
	PluginManager* pluginManager_;
	mutable QStringList sources_;
	mutable int sourceIndex_;
	mutable QString sourceDir_;
	mutable Settings* settings_;

public slots:

	void mainWindow_keypressed(int key, const QString& text, int modifiers);

	QString mediaSource() const;
	void setMediaSource(const QString& source);
	QStringList supportedFileExtensions() const;

	int sourceIndex() const;
	void setSourceIndex(int index);
	void nextSource();
	void previousSource();
	QStringList sources() const;
	void refreshSources();
	void reloadSource() const;

	Settings* settings() const;

};

}

#endif // APPLICATION_H
