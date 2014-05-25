#include <QDebug>
#include <QHash>
#include <QByteArray>

#include <FreeImage.h>

#include "mvjpegtoolsplugin.h"
#include "dialog.h"

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	qWarning() << "FreeImage error:" << FreeImage_GetFormatFromFIF(fif) << message;
}

void MvJpegToolsPlugin::onAction(const QString &action) {
	if (action != "jpeg_tools") return;

	QHash<QString, QVariant> settings = application->settings()->value("plugins/jpegtools/dialogsettings").toHash();

	Dialog dialog;
	dialog.loadSettings(settings);
	dialog.setModal(true);
	int result = dialog.exec();
	if (result == QDialog::Rejected) return;

	settings = dialog.settings();
	application->settings()->setValue("plugins/jpegtools/dialogsettings", settings);

	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	if (settings.contains("losslessRotation")) {
		int r = settings.value("losslessRotation").toInt();
		// TODO: use FreeImage_JPEGTransformU for Windows? Test unicode file paths.
		FREE_IMAGE_JPEG_OPERATION op = FIJPEG_OP_NONE;
		if (r == 90) op = FIJPEG_OP_ROTATE_90;
		if (r == 180) op = FIJPEG_OP_ROTATE_180;
		if (r == 270) op = FIJPEG_OP_ROTATE_270;
		if (op != FIJPEG_OP_NONE) {
			QString sourcePath = application->source();
			bool ok = FreeImage_JPEGTransform(sourcePath.toStdString().c_str(), sourcePath.toStdString().c_str(), op, 0);
			if (ok) application->reloadSource();
		}
	}
}
