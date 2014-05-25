#include "qxtrash.h"

#include <QDebug>

// Note: Also add relevant lib to .pro file:
// LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0A/Lib/" -lshell32
#include <Windows.h>
#include <ShellAPI.h>
#include <string>

bool QxTrash::moveToTrash(const QString &filePath) {
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

	return true;
}
