#include <QDebug>
#include <QFileDialog>
#include <settings.h>
#include <QUrl>

#include "application.h"
#include "constants.h"

// TODO: plugin versioning
// TODO: save window dimensions

Application::Application(int &argc, char **argv, int applicationFlags) : QApplication(argc, argv, applicationFlags) {
	Application::setOrganizationName("mv-project");
	Application::setOrganizationDomain("mv-project.org");
	Application::setApplicationName(APPLICATION_TITLE);
}

Application::~Application() {
	onExit();
}

void Application::initialize() {
	engine_ = new QQmlApplicationEngine();
	engine_->load(QUrl(QStringLiteral("qrc:///main.qml")));

	qmlApplicationWindow()->setProperty("title", APPLICATION_TITLE);

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

	pluginManager_ = new PluginManager(dynamic_cast<IApplication*>(this));
	pluginManager_->loadPlugins("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Debug");

//#ifdef QT_DEBUG
//	QPluginLoader pluginLoader("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Debug/libechoplugin_debug.dylib");
//#else
//	QPluginLoader pluginLoader("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Release/libechoplugin.dylib");
//#endif

	setImageSource(QUrl("file:///Users/laurent/Desktop/CH_12_05_2014.jpg"));

	QObject::connect(this->qmlRootObject(), SIGNAL(keypressed(int, const QString&, int)), this, SLOT(mainWindow_keypressed(int, const QString&, int)));
	QObject::connect(this->qmlRootObject(), SIGNAL(sourceSelected(QString)), this, SLOT(mainWindow_sourceSelected(QString)));

	win->setProperty("visible", true);
}

Application* Application::instance() {
	Application* application = static_cast<Application*>(QGuiApplication::instance());
	return application;
}

QUrl Application::imageSource() const {
	return imageSource_;
}

void Application::setImageSource(const QUrl &source) {
	if (source == imageSource_) return;
	imageSource_ = source;
	onImageSourceChange();
}

void Application::setImageSource(const QString &source) {
	// TODO: handle URLs
	if (source.left(7) == "file://") {
		this->setImageSource(QUrl(source));
	} else {
		this->setImageSource(QUrl("file://" + source));
	}
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
		for (unsigned int i = 0; i < extensions.size(); i++) {
			QString e = extensions[i];
			if (filter != "") filter += " ";
			filter += "*." + e;
		}
		Settings settings;
		QString lastDir = settings.value("lastOpenFileDirectory").toString();
		QString filePath = QFileDialog::getOpenFileName(NULL, tr("Open File"), lastDir, tr("Supported Files (%1)").arg(filter));
		setImageSource(filePath);
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

void Application::mainWindow_sourceSelected(QString source) {
	setImageSource(source);
}

void Application::onImageSourceChange() {
	qmlImage()->setProperty("source", imageSource_);
	qmlApplicationWindow()->setProperty("title", QFileInfo(imageSource_.toLocalFile()).fileName());
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
