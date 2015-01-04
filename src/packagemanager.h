#ifndef MV_PACKAGEMANAGER_H
#define MV_PACKAGEMANAGER_H

#include "progressbardialog.h"

namespace mv {


class Manager {

public:

	Manager(int id, const QString& name, const QString& url, const QString& command, const QStringList& installCommand) ;
	QStringList installCommand() const;
	int id() const;
	QString name() const;
	QString url() const;
	QString command() const;

private:

	int id_;
	QString name_;
	QString command_;
	QStringList installCommand_;
	QString url_;

};

typedef std::vector<Manager*> ManagerVector;


class PackageManager : public QObject {

	Q_OBJECT

public:

	PackageManager();
	~PackageManager();
	void install(const QString& package);
	void install(const QStringList& packages);
	Manager* selectedManager() const;
	bool managerIsInstalled(int managerId) const;
	bool commandIsInstalled(const QString& command);
	bool installationInProgress() const;

private:

	Manager* managerById(int id) const;
	mutable int selectedManagerId_;
	ManagerVector managers_;
	mutable QStringList installedCommands_;
	ProgressBarDialog* progressBarDialog_;
	QProcess* installProcess_;
	QProcessEnvironment* installProcessEnv_;

public slots:

	void process_readyReadStandardError();
	void process_readyReadStandardOutput();
	void process_finished(int, QProcess::ExitStatus);
	void progressBarDialog_rejected();

signals:

	void installationDone();

};

}

#endif