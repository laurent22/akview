#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QKeyEvent>

#include "action.h"
#include "mainwindow.h"
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

private:

	MainWindow* mainWindow_;
	QString source_;
	PluginManager* pluginManager_;
	ActionVector actions_;
	mutable QStringList sources_;
	mutable int sourceIndex_;
	mutable QString sourceDir_;
	mutable Settings* settings_;
	PreferencesDialog* preferencesDialog_;
	Action* addAction(const QString& name, const QString& text, const QStringList& menu, const QKeySequence& shortcut);
	Action* addAction(const QString& name, const QString& text, const QStringList& menu, const QKeySequence& shortcut1, const QKeySequence& shortcut2);
	void playLoopAnimation();
	void saveWindowGeometry();
	void loadWindowGeometry();

public slots:

	void mainWindow_keypressed(QKeyEvent* event);
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
