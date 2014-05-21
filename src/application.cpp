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
		qDebug() << mvPlugin->description();
	}

	setImageSource("https://upload.wikimedia.org/wikipedia/commons/thumb/b/b6/SIPI_Jelly_Beans_4.1.07.tiff/lossy-page1-256px-SIPI_Jelly_Beans_4.1.07.tiff.jpg");
}

Application* Application::instance() {
	Application* application = static_cast<Application*>(QGuiApplication::instance());
	return application;
}

void Application::testing() {
	qDebug() << "AAAAAAAAAAAAAAAAAAA";
}

QString Application::imageSource() const {
	return imageSource_;
}

void Application::setImageSource(const QString &source) {
	if (source == imageSource_) return;
	imageSource_ = source;
	onImageSourceChange();
}

QObject* Application::qmlRootObject() const {
	return engine_->rootObjects().first();
}

QObject* Application::qmlImage() const {
	return qmlRootObject()->findChild<QObject*>("image");
}

void Application::onImageSourceChange() {
	qmlImage()->setProperty("source", imageSource_);
}
