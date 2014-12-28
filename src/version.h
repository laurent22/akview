#ifndef VERSION_H
#define VERSION_H

#define VER_FILEVERSION             1,0,376
#define VER_FILEVERSION_STR         "1.0.376\0"

#define VER_PRODUCTVERSION          1,0,376
#define VER_PRODUCTVERSION_STR      "1.0.376\0"

#define VER_COMPANYNAME_STR         "ackview"
#define VER_FILEDESCRIPTION_STR     "ackview"
#define VER_INTERNALNAME_STR        "ackview"
#define VER_LEGALCOPYRIGHT_STR      "Copyright (c) 2014-2015 Laurent Cozic"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "ackview.exe"
#define VER_PRODUCTNAME_STR         "ackview"
#define VER_DOMAIN_STR              "ackview.org"

namespace mv {
namespace version {

	int majorNumber();
	QString number();

} // version
}

#endif // VERSION_H
