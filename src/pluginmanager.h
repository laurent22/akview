#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugin.h"

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

public slots:

	void packageManager_installationDone();

};

}

#endif // PLUGINMANAGER_H
