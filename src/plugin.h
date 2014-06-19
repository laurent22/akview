#ifndef MV_PLUGIN_H
#define MV_PLUGIN_H

#include "action.h"
#include "pluginevents.h"

namespace mv {

class Plugin {

public:

	Plugin(const QString &pluginFolderPath);
	bool load();
	QString errorMessage() const;
	QString description() const;
	QString version() const;
	QString minEngineVersion() const;
	ActionVector actions() const;
	Action* findAction(const QString& name) const;
	QString actionScriptFilePath(const QString& actionId) const;

private:

	bool loadManifest();
	bool loadActions();

	QString errorMessage_;
	QString pluginFolderPath_;
	QJsonObject manifest_;
	ActionVector actions_;

};

typedef std::vector<Plugin*> PluginVector;

}

#endif