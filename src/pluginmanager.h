#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "actionthread.h"
#include "plugin.h"
#include "progressbardialog.h"

namespace mv {

class PluginManager : public QObject {

	Q_OBJECT

public:

	PluginManager();
	bool loadPlugin(const QString& folderPath);
	void loadPlugins(const QString& folderPath);
	PluginVector plugins() const;
	void execAction(const QString& actionName, const QStringList& filePaths);

private:

	PluginVector plugins_;
	QString afterPackageInstallationAction_;
	QStringList afterPackageInstallationFilePaths_;
	QScriptEngine* scriptEngine_;
	QStringList replaceVariables(const QStringList& command);
	QObject* jsConsole_;
	QObject* jsImaging_;
	QObject* jsSystem_;
	ActionThread* actionThread_;
	bool canceling_;

public slots:

	void packageManager_installationDone();
	void actionThread_finished();
	void mainWindow_cancelButtonClicked();

};

}

#endif // PLUGINMANAGER_H
