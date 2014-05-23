#include <QCommandLineParser>
#include <QDebug>
#include <QFileDialog>
#include <QFileOpenEvent>
#include <QUrl>

#include "application.h"
#include "constants.h"
#include "paths.h"
#include "settings.h"
#include "version.h"

Application::Application(int &argc, char **argv, int applicationFlags) : QApplication(argc, argv, applicationFlags) {
	Application::setOrganizationName("mv-project");
	Application::setOrganizationDomain("mv-project.org");
	Application::setApplicationName(APPLICATION_TITLE);
}

Application::~Application() {
	onExit();
}

void Application::initialize() {
	QCommandLineParser args;
	args.addPositionalArgument("file", tr("File to open."));
	args.process(*this);

	engine_ = new QQmlApplicationEngine();
	engine_->load(QUrl(QStringLiteral("qrc:///main.qml")));

	setWindowTitle(APPLICATION_TITLE);

	QObject* win = qmlApplicationWindow();
	Settings settings;
	QVariant v;
	settings.beginGroup("applicationWindow");
	v = settings.value("width");
	if (!v.isNull()) win->setProperty("width", v);
	v = settings.value("height");
	if (!v.isNull()) win->setProperty("height", v);
	v = settings.value("x");
	if (!v.isNull()) win->setProperty("x", v);
	v = settings.value("y");
	if (!v.isNull()) win->setProperty("y", v);
	settings.endGroup();

	Paths paths;

	pluginManager_ = new PluginManager(dynamic_cast<IApplication*>(this));
	pluginManager_->loadPlugins(paths.pluginFolder());

	QObject::connect(this->qmlRootObject(), SIGNAL(keypressed(int, const QString&, int)), this, SLOT(mainWindow_keypressed(int, const QString&, int)));

	QStringList filePaths = args.positionalArguments();
	if (filePaths.size() > 0) setMediaSource(filePaths[0]);

	win->setProperty("visible", true);
}

void Application::setWindowTitle(const QString &title) {
	QString prefix;
#ifdef MV_DEBUG
	prefix = "** DEBUG ** ";
#endif // MV_DEBUG
	qmlApplicationWindow()->setProperty("title", prefix + title + " (" + version::number() + ")");
}

bool Application::event(QEvent *event) {
	switch (event->type()) {

		case QEvent::FileOpen:

			setMediaSource(static_cast<QFileOpenEvent *>(event)->file());
			return true;

		default:

			return QApplication::event(event);

	}
}

Application* Application::instance() {
	Application* application = static_cast<Application*>(QGuiApplication::instance());
	return application;
}

QString Application::mediaSource() const {
	return mediaSource_;
}

void Application::setMediaSource(const QString &source) {
	if (source == mediaSource_) return;
	mediaSource_ = source;
	onMediaSourceChange();
}

QObject* Application::qmlRootObject() const {
	return engine_->rootObjects().first();
}

QObject* Application::qmlImage() const {
	return qmlRootObject()->findChild<QObject*>("image");
}

QObject* Application::qmlApplicationWindow() const {
	return qmlRootObject();
}

void Application::mainWindow_keypressed(int key, const QString& text, int modifiers) {
	KeypressedEvent event;
	event.keyCode = key;
	event.text = text;
	event.modifiers = modifiers;

	if (event.keyCode == Qt::Key_O && event.modifiers == Qt::ControlModifier) {
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
		setMediaSource(filePath);
		settings.setValue("lastOpenFileDirectory", QVariant(QFileInfo(filePath).absolutePath()));
		return;
	}

	if (event.keyCode == Qt::Key_W && event.modifiers == Qt::ControlModifier) {
		quit();
		return;
	}

	// Keyboard events that are not handled by the application are sent to the plugins
	pluginManager_->onKeypressed(event);
}

void Application::onMediaSourceChange() {
	qmlImage()->setProperty("source", QUrl("file://" + mediaSource_));
	setWindowTitle(QFileInfo(mediaSource_).fileName());
}

void Application::onExit() {
	QObject* win = qmlApplicationWindow();
	Settings settings;

	settings.beginGroup("applicationWindow");
	settings.setValue("width", win->property("width"));
	settings.setValue("height", win->property("height"));
	settings.setValue("x", win->property("x"));
	settings.setValue("y", win->property("y"));
	settings.endGroup();
}

QStringList Application::supportedFileExtensions() {
	QStringList output;
	output << "jpg" << "jpeg" << "png" << "gif" << "bmp" << "tif" << "tiff";
	return output;
}
