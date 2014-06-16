#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QFileSystemWatcher>
#include <QKeyEvent>
#include <QTimer>

#include "iapplication.h" // remove

#include "action.h"
#include "mainwindow.h"
#include "pluginmanager.h"
#include "preferencesdialog.h"
#include "simpletypes.h"

namespace mv {

class Application : public QApplication, IApplication {

	Q_OBJECT

public:

	static const int Backward = 1;
	static const int Forward = 2;

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
	void execAction(const QString& actionName);
	Action* actionByName(const QString& actionName) const;
	void refreshActionShortcuts();
	MainWindow* mainWindow() const;

protected:

	bool event(QEvent* event);
	void onSourceChange();
	void onZoomChange();
	void onExit();

private:

	MainWindow* mainWindow_;
	QString source_;
	PluginManager* pluginManager_;
	ActionVector builtinActions_;
	mutable QStringList sources_;
	mutable int sourceIndex_;
	mutable QString sourceDir_;
	mutable Settings* settings_;
	QStringQMenuMap menus_;
	PreferencesDialog* preferencesDialog_;
	QMenuBar* menuBar_;
	QTimer* preloadTimer_;
	int browsingDirection_;
	Action* createAction(const QString& name, const QString& text, const QString& menu, const QKeySequence& shortcut1 = QKeySequence(), const QKeySequence& shortcut2 = QKeySequence());
	void registerAction(const QString& menuName, Action* action);
	void playLoopAnimation();
	void saveWindowGeometry();
	void loadWindowGeometry();
	void setupActions();
	QFileSystemWatcher fsWatcher_;

public slots:

	void mainWindow_keypressed(QKeyEvent* event);
	void mainWindow_actionTriggered();
	void preloadTimer_timeout();
	void fsWatcher_fileChanged(const QString& path);

	QString source() const;
	void setSource(const QString& source);
	QStringList supportedFileExtensions() const;

	int sourceIndex() const;
	void setSourceIndex(int index);
	void nextSource();
	void previousSource();
	QString previousSourcePath() const;
	QString nextSourcePath() const;
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
