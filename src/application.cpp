#include "application.h"
#include <QDebug>
#include <QUrl>

Application::Application(int &argc, char **argv, int applicationFlags) : QGuiApplication(argc, argv, applicationFlags) {

}

void Application::initialize() {
	engine_ = new QQmlApplicationEngine();
	engine_->load(QUrl(QStringLiteral("qrc:///main.qml")));

	QPluginLoader pluginLoader("/Users/laurent/Docs/PROGS/C++/mv/plugins/build-MvBrowserPlugin-Qt_5_3_1-Debug/libechoplugin_debug.dylib");
	QObject *plugin = pluginLoader.instance();
	if (plugin) {
		MvPluginInterface *mvPlugin;
		mvPlugin = qobject_cast<MvPluginInterface *>(plugin);
		mvPlugin->onInitialize(dynamic_cast<IApplication*>(this));
		plugins_.push_back(mvPlugin);
		//qDebug() << mvPlugin->description();
	}

	setImageSource(QUrl("file:///Users/laurent/Desktop/CH_12_05_2014.jpg"));

	QObject::connect(this->qmlRootObject(), SIGNAL(keypressed(int)), this, SLOT(mainWindow_keypressed(int)));
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
	this->setImageSource(QUrl("file://" + source));
}

QObject* Application::qmlRootObject() const {
	return engine_->rootObjects().first();
}

QObject* Application::qmlImage() const {
	return qmlRootObject()->findChild<QObject*>("image");
}

void Application::mainWindow_keypressed(int key) {
	for (int i = 0; i < plugins_.size(); i++) {
		MvPluginInterface* plugin = plugins_[i];
		KeypressedEvent event;
		event.keyCode = key;
		plugin->onKeypressed(event);
	}
}

void Application::onImageSourceChange() {
	qmlImage()->setProperty("source", imageSource_);
}
