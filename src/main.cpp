#include <QDebug>

#include "application.h"

#include <QByteArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

//#include <FreeImage.h>

//void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
//	qDebug() << FreeImage_GetFormatFromFIF(fif) << message;
//}

int main(int argc, char *argv[]) {
	mv::Application app(argc, argv);
	app.initialize();

//	FreeImage_SetOutputMessage(FreeImageErrorHandler);

//	const char *src_file = "/Users/laurent/Desktop/test/CH_12_05_2014.jpg";
//	const char *dst_file = "/Users/laurent/Desktop/test/CH_12_05_2014_CONV.jpg";

//	bool ok = FreeImage_JPEGTransform(src_file, dst_file, FIJPEG_OP_ROTATE_90, 0);
//	qDebug() << ok;

	return app.exec();
}
