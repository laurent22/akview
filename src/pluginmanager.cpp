#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <QProcess>

#include "action.h"
#include "application.h"
#include "messageboxes.h"
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

		PackageManager* packageManager = Application::instance()->packageManager();

		QString program = command[0];
		QStringList arguments = command.mid(1);

		QStringList missingPackages;
		DependencyVector dependencies = action->dependencies();
		for (unsigned int i = 0; i < dependencies.size(); i++) {
			Dependency* dependency = dependencies[i];
			if (!packageManager->commandIsInstalled(dependency->command)) {
			 	missingPackages.push_back(dependency->package);
			}
		}

		if (missingPackages.size()) {
			int answer = messageBoxes::info(
				QObject::tr("In order to do this operation, the following package(s) must be installed:\n\n%1\n\nDo you wish to install them now?").arg(missingPackages.join(", ")),
				QObject::tr("Information"),
				"okCancel"
			);
			if (answer == QMessageBox::Cancel) return;

			Application::instance()->mainWindow()->showConsole();

			afterPackageInstallationAction_ = actionName;
			connect(packageManager, SIGNAL(installationDone()), this, SLOT(packageManager_installationDone()));
			packageManager->install(missingPackages);
			return;
		}

		qDebug() << qPrintable("$ " + command.join(" "));

		int consoleVScrollValue = 0;
		if (action->showConsole()) {
			Application::instance()->mainWindow()->showConsole();
			consoleVScrollValue = Application::instance()->mainWindow()->console()->documentSize().height();
		}

		QProcess process;
		process.start(program, arguments);
		process.waitForFinished(60000);

		if (process.exitStatus() != QProcess::NormalExit) {
			qWarning() << "Error:" << QString(process.readAllStandardOutput()) << QString(process.readAllStandardError());
		} else {
			QString s = QString(process.readAllStandardError()).trimmed();
			if (s != "") qDebug() << qPrintable(s);
			s = qPrintable(QString(process.readAllStandardOutput()).trimmed());
			if (s != "") qDebug() << qPrintable(s);
		}
	
		if (action->showConsole()) {
			Application::instance()->mainWindow()->console()->setVScrollValue(consoleVScrollValue);
		}
		return;
	}
}

void PluginManager::packageManager_installationDone() {
	PackageManager* packageManager = Application::instance()->packageManager();
	disconnect(packageManager, SIGNAL(installationDone()), this, SLOT(packageManager_installationDone()));
	onAction(afterPackageInstallationAction_);
}

}
