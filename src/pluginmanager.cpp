#include "action.h"
#include "application.h"
#include "messageboxes.h"
#include "pluginmanager.h"

#include "jsapi/jsapi_application.h"
#include "jsapi/jsapi_console.h"
#include "jsapi/jsapi_fileinfo.h"
#include "jsapi/jsapi_imaging.h"
#include "jsapi/jsapi_input.h"
#include "jsapi/jsapi_plugin.h"
#include "jsapi/jsapi_ui.h"
#include "jsapi/jsapi_system.h"

namespace mv {

PluginManager::PluginManager() {
	scriptEngine_ = NULL;
	actionThread_ = NULL;
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

void PluginManager::execAction(const QString& actionName, const QStringList& filePaths) {
	Application* app = Application::instance();

	for (unsigned int i = 0; i < plugins_.size(); i++) {
		Plugin* plugin = plugins_[i];
		Action* action = plugin->findAction(actionName);
		if (!action) continue;

		PackageManager* packageManager = app->packageManager();

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

			app->mainWindow()->showConsole();

			afterPackageInstallationAction_ = actionName;
			afterPackageInstallationFilePaths_ = filePaths;
			connect(packageManager, SIGNAL(installationDone()), this, SLOT(packageManager_installationDone()));
			packageManager->install(missingPackages);
			return;
		}

		if (!scriptEngine_) {
			scriptEngine_ = new QScriptEngine();
			QObject* jsApplication = new jsapi::Application(scriptEngine_);
			jsConsole_ = new jsapi::Console();
			QObject* jsFileInfo = new jsapi::FileInfo();
			jsImaging_ = new jsapi::Imaging(scriptEngine_);
			QObject* jsUi = new jsapi::Ui(scriptEngine_);
			QObject* jsSystem = new jsapi::System(scriptEngine_);
			scriptEngine_->globalObject().setProperty("application", scriptEngine_->newQObject(jsApplication));
			scriptEngine_->globalObject().setProperty("console", scriptEngine_->newQObject(jsConsole_));
			scriptEngine_->globalObject().setProperty("fileinfo", scriptEngine_->newQObject(jsFileInfo));
			scriptEngine_->globalObject().setProperty("imaging", scriptEngine_->newQObject(jsImaging_));
			scriptEngine_->globalObject().setProperty("ui", scriptEngine_->newQObject(jsUi));
			scriptEngine_->globalObject().setProperty("system", scriptEngine_->newQObject(jsSystem));
		}

		jsapi::Console* c = (jsapi::Console*)jsConsole_;
		c->saveVScrollValue(app->mainWindow()->console()->documentSize().height());

		QPixmap* pixmap = app->mainWindow()->pixmap();
		QObject* jsInput = new jsapi::Input(
			scriptEngine_,
			filePaths, app->mainWindow()->selectionRect(),
			pixmap ? pixmap->size() : QSize()
		);
		scriptEngine_->globalObject().setProperty("input", scriptEngine_->newQObject(jsInput));

		QObject* jsPlugin = new jsapi::Plugin(scriptEngine_, plugin, action);
		scriptEngine_->globalObject().setProperty("plugin", scriptEngine_->newQObject(jsPlugin));

		QString scriptFilePath = plugin->actionScriptFilePath(action->id());
		QFile scriptFile(scriptFilePath);
		if (!scriptFile.open(QIODevice::ReadOnly)) {
			qWarning() << "Cannot open script file:" << scriptFilePath;
			return;
		}

		QTextStream stream(&scriptFile);
		QString contents = stream.readAll();
		scriptFile.close();

		actionThread_ = new ActionThread(scriptEngine_, contents, scriptFilePath);
		connect(actionThread_, SIGNAL(completed()), this, SLOT(actionThread_completed()));
		actionThread_->start();

		return;
	}
}

void PluginManager::actionThread_completed() {
	QScriptValue errorValue = scriptEngine_->uncaughtException();
	if (errorValue.isValid()) {
		qWarning() << qPrintable(QString("%1 at line %2").arg(errorValue.toString()).arg(scriptEngine_->uncaughtExceptionLineNumber()));
		QStringList backtrace = scriptEngine_->uncaughtExceptionBacktrace();
		for (int i = 0; i < backtrace.size(); i++) {
			qDebug() << qPrintable("    " + backtrace[i]);
		}
	}

	delete actionThread_;
	actionThread_ = NULL;
}

void PluginManager::packageManager_installationDone() {
	PackageManager* packageManager = Application::instance()->packageManager();
	disconnect(packageManager, SIGNAL(installationDone()), this, SLOT(packageManager_installationDone()));
	execAction(afterPackageInstallationAction_, afterPackageInstallationFilePaths_);
}

}
