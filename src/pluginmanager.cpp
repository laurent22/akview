#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QProcess>

#include "action.h"
#include "application.h"
#include "pluginmanager.h"

namespace mv {

PluginManager::PluginManager() {

}

bool PluginManager::loadPlugin(const QString& folderPath) {
	Plugin* plugin = new Plugin(folderPath);
	bool ok = plugin->load();
	if (!ok) {
		qWarning() << "could not load plugin" << folderPath << ":" << plugin->errorMessage();
		delete plugin; plugin = NULL;
		return false;
	}

	plugins_.push_back(plugin);
	return true;
}

void PluginManager::loadPlugins(const QString &folderPath) {
	qDebug() << "Loading plugins from" << folderPath;

	QDir dir(folderPath);
	QFileInfoList files = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files[i];
		bool loaded = loadPlugin(file.absoluteFilePath());
		if (!loaded) {
			qWarning() << "Could not load plugin" << file.absoluteFilePath();
		} else {
			qDebug() << "Successfully loaded plugin" << file.absoluteFilePath();
		}
	}
}

PluginVector PluginManager::plugins() const {
	return plugins_;
}

QStringList PluginManager::replaceVariables(const QStringList& command) {
	Application* app = Application::instance();
	QString source = app->source();
	QStringList output = command;
	for (int i = 0; i < output.size(); i++) {
		QString s = output[i];
		s = s.replace("__FILE__", source);
		output[i] = s;
	}
	return output;
}

void PluginManager::onAction(const QString& actionName) {
	for (unsigned int i = 0; i < plugins_.size(); i++) {
		Plugin* plugin = plugins_[i];
		Action* action = plugin->findAction(actionName);
		if (!action) continue;

		QStringList command = replaceVariables(action->command());
		if (command.size() <= 0) {
			qWarning() << "action" << actionName << "has an empty command";
			return;
		}

		QString program = command[0];
		QStringList arguments = command.mid(1);
		QProcess process;
		process.start(program, arguments);
		process.waitForFinished(60000);

		if (process.exitStatus() != QProcess::NormalExit) {
			qWarning() << "process failed:" << command << QString(process.readAllStandardOutput()) << QString(process.readAllStandardError());
		} else {
			qDebug() << QString("process exited with error code %1").arg(process.exitCode()) << command << QString(process.readAllStandardOutput()) << QString(process.readAllStandardError());
		}
	
		qDebug() << "Action" << actionName << "has been processed by plugin" << plugin->description();
		return;
	}
}

}
