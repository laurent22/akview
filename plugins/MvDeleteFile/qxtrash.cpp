#include "qxtrash.h"

#include <QDebug>

#ifdef Q_OS_MAC
#include <QByteArray>
#include <QProcess>
#include <QStringList>
#endif

#ifdef Q_OS_WIN
// Note: Also add relevant lib to .pro file:
// LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0A/Lib/" -lshell32
#include <Windows.h>
#include <ShellAPI.h>
#include <string>
#endif // Q_OS_WIN32

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
#elif defined(Q_OS_WIN)
    std::wstring path = filePath.toStdWString();
    path.append(1, L'\0'); // path string must be double nul-terminated

    SHFILEOPSTRUCT shfos = {};
    shfos.hwnd = nullptr; // handle to window that will own generated windows, if applicable
    shfos.wFunc = FO_DELETE;
    shfos.pFrom = path.c_str();
    shfos.pTo = nullptr; // not used for deletion operations
    shfos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI; // use the recycle bin and display no confirmation box or error messages

    const int retVal = SHFileOperation(&shfos);
    if (retVal != 0) {
       if (shfos.fAnyOperationsAborted) {
          // ...but that's because the user canceled.
       } else {
         qWarning() << QString("error when deleting file %1").arg(filePath);
         return false;
       }
    }
#else
    Q_UNUSED(filePath);
	qWarning() << "Platform not supported";
    return false;
#endif

	return true;
}
