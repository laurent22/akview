#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QQmlApplicationEngine>

#include "action.h"
#include "pluginmanager.h"
#include "preferencesdialog.h"

namespace mv {

class Application : public QApplication, IApplication {
	Q_OBJECT

public:

	explicit Application(int &argc, char **argv, int applicationFlags = ApplicationFlags);
	~Application();
	static Application* instance();
	void initialize();
	void setWindowTitle(const QString& title);
	void showPreferencesDialog();
	PluginManager* pluginManager() const;
	ActionVector actions() const;
	bool actionShortcutIsOverridden(const QString& actionName) const;
	QKeySequence actionShortcut(const QString& actionName) const;
	QString shortcutAction(const QKeySequence& shortcut) const;

protected:

	bool event(QEvent* event);

	void onMediaSourceChange();
	void onExit();

	QObject* qmlRootObject() const;
	QObject* qmlImage() const;
	QObject* qmlApplicationWindow() const;

private:

	QString source_;
	QQmlApplicationEngine* engine_;
	PluginManager* pluginManager_;
	ActionVector actions_;
	mutable QStringList sources_;
	mutable int sourceIndex_;
	mutable QString sourceDir_;
	mutable Settings* settings_;
	PreferencesDialog* preferencesDialog_;
	Action* addAction(const QString& name, const QString& text, const QKeySequence& shortcut);
	Action* addAction(const QString& name, const QString& text, const QKeySequence& shortcut1, const QKeySequence& shortcut2);
	void playLoopAnimation();

public slots:

	void mainWindow_keypressed(int key, const QString& text, int modifiers);
	void mainWindow_actionTriggered(const QString& name);

	QString source() const;
	void setSource(const QString& source);
	QStringList supportedFileExtensions() const;

	int sourceIndex() const;
	void setSourceIndex(int index);
	void nextSource();
	void previousSource();
	QStringList sources() const;
	QStringList sources(const QString& filePath) const;
	void refreshSources();
	void reloadSource() const;
	void exifClearOrientation(const QString& filePath);
	bool runAppleScript(const QString& script);

	Settings* settings() const;

};

}

#endif // APPLICATION_H
