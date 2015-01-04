#ifndef VERSION_H
#define VERSION_H

#define VER_FILEVERSION             ____VERSION_COMA____
#define VER_FILEVERSION_STR         "____VERSION____\0"

#define VER_PRODUCTVERSION          ____VERSION_COMA____
#define VER_PRODUCTVERSION_STR      "____VERSION____\0"

#define VER_COMPANYNAME_STR         "akview"
#define VER_FILEDESCRIPTION_STR     "akview"
#define VER_INTERNALNAME_STR        "akview"
#define VER_LEGALCOPYRIGHT_STR      "Copyright (c) 2014-2015 Laurent Cozic"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "akview.exe"
#define VER_PRODUCTNAME_STR         "akview"
#define VER_DOMAIN_STR              "akview.org"

namespace mv {
namespace version {

	int majorNumber();
	QString number();

} // version
}

#endif // VERSION_H
