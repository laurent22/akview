#include <QDebug>

#include <QProcess>

#if defined(Q_OS_WIN)
#include <QDir>
#include <QFileInfo>
#endif

#include "mvrevealplugin.h"

// TODO: test/support Windows
// TODO: test/support Linux

// Adapted from http://stackoverflow.com/a/3546503/561309
bool MvRevealPlugin::revealFile(const QString &pathIn) {
	// Mac, Windows support folder or file.
	#if defined(Q_OS_WIN)
	const QString explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
	if (explorer.isEmpty()) {
		qWarning() << "Launching Windows Explorer failed: Could not find explorer.exe in path to launch Windows Explorer.";
		return false;
	}
	QString param;
	if (!QFileInfo(pathIn).isDir())
		param = QLatin1String("/select,");
	param += QDir::toNativeSeparators(pathIn);
	QProcess::startDetached(explorer, QStringList(param));
	#elif defined(Q_OS_MAC)
	bool ok = application->runAppleScript(QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"").arg(pathIn));
	if (ok) application->runAppleScript(QLatin1String("tell application \"Finder\" to activate"));
	#else
	qWarning() << "Not supported";
	return false;

//	// we cannot select a file here, because no file browser really supports it...
//	const QFileInfo fileInfo(pathIn);
//	const QString folder = fileInfo.absoluteFilePath();
//	const QString app = Utils::UnixUtils::fileBrowser(Core::ICore::instance()->settings());
//	QProcess browserProc;
//	const QString browserArgs = Utils::UnixUtils::substituteFileBrowserParameters(app, folder);
//	bool success = browserProc.startDetached(browserArgs);
//	const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
//	success = success && error.isEmpty();
//	if (!success) {
//		qWarning() << "Error:" << error;
//		return false;
	#endif

	return true;
}

void MvRevealPlugin::onAction(const QString &action) {
	if (action != "reveal") return;

	QString source = application->source();
	if (source == "") return;

	revealFile(source);
}
