#ifndef MV_EXIF_H
#define MV_EXIF_H

#include <FreeImage.h>
#include <QString>

namespace mv {

class Exif {

public:

	Exif(const QString& filePath);
	~Exif();
	void loadFile(const QString& filePath);
	int orientation() const;
	int rotation() const;

private:

	FIBITMAP* dib_;

};

}

#endif