#ifndef MV_PLUGIN_H
#define MV_PLUGIN_H

#include "action.h"
#include "mvplugininterface.h"
#include "pluginevents.h"

namespace mv {

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
	ActionVector actions() const;
	bool supports(const KeypressedEvent& event) const;
	bool interfaceLoaded() const;
	Action *findAction(const KeypressedEvent& event) const;
	Action *findAction(const QString& name) const;

private:

	QString metadataFilePath(const QString &pluginFilePath) const;
	QString errorMessage_;
	MvPluginInterface* interface_;
	IApplication* application_;
	QString pluginFilePath_;
	QJsonObject metadata_;
	ActionVector actions_;

};

typedef std::vector<Plugin*> PluginVector;

}

#endif