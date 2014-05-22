#include "application.h"
#include <QDebug>
#include <QUrl>

// TODO: move opening of files to C++ side
// TODO: plugin versioning

Application::Application(int &argc, char **argv, int applicationFlags) : QGuiApplication(argc, argv, applicationFlags) {

}

void Application::initialize() {
	engine_ = new QQmlApplicationEngine();
	engine_->load(QUrl(QStringLiteral("qrc:///main.qml")));

	pluginManager_ = new PluginManager(dynamic_cast<IApplication*>(this));
	pluginManager_->loadPlugins("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Debug");

//#ifdef QT_DEBUG
//	QPluginLoader pluginLoader("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Debug/libechoplugin_debug.dylib");
//#else
//	QPluginLoader pluginLoader("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_2_1-Release/libechoplugin.dylib");
//#endif

	setImageSource(QUrl("file:///Users/laurent/Desktop/CH_12_05_2014.jpg"));

	QObject::connect(this->qmlRootObject(), SIGNAL(keypressed(int)), this, SLOT(mainWindow_keypressed(int)));
	QObject::connect(this->qmlRootObject(), SIGNAL(sourceSelected(QString)), this, SLOT(mainWindow_sourceSelected(QString)));
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

void Application::mainWindow_keypressed(int key) {
	KeypressedEvent event;
	event.keyCode = key;
	pluginManager_->onKeypressed(event);
}

void Application::mainWindow_sourceSelected(QString source) {
	this->setImageSource(source);
}

void Application::onImageSourceChange() {
	qmlImage()->setProperty("source", imageSource_);
}

QStringList Application::supportedFileExtensions() {
	QStringList output;
	output << "jpg" << "jpeg" << "png" << "gif" << "bmp" << "tif" << "tiff";
	return output;
}
