#include "qxtrash.h"

#include <CoreFoundation/CFError.h>
#include <CoreServices/CoreServices.h>
#include <AppKit/NSWorkspace.h>
#include <Foundation/NSString.h>
#include <QFileInfo>

static void notifyFinder(const QString &trash) {
	QByteArray utf8TrashPath = trash.toUtf8();
	NSString *trashPath = [[NSString alloc] initWithBytes:utf8TrashPath.data()
		length:utf8TrashPath.length()
		encoding:NSUTF8StringEncoding];
	[[NSWorkspace sharedWorkspace] noteFileSystemChanged:trashPath];
	[trashPath release];
}

bool QxTrash::moveToTrash(const QString &filePath) {
	QByteArray pathUtf8 = filePath.toUtf8();
	char *newPathUtf8 = 0;
	OSStatus result = FSPathMoveObjectToTrashSync(pathUtf8.constData(), &newPathUtf8, kFSFileOperationDoNotMoveAcrossVolumes);
	QString newPath = QString::fromUtf8(newPathUtf8);
	free(newPathUtf8);
	notifyFinder(QFileInfo(newPath).path());
	return result == noErr;
}
