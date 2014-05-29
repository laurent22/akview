#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QAction>
#include <QJsonObject>

#include "mvplugininterface.h"
#include "pluginevents.h"
#include "simpletypes.h"

namespace mv {

class PluginAction: QAction {

public:

	PluginAction(const QJsonObject& jsonObject);
	bool supports(const KeypressedEvent& event) const;
	QString name() const;

private:

	QJsonObject jsonObject_;
	QString name_;

};

typedef std::vector<PluginAction*> PluginActionVector;

class Plugin {

public:

	Plugin(IApplication* application, const QString &pluginFilePath);
	bool loadMetadata();
	bool loadInterface();
	MvPluginInterface* interface() const;
	QString errorMessage() const;
	QString description() const;
	QString version() const;
	QString compatibilityMinVersion() const;
	QString compatibilityMaxVersion() const;
	PluginActionVector actions() const;
	bool supports(const KeypressedEvent& event) const;
	bool interfaceLoaded() const;
	PluginAction *findAction(const KeypressedEvent &event) const;

private:

	QString metadataFilePath(const QString &pluginFilePath) const;
	QString errorMessage_;
	MvPluginInterface* interface_;
	IApplication* application_;
	QString pluginFilePath_;
	QJsonObject metadata_;
	PluginActionVector actions_;

};

typedef std::vector<Plugin*> PluginVector;

class PluginManager {

public:

	PluginManager(IApplication* application);
	bool loadPlugin(const QString& filePath);
	void loadPlugins(const QString& folderPath);

	void onKeypressed(const KeypressedEvent& event);

private:

	PluginVector plugins_;
	IApplication* application_;

};

}

#endif // PLUGINMANAGER_H
