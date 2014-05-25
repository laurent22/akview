#include <QDebug>
#include <QHash>
#include <QByteArray>

#include <FreeImage.h>

#include "mvjpegtoolsplugin.h"
#include "dialog.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QtGui>

//void myMessageHandler(QtMsgType type, const char *msg)
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
	QString txt;
	switch (type) {
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
	break;
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
	break;
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
	break;
	}
	QFile outFile("/Users/laurent/mv-jt.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	qWarning() << "FreeImage error:" << FreeImage_GetFormatFromFIF(fif) << message;
}

void MvJpegToolsPlugin::onKeypressed(mv::KeypressedEvent& event) {
	qInstallMessageHandler(myMessageHandler);

	if (event.keyCode == Qt::Key_J && event.modifiers == Qt::ControlModifier) {
		event.accepted = true;

		QHash<QString, QVariant> settings = application->settings()->value("plugins/jpegtools/dialogsettings").toHash();

		Dialog dialog;
		dialog.loadSettings(settings);
		dialog.setModal(true);
		dialog.exec();

		// TODO: handle cancel button

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
				QString sourcePath = application->mediaSource();
				bool ok = FreeImage_JPEGTransform(sourcePath.toStdString().c_str(), sourcePath.toStdString().c_str(), op, 0);
				if (ok) application->reloadSource();
			}
		}
	}
}
