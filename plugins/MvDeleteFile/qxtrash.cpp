#include "qxtrash.h"

#ifdef Q_OS_WIN32
#include <Windows.h>
#include <ShellAPI.h>
#endif // Q_OS_WIN32

#include <QByteArray>
#include <QDebug>
#include <QProcess>
#include <QStringList>

bool QxTrash::moveToTrash(const QString &filePath) {
#ifdef Q_OS_MAC
	QString aScript = "tell application \"Finder\" to move POSIX file \"" + filePath + "\" to trash";
	QString osascript = "/usr/bin/osascript";
	QStringList processArguments;
	processArguments << "-l" << "AppleScript";

	QProcess p;
	p.start(osascript, processArguments);
	p.write(aScript.toUtf8());
	p.closeWriteChannel();
	p.waitForFinished();
	QByteArray result = p.readAll();

	if (p.exitStatus() == QProcess::CrashExit || p.exitCode() != 0) {
		QString resultAsString(result);
		qWarning() << QString("Exit code %1 when deleting file %2: %3").arg(QString::number(p.exitCode()), filePath, resultAsString);
		return false;
	}
#elif Q_OS_WIN32

#else
	qWarning() << "Platform not supported";
#endif

	return true;
}
