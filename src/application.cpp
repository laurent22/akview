#include <QCommandLineParser>
#include <QDebug>
#include <QFileDialog>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>

#include "application.h"
#include "constants.h"
#include "paths.h"
#include "settings.h"
#include "version.h"

#include <QMenuBar>

namespace mv {

Application::Application(int &argc, char **argv, int applicationFlags) : QApplication(argc, argv, applicationFlags) {
	settings_ = NULL;
	preferencesDialog_ = NULL;
	menuBar_ = NULL;

	Application::setOrganizationName(VER_COMPANYNAME_STR);
	Application::setOrganizationDomain(VER_DOMAIN_STR);
	Application::setApplicationName(APPLICATION_TITLE);
}

Application::~Application() {
	onExit();
}

void Application::initialize() {
	QCommandLineParser args;
	args.addPositionalArgument("file", tr("File to open."));
	args.process(*this);

	mainWindow_ = new MainWindow();
	mainWindow_->show();

	setWindowTitle(APPLICATION_TITLE);
	loadWindowGeometry();

	Paths paths;

	pluginManager_ = new PluginManager(dynamic_cast<IApplication*>(this));
	pluginManager_->loadPlugins(paths.pluginFolder());

	connect(mainWindow_, SIGNAL(keypressed(QKeyEvent*)), this, SLOT(mainWindow_keypressed(QKeyEvent*)));

	QStringList filePaths = args.positionalArguments();
	if (filePaths.size() > 0) setSource(filePaths[0]);

	buildMenu();
}

void Application::buildMenu() {
	QMenu* fileMenu = new QMenu(tr("File"));
	QMenu* viewMenu = new QMenu(tr("View"));
	QMenu* toolsMenu = new QMenu(tr("Tools"));
	QMenu* helpMenu = new QMenu(tr("Help"));

	menus_["File"] = fileMenu;
	menus_["View"] = viewMenu;
	menus_["Tools"] = toolsMenu;
	menus_["Help"] = helpMenu;

	addAction("open_file", tr("Open a file..."), "File", QKeySequence(Qt::CTRL + Qt::Key_O));
	addAction("close_window", tr("Close window"), "File", QKeySequence(Qt::CTRL + Qt::Key_W));
	addAction("next", tr("Next"), "View", QKeySequence(Qt::Key_Right), QKeySequence("Num+Right"));
	addAction("previous", tr("Previous"), "View", QKeySequence(Qt::Key_Left), QKeySequence("Num+Left"));
	addAction("about", tr("About"), "Help");
	addAction("preferences", tr("Preferences"), "Tools");

	PluginVector plugins = pluginManager()->plugins();
	for (unsigned int i = 0; i < plugins.size(); i++) {
		Plugin* plugin = plugins[i];
		for (unsigned int j = 0; j < plugin->actions().size(); j++) {
			Action* action = plugin->actions()[j];
			if (action->menu() == "") continue;
			if (action->shortcuts().size() > 1) {
				addAction(action->name(), action->text(), action->menu(), action->shortcuts()[0], action->shortcuts()[1]);
			} else {
				addAction(action->name(), action->text(), action->menu(), action->shortcut());
			}
		}
	}	

	menuBar_ = new QMenuBar(mainWindow_);

	for(QStringQMenuMap::const_iterator i = menus_.begin(); i != menus_.end(); ++i) {
		menuBar_->addMenu(i->second);
	}
}

Action* Application::addAction(const QString& name, const QString& text, const QString& menu, const QKeySequence& shortcut) {
	return addAction(name, text, menu, shortcut, QKeySequence());
}

Action* Application::addAction(const QString& name, const QString& text, const QString& menu, const QKeySequence& shortcut1, const QKeySequence& shortcut2) {
	Action* action = new Action();
	action->setName(name);
	action->setText(text);

	if (!shortcut1.isEmpty() && !shortcut2.isEmpty()) {
		QList<QKeySequence> shortcuts;
		shortcuts << shortcut1 << shortcut2;
		action->setShortcuts(shortcuts);
	} else if (!shortcut1.isEmpty()) {
		action->setShortcut(shortcut1);
	}

	connect(action, SIGNAL(triggered()), this, SLOT(mainWindow_actionTriggered()));

	actions_.push_back(action);

	if (menus_.find(menu) != menus_.end()) {
		menus_[menu]->addAction(action);
	} else {
		QMenu* menuObject = new QMenu(menu);
		menuObject->addAction(action);
		menus_[menu] = menuObject;
	}

	return action;
}

void Application::setWindowTitle(const QString &title) {
	QString prefix;

#ifdef MV_DEBUG
	prefix = "** DEBUG ** ";
#endif // MV_DEBUG

#ifdef QT_DEBUG
	prefix = "** DEBUG ** ";
#endif // QT_DEBUG

	mainWindow_->setWindowTitle(prefix + title);
}

void Application::showPreferencesDialog() {
	if (!preferencesDialog_) {
		preferencesDialog_ = new PreferencesDialog(NULL);
		preferencesDialog_->setModal(true);
	}
	preferencesDialog_->exec();
}

PluginManager *Application::pluginManager() const {
	return pluginManager_;
}

ActionVector Application::actions() const {
	ActionVector output;

	output.insert(output.end(), actions_.begin(), actions_.end());

	PluginVector plugins = pluginManager()->plugins();
	for (unsigned int i = 0; i < plugins.size(); i++) {
		Plugin* plugin = plugins[i];
		ActionVector pluginActions = plugin->actions();
		output.insert(output.end(), pluginActions.begin(), pluginActions.end());
	}

	return output;
}

bool Application::actionShortcutIsOverridden(const QString& actionName) const {
	Settings settings;
	QVariant v = settings.value("shortcuts/" + actionName);
	return !v.isNull();
}

QString Application::shortcutAction(const QKeySequence& shortcut) const {
	Settings settings;
	settings.beginGroup("shortcuts");
	QStringList keys = settings.childKeys();
	QStringList noopActions;

	for (int i = 0; i < keys.size(); i++) {
		QString key = keys[i];
		QString value = settings.value(key).toString();
		QKeySequence kv(value);
		if (kv.matches(shortcut) == QKeySequence::ExactMatch) {
			return key;
		} else if (value == "") {
			noopActions << key;
		}
	}

	ActionVector actions = this->actions();
	for (unsigned int i = 0; i < actions.size(); i++) {
		Action* a = actions[i];
		if (a->supports(shortcut)) {
			// Now also check if the shortcut has been overridden by a blank shortcut (which means
			// this action cannot be started via a shortcut)
			if (noopActions.contains(a->name())) return "";
			return a->name();
		}
	}

	settings.endGroup();

	return "";
}

QKeySequence Application::actionShortcut(const QString &actionName) const {
	ActionVector actions = this->actions();
	Action* action = NULL;
	for (unsigned int i = 0; i < actions.size(); i++) {
		Action* a = actions[i];
		if (a->name() == actionName) {
			action = a;
			break;
		}
	}

	if (!action) {
		QKeySequence output;
		return output;
	}

	Settings settings;
	QVariant v = settings.value("shortcuts/" + actionName);
	if (v.isNull()) return action->shortcut();

	QKeySequence output(v.toString());
	return output;
}

bool Application::event(QEvent *event) {
	switch (event->type()) {

		case QEvent::FileOpen: {

			QString filePath = static_cast<QFileOpenEvent*>(event)->file();

			if (QFileInfo(filePath).isDir()) {
				QStringList sources = this->sources(filePath);
				if (!sources.size()) return true;
				setSource(sources[0]);
			} else {
				setSource(filePath);
			}
			return true;

		}

		default: {

			return QApplication::event(event);

		}

	}
}

Application* Application::instance() {
	Application* application = static_cast<Application*>(QGuiApplication::instance());
	return application;
}

QString Application::source() const {
	return source_;
}

void Application::setSource(const QString &source) {
	if (source == source_) return;
	source_ = source;
	onMediaSourceChange();
}

void Application::execAction(const QString& actionName) {
	if (actionName == "") return;

	if (actionName == "open_file") {
		QString filter;
		QStringList extensions = supportedFileExtensions();
		for (int i = 0; i < extensions.size(); i++) {
			QString e = extensions[i];
			if (filter != "") filter += " ";
			filter += "*." + e;
		}
		Settings settings;
		QString lastDir = settings.value("lastOpenFileDirectory").toString();
		QString filePath = QFileDialog::getOpenFileName(NULL, tr("Open File"), lastDir, tr("Supported Files (%1)").arg(filter));
		if (filePath != "") {
			setSource(filePath);
			settings.setValue("lastOpenFileDirectory", QVariant(QFileInfo(filePath).absolutePath()));
		}
		return;
	}

	if (actionName == "close_window") {
		quit();
		return;
	}

	if (actionName == "previous") {
		previousSource();
		return;
	}

	if (actionName == "next") {
		nextSource();
		return;
	}

	pluginManager_->onAction(actionName);
}

void Application::mainWindow_keypressed(QKeyEvent* event) {
	QKeySequence ks(event->modifiers() + event->key());
	QString actionName = shortcutAction(ks);
	execAction(actionName);	
}

void Application::mainWindow_actionTriggered() {
	Action* action = dynamic_cast<Action*>(sender());
	QString name = action->name();

	if (name == "about") {
		QMessageBox::about(NULL, tr("About %1").arg(APPLICATION_TITLE), tr("%1 %2").arg(APPLICATION_TITLE).arg(version::number()));
		return;
	}

	if (name == "preferences") {
		showPreferencesDialog();
		return;
	}

	execAction(name);
}

void Application::onMediaSourceChange() {
	mainWindow_->setSource(source_);
	setWindowTitle(QFileInfo(source_).fileName());
}

void Application::saveWindowGeometry() {
	if (!mainWindow_) return;

	Settings settings;
	settings.beginGroup("applicationWindow");
	settings.setValue("width", mainWindow_->size().width());
	settings.setValue("height", mainWindow_->size().height());
	settings.setValue("x", mainWindow_->x());
	settings.setValue("y", mainWindow_->y());
	settings.endGroup();
}

void Application::loadWindowGeometry() {
	Settings settings;
	QVariant v;
	int windowX = 0;
	int windowY = 0;
	int windowWidth = 800;
	int windowHeight = 600;
	settings.beginGroup("applicationWindow");
	v = settings.value("width");
	if (!v.isNull()) windowWidth = v.toInt();
	v = settings.value("height");
	if (!v.isNull()) windowHeight = v.toInt();
	v = settings.value("x");
	if (!v.isNull()) windowX = v.toInt();
	v = settings.value("y");
	if (!v.isNull()) windowY = v.toInt();
	settings.endGroup();

	mainWindow_->move(windowX, windowY);
	mainWindow_->resize(windowWidth, windowHeight);
}

void Application::onExit() {
	saveWindowGeometry();
}

QStringList Application::supportedFileExtensions() const {
	QStringList output;
	output << "jpg" << "jpeg" << "png" << "gif" << "bmp" << "tif" << "tiff";
	return output;
}

void Application::playLoopAnimation() {
	return;

	// QObject* loopImage = qmlRootObject()->findChild<QObject*>("loopImage");
	// if (!loopImage) return;

	// loopImage->setProperty("rotation", 0);
	// loopImage->setProperty("opacity", 0);

	// QObject* loopAnimation = qmlRootObject()->findChild<QObject*>("loopImageShowAnimation");
	// if (loopAnimation) {
	// 	QMetaObject::invokeMethod(loopAnimation, "stop");
	// 	QMetaObject::invokeMethod(loopAnimation, "start");
	// }

	// QObject* rotateAnimation = qmlRootObject()->findChild<QObject*>("loopImageRotateAnimation");
	// if (rotateAnimation) {
	// 	QMetaObject::invokeMethod(rotateAnimation, "stop");
	// 	QMetaObject::invokeMethod(rotateAnimation, "start");
	// }
}

void Application::setSourceIndex(int index) {
	QStringList sources = this->sources();
	if (!sources.size()) return;

	QString source = sources[index];
	sourceIndex_ = index;

	setSource(source);
}

void Application::nextSource() {
	int index = sourceIndex();
	QStringList sources = this->sources();
	index++;
	if (index >= sources.size()) {
		index = 0;
		playLoopAnimation();
	}
	setSourceIndex(index);
}

void Application::previousSource() {
	int index = sourceIndex();
	QStringList sources = this->sources();
	index--;
	if (index < 0) {
		index = sources.size() - 1;
		playLoopAnimation();
	}
	setSourceIndex(index);
}

int Application::sourceIndex() const {
	QString source = this->source();

	if (QFileInfo(source).dir().absolutePath() != sourceDir_) {
		// Current dir has changed - reload source list
		sourceIndex_ = -1;
		sources_.clear();
	}

	QStringList sources = this->sources();
	if (!sources.size()) return -1;

	source = QFileInfo(source).fileName();

	// Check if the index we have is correct
	if (sourceIndex_ >= 0 && sourceIndex_ < sources.size() && sources[sourceIndex_] == source) return sourceIndex_;

	// If it's not correct, try to get it from the current source and source list
	sourceIndex_ = -1;

	for (int i = 0; i < sources.size(); i++) {
		if (QFileInfo(sources[i]).fileName() == source) {
			sourceIndex_ = i;
			break;
		}
	}
	return sourceIndex_;
}

void Application::refreshSources() {
	sources_.clear();
	sourceIndex_ = -1;
}

void Application::reloadSource() const {
	mainWindow_->reloadSource();
}

void Application::exifClearOrientation(const QString &filePath) {
	JheadHandler handler(filePath);
	handler.clearOrientation();
}

bool Application::runAppleScript(const QString &script) {
	QStringList scriptArgs;
	scriptArgs << QLatin1String("-e") << script;
	int exitCode = QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);

	if (exitCode == -2) {
		qWarning() << "runAppleScript: the process could not be started:" << script;
		return false;
	}

	if (exitCode == -1) {
		qWarning() << "runAppleScript: the process crashed:" << script;
		return false;
	}

	if (exitCode > 0) {
		qWarning() << "runAppleScript: the process exited with error code " << exitCode;
		return false;
	}

	return true;
}

Settings *Application::settings() const {
	if (settings_) return settings_;
	settings_ = new Settings();
	return settings_;
}

QStringList Application::sources() const {
	return sources(source());
}

QStringList Application::sources(const QString& filePath) const {
	if (sources_.length()) return sources_;

	QStringList supportedFileExtensions = this->supportedFileExtensions();
	sourceIndex_ = -1;

	QFileInfo fileInfo(filePath);
	QDir dir;
	if (fileInfo.isDir()) {
		dir.setPath(filePath);
	} else {
		dir = fileInfo.dir();
	}

	sourceDir_ = dir.absolutePath();
	QFileInfoList files = dir.entryInfoList(QDir::Files, QDir::LocaleAware);
	for (int i = 0; i < files.size(); i++) {
		if (!supportedFileExtensions.contains(files[i].suffix().toLower())) continue;
		sources_.append(files[i].absoluteFilePath());
	}

	return sources_;
}

}
