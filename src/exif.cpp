#include "exif.h"

namespace mv {

Exif::Exif(const QString& filePath) {
	dib_ = NULL;
	loadFile(filePath);
}

Exif::~Exif() {
	if (dib_) {
		FreeImage_Unload(dib_);
		dib_ = NULL;
	}
}

void Exif::loadFile(const QString& filePath) {
	const char *cfilePath = filePath.toStdString().c_str();

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(cfilePath, 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(cfilePath);

	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		dib_ = FreeImage_Load(fif, cfilePath, FIF_LOAD_NOPIXELS);
	} else {
		qWarning() << "Format not supported:" << filePath;
	}
}

int Exif::orientation() const {
	if (!dib_) return 1;

	//   1        2       3      4         5            6           7          8

	// 888888  888888      88  88      8888888888  88                  88  8888888888
	// 88          88      88  88      88  88      88  88          88  88      88  88
	// 8888      8888    8888  8888    88          8888888888  8888888888          88
	// 88          88      88  88
	// 88          88  888888  888888

	FITAG *tag = NULL;
	FreeImage_GetMetadata(FIMD_EXIF_MAIN, dib_, "Orientation", &tag);
	short* v = (short*)FreeImage_GetTagValue(tag);
	if (!v) return 1;
	
	return *v;
}

int Exif::rotation() const {
	int o = orientation();
	if (o == 1 || o == 2) return 0;
	if (o == 3 || o == 4) return 180;
	if (o == 5 || o == 6) return 270;
	if (o == 7 || o == 8) return 90;
	return 0;
}

}